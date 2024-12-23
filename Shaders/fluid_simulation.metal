#include <metal_stdlib>
using namespace metal;

struct Particle {
    float2 position;
    float2 velocity;
    float density;
    float pressure;
};

kernel void compute_sph(device Particle *particles [[buffer(0)]],
                        constant float &timeStep [[buffer(1)]],
                        constant float &restDensity [[buffer(2)]],
                        constant float &stiffness [[buffer(3)]],
                        constant float &viscosity [[buffer(4)]],
                        uint id [[thread_position_in_grid]]) {
    // Particle interaction (density and pressure computation)
    float density = 0.0;
    float2 force = float2(0.0, 0.0);
    Particle p = particles[id];

    for (uint i = 0; i < num_particles; ++i) {
        if (i == id) continue;
        Particle neighbor = particles[i];
        float2 diff = p.position - neighbor.position;
        float dist = length(diff);
        
        if (dist < smoothing_radius) {
            // Density kernel
            density += mass * kernel_poly6(dist);
            
            // Pressure force
            force += kernel_pressure_gradient(dist) * (p.pressure + neighbor.pressure) / (2.0 * density);
            
            // Viscosity force
            force += viscosity * kernel_viscosity_laplacian(dist) * (neighbor.velocity - p.velocity);
        }
    }
    
    p.density = density;
    p.pressure = stiffness * (density - restDensity);
    p.velocity += force * timeStep / density;
    p.position += p.velocity * timeStep;
    particles[id] = p;
}
