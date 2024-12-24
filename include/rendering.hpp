#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <vector>
#include <simd/simd.h> // For simd::float2
//#include "Metal/Metal.hpp"
#include "../metal-cpp/Metal/Metal.hpp"

class Renderer {
public:
    explicit Renderer(MTL::Device*);
    void initialize();
    void render(const std::vector<simd::float2>& positions);

private:
    void* device; // Use a void pointer to avoid including Metal headers
};

#endif // RENDERING_HPP