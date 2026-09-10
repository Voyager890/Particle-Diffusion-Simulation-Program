add_library(debug_tools STATIC
    debug_tools.cpp
)
target_include_directories(debug_tools PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(debug_tools PRIVATE
    bufferObjectsInitHelper
    glm
)
