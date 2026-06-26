#include "physicsEngine.h"
#include "class_particles/particles.h"

#include <cmath>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <iostream>


void physicsEngine(class_particleType **&particleTypePointer, const int particleTypesAmount, const float borderArea){
    const double borderDisplacement = std::cbrt(borderArea) / 2.0;

    for(int currentType = 0; currentType < particleTypesAmount; currentType++){
        for(int currentParticle = 0; currentParticle < particleTypePointer[currentType]->particleCount; currentParticle++){

            particleCollisionHandler(particleTypePointer, currentType, currentParticle, particleTypesAmount);
            
            borderCollisionHandler(particleTypePointer[currentType]->particle[currentParticle],
                                   particleTypePointer[currentType]->particleRadius,
                                   borderDisplacement);
        }
    }
}

void borderCollisionHandler(class_particle& particle, const float particleRadius, const double& borderDisplacement){
    // Toggles direction based on which axis border was crossed
    bool borderCollision = false;
    do{
        borderCollision = false;
        const glm::vec3 nextPosition = particle.position + particle.velocity;
        for(int i = 0; i < 3; i++){
            if((glm::abs(nextPosition[i]) + particleRadius) > borderDisplacement){
                borderCollision = true;
                const double trespassMagnitude = borderDisplacement + glm::abs(nextPosition[i]);
                particle.position[i] = std::copysign(borderDisplacement - trespassMagnitude, particle.position[i]);
                particle.velocity [i]*= -1;
            }else{
                particle.position[i] += particle.velocity[i];
            }
        
        }
    }while(borderCollision);
}

void particleCollisionHandler(class_particleType**& particleTypePointer, const size_t targetType, const size_t targetIndex, const size_t particleTypesAmount){
    for(int currentType = 0; currentType < particleTypesAmount; currentType++){
        for(int currentParticle = 0; currentParticle < particleTypePointer[currentType]->particleCount; currentParticle++){
            if(targetType == currentType && targetIndex == currentParticle){continue;}

            class_particle& target = particleTypePointer[targetType]->particle[targetIndex];
            class_particle& comperand = particleTypePointer[currentType]->particle[currentParticle];

            const double distance = glm::distance(target.position, comperand.position);
            const double maxDistance = particleTypePointer[targetType]->particleRadius + particleTypePointer[currentType]->particleRadius;
            
            if(distance > maxDistance){continue;}
            
            const double targetMass = particleTypePointer[targetType]->mass;
            const double comperandMass = particleTypePointer[currentParticle]->mass;

            // I think a POD will be best. 
        }
    }
}

