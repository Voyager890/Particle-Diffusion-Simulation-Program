#include "particles.h"

#include <cmath>
#include <cstddef>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/geometric.hpp>
#include <random>


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

void initParticleProperties(class_particleType**& particleTypePointer, const size_t countParticleType, const float borderArea, const float speed){
    const double borderDisplacement = std::cbrt(borderArea) / 2;

    std::random_device rd;
    std::uniform_real_distribution<float>dist(-1.0,1.0);

    for(int typeN = 0; typeN < countParticleType; typeN++){
        const float maxDisplacementInBound = borderDisplacement - particleTypePointer[typeN]->particleRadius;
        for(int particleN = 0; particleN < particleTypePointer[typeN]->particleCount; particleN++){
            particleTypePointer[typeN]->particle[particleN].position = glm::vec3(dist(rd), dist(rd), dist(rd));
            particleTypePointer[typeN]->particle[particleN].position *= maxDisplacementInBound;

            particleTypePointer[typeN]->particle[particleN].velocity = glm::vec3(dist(rd), dist(rd), dist(rd));
            particleTypePointer[typeN]->particle[particleN].velocity *= speed;
        }
    }
}

void removeOverlap(class_particleType**& particleTypePointer, const size_t countParticleType, const float borderArea){
    const double borderDisplacement = std::cbrt(borderArea) / 2;

    for(int typeN = 0; typeN < countParticleType; typeN++){
        const float maxDisplacementInBound = borderDisplacement - particleTypePointer[typeN]->particleRadius;
        for(int particleN = 0; particleN < particleTypePointer[typeN]->particleCount; particleN++){

            if(!isOverlapping(particleTypePointer, countParticleType, typeN, particleN)){continue;}

        }
    }
}

bool isOverlapping(class_particleType**& particleTypePointer,const size_t countParticleType, const size_t targetType, const size_t targetParticle){
    bool isOverlapping = false;
    
    const glm::vec3 targetPosition = particleTypePointer[targetType]->particle[targetParticle].position;
    const double targetRadius = particleTypePointer[targetType]->particleRadius;

    for(int typeN = 0; typeN < countParticleType; typeN++){
        const double maxDistance = targetRadius + particleTypePointer[typeN]->particleRadius;
        for(int particleN = 0; particleN < particleTypePointer[typeN]->particleCount; particleN++){
            const double distance = glm::distance(targetPosition,  particleTypePointer[typeN]->particle[particleN].position);
            if(distance <= maxDistance){isOverlapping = true;}
        }
    }

    return isOverlapping;
}