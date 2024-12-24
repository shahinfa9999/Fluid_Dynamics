#include "FluidSimulation.hpp"
#include <iostream>
#include <QuartzCore/CAMediaTiming.h>
#include <Foundation/Foundation.h>

FluidSimulation::FluidSimulation(MTL::Device* device) {
    this->device = device;
    int gridWidth = 128; // Set appropriate values for your simulation
    int gridHeight = 128;
    int gridDepth = 128;
    int gridSize = gridWidth * gridHeight * gridDepth;

    // Create a command queue for submitting commands
    commandQueue = device->newCommandQueue();

    // Load and compile the compute shader
    std::string shaderPath = "Shaders/fluid_simulation.metal";
    NS::Error* compilationError = nil;
    NS::String* nsShaderPath = NS::String::string(shaderPath.c_str(), NS::UTF8StringEncoding);
    MTL::Library* library = device->newLibrary(nsShaderPath, &compilationError);
    if (compilationError) {
        std::cerr << "Shader compilation failed: " << compilationError->localizedDescription()->utf8String() << std::endl;
        exit(1);
    }

    MTL::Function* computeFunction = library->newFunction(NS::String::string("fluidSimulation", NS::UTF8StringEncoding));
    if (!computeFunction) {
        std::cerr << "Failed to find the compute function." << std::endl;
        exit(1);
    }

    // Create compute pipeline state from the function
    NS::Error* pipelineError = nil;
    computePipelineState = device->newComputePipelineState(computeFunction, &pipelineError);
    if (pipelineError) {
        std::cerr << "Pipeline creation failed: " << pipelineError->localizedDescription()->utf8String() << std::endl;
        exit(1);
    }

    // Create buffers for simulation data (velocity and pressure)
    velocityBuffer = device->newBuffer(sizeof(float) * gridSize * gridSize * gridSize, MTL::ResourceStorageModeShared);
    pressureBuffer = device->newBuffer(sizeof(float) * gridSize * gridSize * gridSize, MTL::ResourceStorageModeShared);
}

void FluidSimulation::stepSimulation() {
    this->device = device;

    int gridWidth = 128; // Set appropriate values for your simulation
    int gridHeight = 128;
    int gridDepth = 128;
    //int gridSize = gridWidth * gridHeight * gridDepth;
    // Set up a Metal command buffer and command encoder
    MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();
    MTL::ComputeCommandEncoder* computeEncoder = commandBuffer->computeCommandEncoder();

    // Set up the compute encoder with our compute pipeline
    computeEncoder->setComputePipelineState(computePipelineState);
    computeEncoder->setBuffer(velocityBuffer, 0, 0);
    computeEncoder->setBuffer(pressureBuffer, 0, 1);

    // Define threadgroup sizes and dispatch
    MTL::Size gridSize = MTL::Size::Make(gridWidth, gridHeight, gridDepth);
    MTL::Size threadgroupSize = MTL::Size::Make(16, 16, 16); // Define per-thread size (adjust as needed)

    computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    computeEncoder->endEncoding();

    // Commit the command buffer for execution
    commandBuffer->commit();
    commandBuffer->waitUntilCompleted();
}

void FluidSimulation::update() {
    // Simulate the next time step
    stepSimulation();

    // Render the updated fluid state (you will need to set up a Metal render pipeline for this)
    renderFluid();
}
