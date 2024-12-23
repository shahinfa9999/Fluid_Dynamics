#import <Metal/Metal.h>
#include <simd/simd.h>
#include "../include/simulation.hpp"
#include "../include/rendering.hpp"

#include <iostream>

int main() {
    @autoreleasepool {
        // Create a Metal device
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device) {
            std::cerr << "Metal is not supported on this device.\n";
            return -1;
        }

        // Initialize Simulation and Renderer
        Simulation simulation((void*)device);
        Renderer renderer(device);

        std::cout << "Simulation and Renderer initialized successfully!\n";
    }
    return 0;
}