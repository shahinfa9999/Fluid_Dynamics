//poision equation solver
#include <vector>
#include "../include/Pressure_Projection.hpp"

/*
 grid: 3D grid of velocity components and pressure
    dt: time step
*/
void project3D(Grid3D &grid, float dt) {
    std::vector<std::vector<std::vector<float>>> div(grid.depth,
        std::vector<std::vector<float>>(grid.height, std::vector<float>(grid.width, 0.0f)));

    // Compute divergence
    for (int z = 1; z < grid.depth - 1; z++) {
        for (int y = 1; y < grid.height - 1; y++) {
            for (int x = 1; x < grid.width - 1; x++) {
                div[z][y][x] = -0.5f * grid.cellSize * (
                    grid.u[z][y][x + 1] - grid.u[z][y][x - 1] +
                    grid.v[z][y + 1][x] - grid.v[z][y - 1][x] +
                    grid.w[z + 1][y][x] - grid.w[z - 1][y][x]
                );
                grid.pressure[z][y][x] = 0.0f;
            }
        }
    }

    // Iterative solve
    for (int k = 0; k < 20; k++) {
        for (int z = 1; z < grid.depth - 1; z++) {
            for (int y = 1; y < grid.height - 1; y++) {
                for (int x = 1; x < grid.width - 1; x++) {
                    grid.pressure[z][y][x] = (div[z][y][x] +
                        grid.pressure[z][y][x + 1] + grid.pressure[z][y][x - 1] +
                        grid.pressure[z][y + 1][x] + grid.pressure[z][y - 1][x] +
                        grid.pressure[z + 1][y][x] + grid.pressure[z - 1][y][x]) / 6.0f;
                }
            }
        }
    }

    // Subtract pressure gradient from velocity
    for (int z = 1; z < grid.depth - 1; z++) {
        for (int y = 1; y < grid.height - 1; y++) {
            for (int x = 1; x < grid.width - 1; x++) {
                grid.u[z][y][x] -= 0.5f * (grid.pressure[z][y][x + 1] - grid.pressure[z][y][x - 1]) / grid.cellSize;
                grid.v[z][y][x] -= 0.5f * (grid.pressure[z][y + 1][x] - grid.pressure[z][y - 1][x]) / grid.cellSize;
                grid.w[z][y][x] -= 0.5f * (grid.pressure[z + 1][y][x] - grid.pressure[z - 1][y][x]) / grid.cellSize;
            }
        }
    }
}
