#ifndef PARTICLES
#define PARTICLES

#include <string>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>


class class_particle{
    public:
    glm::vec3 velocity;
    glm::vec3 position;
};


class class_particleType {
    public:
    
    class_particleType(std::string particleName, glm::vec3 objectColor, float mass, float particleRadius, long particleCount);
    ~class_particleType();
    
    const std::string particleName;
    const glm::vec3 objectColor;
    const float mass;
    const float particleRadius;

    unsigned int vertexBufferObject;
    unsigned int vertexArrayObject;

    long particleCount;
    class_particle* particle;
};

void initParticleProperties(class_particleType**& particleTypePointer, const size_t countParticleType, const float borderArea, const float speed);
void removeOverlap(class_particleType**& particleTypePointer, const size_t countParticleType, const double borderDisplacement);
bool isOverlapping(class_particleType**& particleTypePointer,const size_t countParticleType, const size_t targetType, const size_t targetParticle);
#endif
