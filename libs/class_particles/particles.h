#ifndef PARTICLES
#define PARTICLES

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "class_bufferObjects/bufferObjects.h"

class class_particleType {
    public:
    class_particleType(class_bufferObjects &bufferObjectLink, glm::vec3 objectColor, float mass);

    const class_bufferObjects* bufferObjectLink;
    const glm::vec3 objectColor;
    const float mass;

    int particleCount;
};
class class_particle{
    public:
    class_particle();

    class_particleType* particleTypeLink;
    glm::vec3 velocity;
    glm::mat4 position;
};
#endif
