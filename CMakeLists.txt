cmake_minimum_required(VERSION 3.10)
project(compression VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Set the default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Add the executable
add_executable(compression src/main.cpp src/heap.cpp src/huffman.cpp)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
