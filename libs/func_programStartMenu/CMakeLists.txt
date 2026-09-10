add_library(programStartMenu STATIC
    programStartMenu.cpp
)

target_include_directories(programStartMenu PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(programStartMenu PRIVATE
    glm
    particleInitHelper
)

