//semi-Lagrangian advection
#include <algorithm>
#include <vector>
#include "../include/Advection.hpp"


/*
grid: 3D grid of velocity components
  dt: time step
*/

void advect3D(Grid3D &grid, float dt) {
    for (int z = 1; z < grid.depth - 1; z++) {
        for (int y = 1; y < grid.height - 1; y++) {
            for (int x = 1; x < grid.width - 1; x++) {
                float xPrev = x - grid.u[z][y][x] * dt;
                float yPrev = y - grid.v[z][y][x] * dt;
                float zPrev = z - grid.w[z][y][x] * dt;

                // Clamp to grid bounds
                xPrev = std::max(0.0f, std::min((float)grid.width - 1, xPrev));
                yPrev = std::max(0.0f, std::min((float)grid.height - 1, yPrev));
                zPrev = std::max(0.0f, std::min((float)grid.depth - 1, zPrev));

                // Perform trilinear interpolation
                int x0 = (int)xPrev, x1 = x0 + 1;
                int y0 = (int)yPrev, y1 = y0 + 1;
                int z0 = (int)zPrev, z1 = z0 + 1;

                float s1 = xPrev - x0, s0 = 1 - s1;
                float t1 = yPrev - y0, t0 = 1 - t1;
                float u1 = zPrev - z0, u0 = 1 - u1;

                grid.u[z][y][x] = 
                    s0 * (t0 * (u0 * grid.u[z0][y0][x0] + u1 * grid.u[z1][y0][x0]) +
                          t1 * (u0 * grid.u[z0][y1][x0] + u1 * grid.u[z1][y1][x0])) +
                    s1 * (t0 * (u0 * grid.u[z0][y0][x1] + u1 * grid.u[z1][y0][x1]) +
                          t1 * (u0 * grid.u[z0][y1][x1] + u1 * grid.u[z1][y1][x1]));
                
                // Repeat for v and w
            }
        }
    }
}
