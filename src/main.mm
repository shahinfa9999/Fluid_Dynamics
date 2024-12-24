#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

// Include system headers first
#import <Foundation/Foundation.hpp>
#import <QuartzCore/QuartzCore.hpp>
#import <Metal/Metal.hpp>

// Include custom or third-party headers after system headers
#include <simd/simd.h>
#include "../include/simulation.hpp"
#include "../include/rendering.hpp"

#include <iostream>


int main() {
    @autoreleasepool {
        // Create a Metal device
        MTL::Device* device= MTLCreateSystemDefaultDevice();
        if (!device) {
            std::cerr << "Metal is not supported on this device.\n";
            return -1;
        }

        // Initialize Simulation and Renderer
        Simulation simulation(device);
        Renderer renderer(device);

        std::cout << "Simulation and Renderer initialized successfully!\n";
    }
    return 0;
}
