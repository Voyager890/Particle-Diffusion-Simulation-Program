#include "programInitHelper.h"

#include "glm/ext/vector_float3.hpp"
#include <string>

class_programInitHelper::class_programInitHelper(long& count_particleTypes):
count_particleTypes(count_particleTypes)
{
    name   = new std::string[count_particleTypes];
    color    = new glm::vec3[count_particleTypes];
    mass           = new float[count_particleTypes];
    radius = new float[count_particleTypes];
}

class_programInitHelper::~class_programInitHelper(){
    delete [] name; 
    delete [] color;
    delete [] mass;
    delete [] radius;
}
