#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <simd/simd.h> // For simd::float2

class Simulation {
public:
    // Forward declare the constructor to avoid including Metal headers
    explicit Simulation(void* device);
    void initializeParticles();
    void update();
    std::vector<simd::float2> getParticlePositions() const;

private:
    void* device; // Use a void pointer to avoid including Metal headers
    std::vector<simd::float2> positions;
};

#endif // SIMULATION_HPP