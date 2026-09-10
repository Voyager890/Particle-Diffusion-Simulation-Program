add_library(physicsEngine STATIC
    physicsEngine.cpp
)

target_include_directories(physicsEngine PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(physicsEngine PRIVATE
    glm
    particles
)

