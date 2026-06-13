add_library(programStartMenu STATIC
    programStartMenu.cpp
)

target_include_directories(programStartMenu PRIVATE
    ${CMAKE_SOURCE_DIR}/libs
)

target_link_libraries(programStartMenu PRIVATE
    glm
    particleInitHelper
)

