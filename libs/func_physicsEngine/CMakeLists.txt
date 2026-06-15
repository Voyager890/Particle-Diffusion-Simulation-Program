add_library(physicsEngine STATIC
    physicsEngine.cpp
)

target_link_libraries(physicsEngine PRIVATE
    glm
    particles
)

target_include_directories(physicsEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/libs
)