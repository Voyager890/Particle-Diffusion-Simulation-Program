#include "particles.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <random>

class_particle::class_particle()
{
    std::random_device rd;
    std::uniform_real_distribution<float>dist(-2.0,2.0);
    
    // Randomly assign a positon and velocity
    position = glm::vec3(dist(rd), dist(rd), dist(rd));
    velocity = glm::vec3(dist(rd), dist(rd), dist(rd));
}



class_particleType::class_particleType(glm::vec3 objectColor, float mass, float particleRadius, long particleCount):
objectColor(objectColor),
mass(mass),
particleRadius(particleRadius),
particleCount(particleCount)
{
    particle = new class_particle[particleCount];
}

class_particleType::~class_particleType(){
    delete [] particle;
}
