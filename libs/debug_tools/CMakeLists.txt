add_library(debug_tools STATIC
    debug_tools.cpp
)
target_include_directories(debug_tools PRIVATE
    ${CMAKE_SOURCE_DIR}/libs)

target_link_libraries(debug_tools PRIVATE
    bufferObjectsInitHelper
    glm
)
