add_library(particles STATIC
    particles.cpp
)

target_include_directories(particles PRIVATE
    ${CMAKE_SOURCE_DIR}/libs
)

target_link_libraries(particles PRIVATE
    bufferObjects
    glm
)

