add_library(proceduralGeometry STATIC
    proceduralSphere.cpp
    proceduralBoundry.cpp
)

target_include_directories(proceduralGeometry PRIVATE
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/external
)
target_link_libraries(proceduralGeometry PRIVATE
    glm
    bufferObjectsInitHelper
    glad
    OpenGL::GL
)