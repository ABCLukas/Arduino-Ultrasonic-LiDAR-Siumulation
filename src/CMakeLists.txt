cmake_minimum_required(VERSION 3.10)
project(LiDARSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 (required for Renderer)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add the source files
add_executable(LiDARSimulation
    Main.cpp
    LineParser.cpp
    PointCloud.cpp
    Renderer.cpp
    SerialReader.cpp
)

# Add include directories
target_include_directories(LiDARSimulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries
target_link_libraries(LiDARSimulation ${SDL2_LIBRARIES})