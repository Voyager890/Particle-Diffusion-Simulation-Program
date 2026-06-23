#include "particles.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/geometric.hpp>
#include <random>

class_particle::class_particle()
{
    std::random_device rd;
    std::uniform_real_distribution<float>dist(-2.0,2.0);
    
    // Randomly assign a positon and velocity
    position = glm::vec3(0.0f);
    // position = glm::vec3(dist(rd), dist(rd), dist(rd));
    velocity = glm::vec3(dist(rd), dist(rd), dist(rd));
    velocity = glm::normalize(velocity);
    velocity *= 0.05f; // Replace with speed input
}



class_particleType::class_particleType(std::string particleName, glm::vec3 objectColor, float mass, float particleRadius, long particleCount):
particleName(particleName),
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
