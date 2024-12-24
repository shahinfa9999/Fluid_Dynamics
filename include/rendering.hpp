#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <vector>
#include <simd/simd.h> // For simd::float2
//#include "Metal/Metal.hpp"
#include "../metal-cpp/Metal/Metal.hpp"
#include <MetalKit/MetalKit.hpp>

class Renderer
{
    public:
        Renderer( MTL::Device* pDevice );
        ~Renderer();
        void buildShaders();
        void buildComputePipeline();
        void buildDepthStencilStates();
        void buildTextures();
        void buildBuffers();
        void generateMandelbrotTexture( MTL::CommandBuffer* pCommandBuffer );
        void draw( MTK::View* pView );

    private:
        MTL::Device* _pDevice;
        MTL::CommandQueue* _pCommandQueue;
        MTL::Library* _pShaderLibrary;
        MTL::RenderPipelineState* _pPSO;
        MTL::ComputePipelineState* _pComputePSO;
        MTL::DepthStencilState* _pDepthStencilState;
        MTL::Texture* _pTexture;
        MTL::Buffer* _pVertexDataBuffer;
        MTL::Buffer* _pInstanceDataBuffer[kMaxFramesInFlight];
        MTL::Buffer* _pCameraDataBuffer[kMaxFramesInFlight];
        MTL::Buffer* _pIndexBuffer;
        MTL::Buffer* _pTextureAnimationBuffer;
        float _angle;
        int _frame;
        dispatch_semaphore_t _semaphore;
        static const int kMaxFramesInFlight;
        uint _animationIndex;
};
/*
class Renderer {
public:
    explicit Renderer(MTL::Device*);
    void initialize();
    void render(const std::vector<simd::float2>& positions);

private:
    void* device; // Use a void pointer to avoid including Metal headers
};
*/
#endif // RENDERING_HPP