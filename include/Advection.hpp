// filepath: /Users/faisalshahin/Documents/Projects/Jobs_Projects/CFD/include/Advection.hpp
#ifndef ADVECTION_HPP
#define ADVECTION_HPP

#include <vector>

struct Grid3D {
    int width, height, depth;
    std::vector<std::vector<std::vector<float>>> u, v, w;
};

void advect3D(Grid3D &grid, float dt);

#endif // ADVECTION_HPP