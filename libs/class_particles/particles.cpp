#include "particles.h"



#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "class_bufferObjects/bufferObjects.h"
class_particleType::class_particleType(class_bufferObjects &bufferObjectLink, glm::vec3 objectColor, float mass):
bufferObjectLink(&bufferObjectLink),
objectColor(objectColor),
mass(mass) 
{
    particleCount = 1; // There could be a feature that requires this to variable
}

class_particle::class_particle(){
    velocity = glm::vec3(0.0f);
    position = glm::mat4(1.0f);
}
