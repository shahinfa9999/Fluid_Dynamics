#ifndef FLUID_SIMULATION_HPP
#define FLUID_SIMULATION_HPP

#include "../metal-cpp/Metal/Metal.hpp"
#include "../metal-cpp/Foundation/Foundation.hpp"
#include "../metal-cpp/QuartzCore/QuartzCore.hpp"




class FluidSimulation {
private:
    MTL::Device* device;                           // Metal device
    MTL::CommandQueue* commandQueue;               // Command queue
    MTL::ComputePipelineState* computePipelineState; // Compute pipeline
    MTL::Buffer* velocityBuffer;                   // Velocity buffer
    MTL::Buffer* pressureBuffer;                   // Pressure buffer

public:
    // Constructor that initializes Metal resources
    FluidSimulation(MTL::Device* device);

    // Function to step through the simulation
    void stepSimulation();

    // Function to update the simulation
    void update();
    
    void renderFluid();
};

#endif // FLUID_SIMULATION_HPP
