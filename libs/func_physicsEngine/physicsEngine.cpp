#include "physicsEngine.h"
#include "class_particles/particles.h"

#include <cmath>
#include <glm/common.hpp>
#include <glm/geometric.hpp>


void physicsEngine(class_particleType **&particleTypePointer, const int particleTypesAmount, const float borderArea){
    const double borderDisplacement = std::cbrt(borderArea) / 2.0;

    for(int current_particleType = 0; current_particleType < particleTypesAmount; current_particleType++){
        for(int current_particle = 0; current_particle < particleTypePointer[current_particleType]->particleCount; current_particle++){


            borderCollisionHandler(particleTypePointer[current_particleType]->particle[current_particle],
                                   particleTypePointer[current_particleType]->particleRadius,
                                   borderDisplacement);
        }
    }
}

void borderCollisionHandler(class_particle& particle, const float particleRadius, const double& borderDisplacement){
    // Toggles direction based on which axis border was crossed
    // for(int i = 0; i < 3; i++){
    //     if((glm::abs(particle.position[i]) + particleRadius) > borderDisplacement){
    //         particle.velocity[i] *= -1;
    //     }
    //     particle.position[i] += particle.velocity[i];
    // }
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