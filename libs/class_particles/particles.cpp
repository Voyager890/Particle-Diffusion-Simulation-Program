#include "particles.h"



#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <random>

#include "class_bufferObjects/bufferObjects.h"
class_particleType::class_particleType(class_bufferObjects &bufferObjectLink, glm::vec3 objectColor, float mass, float particleRadius):
bufferObjectLink(&bufferObjectLink),
objectColor(objectColor),
mass(mass),
particleRadius(particleRadius)
{
    particleCount = 3; // There could be a feature that requires this to variable
}

class_particle::class_particle(class_particleType &particleTypeLink):
particleTypeLink(&particleTypeLink)
{
    std::random_device rd;
    std::uniform_real_distribution<float>dist(-2.0,2.0);
    
    position = glm::mat4(1.0f);
    position = glm::translate(position, glm::vec3(dist(rd), dist(rd), dist(rd)));

    velocity = glm::vec3(0.0f);
}
