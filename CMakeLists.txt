cmake_minimum_required(VERSION 3.10.0)
project(particleDiffusion_Simulation VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/class_bufferObjects)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/class_particles)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/func_proceduralSphere)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/debug_tools)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/func_programStartMenu)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad)

set(EXECUTABLE_NAME "Diffusion")

add_executable(${EXECUTABLE_NAME}
    src/main.cpp
)

target_link_libraries(${EXECUTABLE_NAME}
    # External Libs
    glfw
    OpenGL::GL
    glm
    glad
    # Custom Libs
    bufferObjects
    particles
    
    programStartMenu
    proceduralSphere
    debug_tools
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/libs
)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
    SHADER_PATH="${CMAKE_SOURCE_DIR}/src/shaders"
)


