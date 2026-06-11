add_library(particles STATIC
    particles.cpp
)

target_link_libraries(particles PRIVATE
    glm
)

