add_library(programInitHelper STATIC
    programInitHelper.cpp
)

target_link_libraries(programInitHelper PRIVATE
    glm
)