cmake_minimum_required(VERSION 3.18)
project(FluidSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -objective-c++ -fobjc-arc -fmodules")

# Find Metal framework and required libraries
find_library(APPLE_FWK_FOUNDATION Foundation REQUIRED)
find_library(APPLE_FWK_QUARTZ_CORE QuartzCore REQUIRED)
find_library(APPLE_FWK_METAL Metal REQUIRED)
find_library(APPLE_FWK_METAL_KIT MetalKit REQUIRED)
find_library(APPLE_FWK_COCOA Cocoa REQUIRED)
find_library(METALKIT_LIB MetalKit) 

#find_library(METAL_LIB Metal)
#find_library(COCOA_LIB Cocoa)
#find_library(QUARTZCORE_LIB QuartzCore)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/metal-cpp)
include_directories(${PROJECT_SOURCE_DIR}/metal-cpp-extendions)

# Add source files
file(GLOB_RECURSE ALL_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(fluid_simulation ${ALL_SOURCES})


# Link libraries
target_link_libraries(fluid_simulation ${APPLE_FWK_FOUNDATION} ${APPLE_FWK_QUARTZ_CORE} ${APPLE_FWK_METAL} ${APPLE_FWK_METAL_KIT} ${APPLE_FWK_COCOA})
