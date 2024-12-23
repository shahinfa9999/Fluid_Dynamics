#include <metal_stdlib>
using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
};

vertex Vertex vertex_main(const device float2 *positions [[buffer(0)]],
                          uint id [[vertex_id]]) {
    Vertex out;
    out.position = float4(positions[id], 0.0, 1.0);
    out.color = float4(0.0, 0.5, 1.0, 1.0); // Blue particles
    return out;
}

fragment float4 fragment_main(Vertex in [[stage_in]]) {
    return in.color;
}
