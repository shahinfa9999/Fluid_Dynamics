cmake_minimum_required(VERSION 3.18)
project(FluidSimulation)

set(CMAKE_CXX_STANDARD 17)

# Find Metal framework and required libraries
find_library(METAL_LIB Metal)
find_library(COCOA_LIB Cocoa)
find_library(QUARTZCORE_LIB QuartzCore)
# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
add_executable(fluid_simulation src/main.mm src/simulation.cpp src/rendering.cpp)

# Link libraries
target_link_libraries(fluid_simulation ${METAL_LIB} ${COCOA_LIB} ${QUARTZCORE_LIB})
