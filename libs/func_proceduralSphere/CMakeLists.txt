add_library(proceduralSphere STATIC
    proceduralSphere.cpp
)

target_include_directories(proceduralSphere PRIVATE
    ${CMAKE_SOURCE_DIR}/libs
)
target_link_libraries(proceduralSphere PRIVATE
    glm
    bufferObjectsInitHelper
)