cmake_minimum_required(VERSION 4.2)
project(ParticleSimulator)

set(CMAKE_CXX_STANDARD 20)
find_package(raylib REQUIRED)

add_executable(particleSim src/main.cpp)
if (WIN32)
    target_link_libraries(particleSim raylib winmm gdi32)
else()
    target_link_libraries(particleSim raylib)
endif()
