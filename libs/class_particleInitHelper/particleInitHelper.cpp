#include "particleInitHelper.h"

#include "glm/ext/vector_float3.hpp"
#include <string>

class_particleInitHelper::class_particleInitHelper(size_t& count_particleTypes)
{
    name   = new std::string[count_particleTypes];
    color  = new glm::vec3[count_particleTypes];
    mass   = new float[count_particleTypes];
    radius = new float[count_particleTypes];
    particleCount = new float[count_particleTypes];
}

class_particleInitHelper::~class_particleInitHelper(){
    delete [] name; 
    delete [] color;
    delete [] mass;
    delete [] radius;
    delete [] particleCount;
}

