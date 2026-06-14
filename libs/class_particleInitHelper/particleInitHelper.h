#ifndef PARTICLE_INIT_HELPER
#define PARTICLE_INIT_HELPER

#include "glm/ext/vector_float3.hpp"
#include <cstddef>
#include <string>

class class_particleInitHelper{
public:
    class_particleInitHelper(size_t& count_particleTypes);
    ~class_particleInitHelper();

    std::string* name;
    float* mass;
    float* radius;
    float* particleCount;
    glm::vec3* color;
};

#endif