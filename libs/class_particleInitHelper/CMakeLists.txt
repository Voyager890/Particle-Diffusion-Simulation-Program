add_library(particleInitHelper STATIC
    particleInitHelper.cpp
)

target_link_libraries(particleInitHelper PRIVATE
    glm
)