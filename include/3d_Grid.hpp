// Description: 3D grid class for storing
// velocity components, pressure, and density.
// The grid is initialized with the given width,
// height, depth, and cell size. The velocity
// components, pressure, and density are stored
// as 3D vectors.
#ifndef GRID3D_HPP
#define GRID3D_HPP

#include <vector>
class Grid3D {
public:
    int width, height, depth;
    int cellSize;
    std::vector<std::vector<std::vector<float>>> u, v, w;  // Velocity components
    std::vector<std::vector<std::vector<float>>> pressure;
    std::vector<std::vector<std::vector<float>>> density;

    Grid3D(int w, int h, int d, float size) : width(w), height(h), depth(d), cellSize(size) {
        u.resize(depth, std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0f)));
        v.resize(depth, std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0f)));
        //not sure why i need this->w here. look into it.
        this->w.resize(depth, std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0f)));
        pressure.resize(depth, std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0f)));
        density.resize(depth, std::vector<std::vector<float>>(height, std::vector<float>(width, 0.0f)));
    }
};

#endif // GRID3D_HPP

