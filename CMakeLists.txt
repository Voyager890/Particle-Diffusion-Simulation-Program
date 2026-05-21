cmake_minimum_required(VERSION 3.10.0)
project(particleDiffusion_Simulation VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
add_executable(particleDiffusion_Simulation
    src/main.cpp
    src/glad/glad.c
)

target_link_libraries(particleDiffusion_Simulation
    glfw
    OpenGL::GL
)

target_include_directories(particleDiffusion_Simulation PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

add_custom_command(TARGET particleDiffusion_Simulation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:particleDiffusion_Simulation>/shaders
)