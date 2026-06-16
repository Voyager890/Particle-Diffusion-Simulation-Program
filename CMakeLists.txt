cmake_minimum_required(VERSION 3.10.0)
project(particleDiffusion_Simulation VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)


find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/class_bufferObjectsInitHelper)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/class_particles)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/func_proceduralSphere)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/class_particleInitHelper)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/debug_tools)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/func_programStartMenu)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/func_physicsEngine)



add_executable(Diffusion
src/main.cpp
)
#target_compile_options(Diffusion PRIVATE -g)

target_link_libraries(Diffusion PRIVATE
    # External Libs
    glfw
    OpenGL::GL
    glm
    glad
    # Custom Libs
    bufferObjectsInitHelper
    particles
    particleInitHelper

    programStartMenu
    proceduralSphere
    debug_tools

    physicsEngine
)

target_include_directories(Diffusion PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/libs
)

target_compile_definitions(Diffusion PRIVATE
    SHADER_PATH="${CMAKE_SOURCE_DIR}/src/shaders"
)


