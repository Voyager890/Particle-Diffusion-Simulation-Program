cmake_minimum_required(VERSION 3.10.0)
project(particleDiffusion_Simulation VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

# External libraries
find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glm)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/class_bufferObjectsInitHelper)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/class_particles)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/proceduralGeometry)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/class_particleInitHelper)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/debug_tools)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/func_programStartMenu)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/func_physicsEngine)


add_executable(Diffusion
src/main.cpp
)

# Debugging Compile Flags
#target_compile_options(Diffusion PRIVATE -fsanitize=address -g -fno-omit-frame-pointer -Wall)
#target_link_options(Diffusion PRIVATE -fsanitize=address)

target_link_libraries(Diffusion PRIVATE
    # External Libs
    glfw
    OpenGL::GL
    glm
    glad

    # Internal Libs
    bufferObjectsInitHelper
    particles
    particleInitHelper

    programStartMenu
    proceduralGeometry

    physicsEngine
)

target_link_libraries(Diffusion PUBLIC
  debug_tools
)

target_include_directories(Diffusion PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

# Shader Path Definition
file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders" SHADER_PATH_DIR)
target_compile_definitions(Diffusion PRIVATE
    SHADER_PATH="${SHADER_PATH_DIR}"
)

