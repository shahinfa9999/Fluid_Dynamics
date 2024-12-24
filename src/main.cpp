#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

// Include system headers first
#include <Foundation/Foundation.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <Metal/Metal.hpp>

// Include custom or third-party headers after system headers
#include <simd/simd.h>
#include "../include/simulation.hpp"
#include "../include/rendering.hpp"

#include <iostream>

void simulate3D(Grid3D &grid, float dt) {
    advect3D(grid, dt);
    project3D(grid, dt);
    // Add other steps (e.g., diffusion, external forces)
}

int main() {
    
        // Create a Metal device
        MTL::Device* device= MTLCreateSystemDefaultDevice();
        if (!device) {
            std::cerr << "Metal is not supported on this device.\n";
            return -1;
        }
        Grid3D grid(64, 64, 64, 1.0f);
        float dt = 0.1f;

        // Initialize Simulation and Renderer
        Simulation simulation(device);
        Renderer renderer(device);
        for (int frame = 0; frame < 1000; frame++) {
        simulate3D(grid, dt);
        render(grid); // Implement rendering}
    }

        std::cout << "Simulation and Renderer initialized successfully!\n";
    return 0;
}
