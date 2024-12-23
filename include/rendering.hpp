#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <vector>
#include <simd/simd.h> // For simd::float2

class Renderer {
public:
    explicit Renderer(void* device);
    void initialize();
    void render(const std::vector<simd::float2>& positions);

private:
    void* device; // Use a void pointer to avoid including Metal headers
};

#endif // RENDERING_HPP