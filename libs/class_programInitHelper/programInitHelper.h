#ifndef PROGRAM_INIT_HELPER
#define PROGRAM_INIT_HELPER

#include "glm/ext/vector_float3.hpp"
#include <string>

class class_programInitHelper{
public:
    const long count_particleTypes;

    class_programInitHelper(long& count_particleTypes);
    ~class_programInitHelper();

    std::string* name;
    float* mass;
    float* radius;
    glm::vec3* color;
};

#endif