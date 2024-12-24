#include "rendering.hpp"
#include "<MetalKit/MetalKit.hpp>"


/*
Renderer::Renderer(MTL::Device*)
{
    // Initialize renderer with the provided Metal device
}

void Renderer::initialize()
{
    // Placeholder for renderer initialization
}

void Renderer::render(const std::vector<simd::float2>& positions) {
    // Placeholder for rendering particles
}
*/
onst int Renderer::kMaxFramesInFlight = 3;

Renderer::Renderer( MTL::Device* pDevice )
: _pDevice( pDevice->retain() )
, _angle ( 0.f )
, _frame( 0 )
, _animationIndex(0)
{
    _pCommandQueue = _pDevice->newCommandQueue();
    buildShaders();
    buildComputePipeline();
    buildDepthStencilStates();
    buildTextures();
    buildBuffers();

    _semaphore = dispatch_semaphore_create( Renderer::kMaxFramesInFlight );
}

Renderer::~Renderer()
{
    _pTextureAnimationBuffer->release();
    _pTexture->release();
    _pShaderLibrary->release();
    _pDepthStencilState->release();
    _pVertexDataBuffer->release();
    for ( int i = 0; i < kMaxFramesInFlight; ++i )
    {
        _pInstanceDataBuffer[i]->release();
    }
    for ( int i = 0; i < kMaxFramesInFlight; ++i )
    {
        _pCameraDataBuffer[i]->release();
    }
    _pIndexBuffer->release();
    _pComputePSO->release();
    _pPSO->release();
    _pCommandQueue->release();
    _pDevice->release();
}

namespace shader_types
{
    struct VertexData
    {
        simd::float3 position;
        simd::float3 normal;
        simd::float2 texcoord;
    };

    struct InstanceData
    {
        simd::float4x4 instanceTransform;
        simd::float3x3 instanceNormalTransform;
        simd::float4 instanceColor;
    };

    struct CameraData
    {
        simd::float4x4 perspectiveTransform;
        simd::float4x4 worldTransform;
        simd::float3x3 worldNormalTransform;
    };
}

void Renderer::buildShaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    const char* shaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        struct v2f
        {
            float4 position [[position]];
            float3 normal;
            half3 color;
            float2 texcoord;
        };

        struct VertexData
        {
            float3 position;
            float3 normal;
            float2 texcoord;
        };

        struct InstanceData
        {
            float4x4 instanceTransform;
            float3x3 instanceNormalTransform;
            float4 instanceColor;
        };

        struct CameraData
        {
            float4x4 perspectiveTransform;
            float4x4 worldTransform;
            float3x3 worldNormalTransform;
        };

        v2f vertex vertexMain( device const VertexData* vertexData [[buffer(0)]],
                               device const InstanceData* instanceData [[buffer(1)]],
                               device const CameraData& cameraData [[buffer(2)]],
                               uint vertexId [[vertex_id]],
                               uint instanceId [[instance_id]] )
        {
            v2f o;

            const device VertexData& vd = vertexData[ vertexId ];
            float4 pos = float4( vd.position, 1.0 );
            pos = instanceData[ instanceId ].instanceTransform * pos;
            pos = cameraData.perspectiveTransform * cameraData.worldTransform * pos;
            o.position = pos;

            float3 normal = instanceData[ instanceId ].instanceNormalTransform * vd.normal;
            normal = cameraData.worldNormalTransform * normal;
            o.normal = normal;

            o.texcoord = vd.texcoord.xy;

            o.color = half3( instanceData[ instanceId ].instanceColor.rgb );
            return o;
        }

        half4 fragment fragmentMain( v2f in [[stage_in]], texture2d< half, access::sample > tex [[texture(0)]] )
        {
            constexpr sampler s( address::repeat, filter::linear );
            half3 texel = tex.sample( s, in.texcoord ).rgb;

            // assume light coming from (front-top-right)
            float3 l = normalize(float3( 1.0, 1.0, 0.8 ));
            float3 n = normalize( in.normal );

            half ndotl = half( saturate( dot( n, l ) ) );

            half3 illum = (in.color * texel * 0.1) + (in.color * texel * ndotl);
            return half4( illum, 1.0 );
        }
    )";

    NS::Error* pError = nullptr;
    MTL::Library* pLibrary = _pDevice->newLibrary( NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError );
    if ( !pLibrary )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    MTL::Function* pVertexFn = pLibrary->newFunction( NS::String::string("vertexMain", UTF8StringEncoding) );
    MTL::Function* pFragFn = pLibrary->newFunction( NS::String::string("fragmentMain", UTF8StringEncoding) );

    MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    pDesc->setVertexFunction( pVertexFn );
    pDesc->setFragmentFunction( pFragFn );
    pDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB );
    pDesc->setDepthAttachmentPixelFormat( MTL::PixelFormat::PixelFormatDepth16Unorm );

    _pPSO = _pDevice->newRenderPipelineState( pDesc, &pError );
    if ( !_pPSO )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    pVertexFn->release();
    pFragFn->release();
    pDesc->release();
    _pShaderLibrary = pLibrary;
}

void Renderer::buildComputePipeline()
{
    const char* kernelSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        kernel void mandelbrot_set(texture2d< half, access::write > tex [[texture(0)]],
                                   uint2 index [[thread_position_in_grid]],
                                   uint2 gridSize [[threads_per_grid]],
                                   device const uint* frame [[buffer(0)]])
        {
            constexpr float kAnimationFrequency = 0.01;
            constexpr float kAnimationSpeed = 4;
            constexpr float kAnimationScaleLow = 0.62;
            constexpr float kAnimationScale = 0.38;

            constexpr float2 kMandelbrotPixelOffset = {-0.2, -0.35};
            constexpr float2 kMandelbrotOrigin = {-1.2, -0.32};
            constexpr float2 kMandelbrotScale = {2.2, 2.0};

            // Map time to zoom value in [kAnimationScaleLow, 1]
            float zoom = kAnimationScaleLow + kAnimationScale * cos(kAnimationFrequency * *frame);
            // Speed up zooming
            zoom = pow(zoom, kAnimationSpeed);

            //Scale
            float x0 = zoom * kMandelbrotScale.x * ((float)index.x / gridSize.x + kMandelbrotPixelOffset.x) + kMandelbrotOrigin.x;
            float y0 = zoom * kMandelbrotScale.y * ((float)index.y / gridSize.y + kMandelbrotPixelOffset.y) + kMandelbrotOrigin.y;

            // Implement Mandelbrot set
            float x = 0.0;
            float y = 0.0;
            uint iteration = 0;
            uint max_iteration = 1000;
            float xtmp = 0.0;
            while(x * x + y * y <= 4 && iteration < max_iteration)
            {
                xtmp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtmp;
                iteration += 1;
            }

            // Convert iteration result to colors
            half color = (0.5 + 0.5 * cos(3.0 + iteration * 0.15));
            tex.write(half4(color, color, color, 1.0), index, 0);
        })";
    NS::Error* pError = nullptr;

    MTL::Library* pComputeLibrary = _pDevice->newLibrary( NS::String::string(kernelSrc, NS::UTF8StringEncoding), nullptr, &pError );
    if ( !pComputeLibrary )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert(false);
    }

    MTL::Function* pMandelbrotFn = pComputeLibrary->newFunction( NS::String::string("mandelbrot_set", NS::UTF8StringEncoding) );
    _pComputePSO = _pDevice->newComputePipelineState( pMandelbrotFn, &pError );
    if ( !_pComputePSO )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert(false);
    }

    pMandelbrotFn->release();
    pComputeLibrary->release();
}

void Renderer::buildDepthStencilStates()
{
    MTL::DepthStencilDescriptor* pDsDesc = MTL::DepthStencilDescriptor::alloc()->init();
    pDsDesc->setDepthCompareFunction( MTL::CompareFunction::CompareFunctionLess );
    pDsDesc->setDepthWriteEnabled( true );

    _pDepthStencilState = _pDevice->newDepthStencilState( pDsDesc );

    pDsDesc->release();
}

void Renderer::buildTextures()
{
    MTL::TextureDescriptor* pTextureDesc = MTL::TextureDescriptor::alloc()->init();
    pTextureDesc->setWidth( kTextureWidth );
    pTextureDesc->setHeight( kTextureHeight );
    pTextureDesc->setPixelFormat( MTL::PixelFormatRGBA8Unorm );
    pTextureDesc->setTextureType( MTL::TextureType2D );
    pTextureDesc->setStorageMode( MTL::StorageModeManaged );
    pTextureDesc->setUsage( MTL::ResourceUsageSample | MTL::ResourceUsageRead | MTL::ResourceUsageWrite);

    MTL::Texture *pTexture = _pDevice->newTexture( pTextureDesc );
    _pTexture = pTexture;

    pTextureDesc->release();
}

void Renderer::buildBuffers()
{
    using simd::float2;
    using simd::float3;

    const float s = 0.5f;

    shader_types::VertexData verts[] = {
        //                                         Texture
        //   Positions           Normals         Coordinates
        { { -s, -s, +s }, {  0.f,  0.f,  1.f }, { 0.f, 1.f } },
        { { +s, -s, +s }, {  0.f,  0.f,  1.f }, { 1.f, 1.f } },
        { { +s, +s, +s }, {  0.f,  0.f,  1.f }, { 1.f, 0.f } },
        { { -s, +s, +s }, {  0.f,  0.f,  1.f }, { 0.f, 0.f } },

        { { +s, -s, +s }, {  1.f,  0.f,  0.f }, { 0.f, 1.f } },
        { { +s, -s, -s }, {  1.f,  0.f,  0.f }, { 1.f, 1.f } },
        { { +s, +s, -s }, {  1.f,  0.f,  0.f }, { 1.f, 0.f } },
        { { +s, +s, +s }, {  1.f,  0.f,  0.f }, { 0.f, 0.f } },

        { { +s, -s, -s }, {  0.f,  0.f, -1.f }, { 0.f, 1.f } },
        { { -s, -s, -s }, {  0.f,  0.f, -1.f }, { 1.f, 1.f } },
        { { -s, +s, -s }, {  0.f,  0.f, -1.f }, { 1.f, 0.f } },
        { { +s, +s, -s }, {  0.f,  0.f, -1.f }, { 0.f, 0.f } },

        { { -s, -s, -s }, { -1.f,  0.f,  0.f }, { 0.f, 1.f } },
        { { -s, -s, +s }, { -1.f,  0.f,  0.f }, { 1.f, 1.f } },
        { { -s, +s, +s }, { -1.f,  0.f,  0.f }, { 1.f, 0.f } },
        { { -s, +s, -s }, { -1.f,  0.f,  0.f }, { 0.f, 0.f } },

        { { -s, +s, +s }, {  0.f,  1.f,  0.f }, { 0.f, 1.f } },
        { { +s, +s, +s }, {  0.f,  1.f,  0.f }, { 1.f, 1.f } },
        { { +s, +s, -s }, {  0.f,  1.f,  0.f }, { 1.f, 0.f } },
        { { -s, +s, -s }, {  0.f,  1.f,  0.f }, { 0.f, 0.f } },

        { { -s, -s, -s }, {  0.f, -1.f,  0.f }, { 0.f, 1.f } },
        { { +s, -s, -s }, {  0.f, -1.f,  0.f }, { 1.f, 1.f } },
        { { +s, -s, +s }, {  0.f, -1.f,  0.f }, { 1.f, 0.f } },
        { { -s, -s, +s }, {  0.f, -1.f,  0.f }, { 0.f, 0.f } }
    };

    uint16_t indices[] = {
         0,  1,  2,  2,  3,  0, /* front */
         4,  5,  6,  6,  7,  4, /* right */
         8,  9, 10, 10, 11,  8, /* back */
        12, 13, 14, 14, 15, 12, /* left */
        16, 17, 18, 18, 19, 16, /* top */
        20, 21, 22, 22, 23, 20, /* bottom */
    };

    const size_t vertexDataSize = sizeof( verts );
    const size_t indexDataSize = sizeof( indices );

    MTL::Buffer* pVertexBuffer = _pDevice->newBuffer( vertexDataSize, MTL::ResourceStorageModeManaged );
    MTL::Buffer* pIndexBuffer = _pDevice->newBuffer( indexDataSize, MTL::ResourceStorageModeManaged );

    _pVertexDataBuffer = pVertexBuffer;
    _pIndexBuffer = pIndexBuffer;

    memcpy( _pVertexDataBuffer->contents(), verts, vertexDataSize );
    memcpy( _pIndexBuffer->contents(), indices, indexDataSize );

    _pVertexDataBuffer->didModifyRange( NS::Range::Make( 0, _pVertexDataBuffer->length() ) );
    _pIndexBuffer->didModifyRange( NS::Range::Make( 0, _pIndexBuffer->length() ) );

    const size_t instanceDataSize = kMaxFramesInFlight * kNumInstances * sizeof( shader_types::InstanceData );
    for ( size_t i = 0; i < kMaxFramesInFlight; ++i )
    {
        _pInstanceDataBuffer[ i ] = _pDevice->newBuffer( instanceDataSize, MTL::ResourceStorageModeManaged );
    }

    const size_t cameraDataSize = kMaxFramesInFlight * sizeof( shader_types::CameraData );
    for ( size_t i = 0; i < kMaxFramesInFlight; ++i )
    {
        _pCameraDataBuffer[ i ] = _pDevice->newBuffer( cameraDataSize, MTL::ResourceStorageModeManaged );
    }

    _pTextureAnimationBuffer = _pDevice->newBuffer( sizeof(uint), MTL::ResourceStorageModeManaged );
}

void Renderer::generateMandelbrotTexture( MTL::CommandBuffer* pCommandBuffer )
{
    assert(pCommandBuffer);

    uint* ptr = reinterpret_cast<uint*>(_pTextureAnimationBuffer->contents());
    *ptr = (_animationIndex++) % 5000;
    _pTextureAnimationBuffer->didModifyRange(NS::Range::Make(0, sizeof(uint)));

    MTL::ComputeCommandEncoder* pComputeEncoder = pCommandBuffer->computeCommandEncoder();

    pComputeEncoder->setComputePipelineState( _pComputePSO );
    pComputeEncoder->setTexture( _pTexture, 0 );
    pComputeEncoder->setBuffer(_pTextureAnimationBuffer, 0, 0);

    MTL::Size gridSize = MTL::Size( kTextureWidth, kTextureHeight, 1 );

    NS::UInteger threadGroupSize = _pComputePSO->maxTotalThreadsPerThreadgroup();
    MTL::Size threadgroupSize( threadGroupSize, 1, 1 );

    pComputeEncoder->dispatchThreads( gridSize, threadgroupSize );

    pComputeEncoder->endEncoding();
}

void Renderer::draw( MTK::View* pView )
{
    using simd::float3;
    using simd::float4;
    using simd::float4x4;

    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    _frame = (_frame + 1) % Renderer::kMaxFramesInFlight;
    MTL::Buffer* pInstanceDataBuffer = _pInstanceDataBuffer[ _frame ];

    MTL::CommandBuffer* pCmd = _pCommandQueue->commandBuffer();
    dispatch_semaphore_wait( _semaphore, DISPATCH_TIME_FOREVER );
    Renderer* pRenderer = this;
    pCmd->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd ){
        dispatch_semaphore_signal( pRenderer->_semaphore );
    });

    _angle += 0.002f;

    const float scl = 0.2f;
    shader_types::InstanceData* pInstanceData = reinterpret_cast< shader_types::InstanceData *>( pInstanceDataBuffer->contents() );

    float3 objectPosition = { 0.f, 0.f, -10.f };

    float4x4 rt = math::makeTranslate( objectPosition );
    float4x4 rr1 = math::makeYRotate( -_angle );
    float4x4 rr0 = math::makeXRotate( _angle * 0.5 );
    float4x4 rtInv = math::makeTranslate( { -objectPosition.x, -objectPosition.y, -objectPosition.z } );
    float4x4 fullObjectRot = rt * rr1 * rr0 * rtInv;

    size_t ix = 0;
    size_t iy = 0;
    size_t iz = 0;
    for ( size_t i = 0; i < kNumInstances; ++i )
    {
        if ( ix == kInstanceRows )
        {
            ix = 0;
            iy += 1;
        }
        if ( iy == kInstanceRows )
        {
            iy = 0;
            iz += 1;
        }

        float4x4 scale = math::makeScale( (float3){ scl, scl, scl } );
        float4x4 zrot = math::makeZRotate( _angle * sinf((float)ix) );
        float4x4 yrot = math::makeYRotate( _angle * cosf((float)iy));

        float x = ((float)ix - (float)kInstanceRows/2.f) * (2.f * scl) + scl;
        float y = ((float)iy - (float)kInstanceColumns/2.f) * (2.f * scl) + scl;
        float z = ((float)iz - (float)kInstanceDepth/2.f) * (2.f * scl);
        float4x4 translate = math::makeTranslate( math::add( objectPosition, { x, y, z } ) );

        pInstanceData[ i ].instanceTransform = fullObjectRot * translate * yrot * zrot * scale;
        pInstanceData[ i ].instanceNormalTransform = math::discardTranslation( pInstanceData[ i ].instanceTransform );

        float iDivNumInstances = i / (float)kNumInstances;
        float r = iDivNumInstances;
        float g = 1.0f - r;
        float b = sinf( M_PI * 2.0f * iDivNumInstances );
        pInstanceData[ i ].instanceColor = (float4){ r, g, b, 1.0f };

        ix += 1;
    }
    pInstanceDataBuffer->didModifyRange( NS::Range::Make( 0, pInstanceDataBuffer->length() ) );

    // Update camera state:

    MTL::Buffer* pCameraDataBuffer = _pCameraDataBuffer[ _frame ];
    shader_types::CameraData* pCameraData = reinterpret_cast< shader_types::CameraData *>( pCameraDataBuffer->contents() );
    pCameraData->perspectiveTransform = math::makePerspective( 45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f ) ;
    pCameraData->worldTransform = math::makeIdentity();
    pCameraData->worldNormalTransform = math::discardTranslation( pCameraData->worldTransform );
    pCameraDataBuffer->didModifyRange( NS::Range::Make( 0, sizeof( shader_types::CameraData ) ) );

    // Update texture:

    generateMandelbrotTexture( pCmd );

    // Begin render pass:

    MTL::RenderPassDescriptor* pRpd = pView->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = pCmd->renderCommandEncoder( pRpd );

    pEnc->setRenderPipelineState( _pPSO );
    pEnc->setDepthStencilState( _pDepthStencilState );

    pEnc->setVertexBuffer( _pVertexDataBuffer, /* offset */ 0, /* index */ 0 );
    pEnc->setVertexBuffer( pInstanceDataBuffer, /* offset */ 0, /* index */ 1 );
    pEnc->setVertexBuffer( pCameraDataBuffer, /* offset */ 0, /* index */ 2 );

    pEnc->setFragmentTexture( _pTexture, /* index */ 0 );

    pEnc->setCullMode( MTL::CullModeBack );
    pEnc->setFrontFacingWinding( MTL::Winding::WindingCounterClockwise );

    pEnc->drawIndexedPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle,
                                6 * 6, MTL::IndexType::IndexTypeUInt16,
                                _pIndexBuffer,
                                0,
                                kNumInstances );

    pEnc->endEncoding();
    pCmd->presentDrawable( pView->currentDrawable() );
    pCmd->commit();

    pPool->release();
}
