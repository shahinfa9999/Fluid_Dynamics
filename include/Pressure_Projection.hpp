#ifndef Pressure_Projection_HPP
#define Pressure_Projection_HPP

#include "3d_Grid.hpp"

/**
 * @brief Project the velocity field to be mass conserving
 * 
 * @param grid 3D grid of velocity components and pressure
 * @param dt time step
 */
/*
struct Grid3D {
    int width, height, depth;
    float cellSize;
    std::vector<std::vector<std::vector<float>>> u, v, w;
    std::vector<std::vector<std::vector<float>>> pressure;
    std::vector<std::vector<std::vector<float>>> density;

    Grid3D(int w, int h, int d, float cs)
        : width(w), height(h), depth(d), cellSize(cs),
          u(w, std::vector<std::vector<float>>(h, std::vector<float>(d))),
          v(w, std::vector<std::vector<float>>(h, std::vector<float>(d))),
          w(w, std::vector<std::vector<float>>(h, std::vector<float>(d))),
          pressure(w, std::vector<std::vector<float>>(h, std::vector<float>(d))),
          density(w, std::vector<std::vector<float>>(h, std::vector<float>(d))) {}
};
*/
void project3D(Grid3D &grid, float dt);

#endif // Pressure_Projection_HPP
