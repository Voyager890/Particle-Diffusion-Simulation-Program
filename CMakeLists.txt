cmake_minimum_required(VERSION 3.10.0)
project(particleDiffusion_Simulation VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

set(EXECUTABLE_NAME "Diffusion")

add_executable(${EXECUTABLE_NAME}
    src/main.cpp
    external/glad/glad.c
)

add_library(bufferObjects STATIC
    libs/class_bufferObjects/bufferObjects.cpp
)

add_library(particles STATIC
    libs/class_particles/particles.cpp
)

target_include_directories(particles PUBLIC
    ${CMAKE_SOURCE_DIR}/libs
)

target_link_libraries(particles PUBLIC
    bufferObjects
    glm
)

target_link_libraries(${EXECUTABLE_NAME}
    glfw
    OpenGL::GL
    glm
    bufferObjects
    particles
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/libs
)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
    SHADER_PATH="${CMAKE_SOURCE_DIR}/src/shaders"
)


