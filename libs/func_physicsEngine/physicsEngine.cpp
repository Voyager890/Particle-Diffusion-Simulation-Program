#include "physicsEngine.h"
#include "class_particles/particles.h"

#include <iostream>
#include <cmath>
#include <glm/common.hpp>
#include <glm/geometric.hpp>


void physicsEngine(class_particleType **&particleTypePointer, const int particleTypesAmount, const float borderArea){
    const double borderDisplacement = std::cbrt(borderArea) / 2.0;

    for(int current_particleType = 0; current_particleType < particleTypesAmount; current_particleType++){
        for(int current_particle = 0; current_particle < particleTypePointer[current_particleType]->particleCount; current_particle++){


            borderCollisionHandler(particleTypePointer[current_particleType]->particle[current_particle], borderDisplacement);
        }
    }
}

void borderCollisionHandler(class_particle& particle, const double& borderDisplacement){
    // Toggles direction based on which axis border was crossed
    for(int i = 0; i < 3; i++){
        particle.velocity[i] *= (glm::abs(particle.position[i]) > borderDisplacement) ? -1 : 1;
        std::cout << "Out of bounds on axis " << i << std::endl;
    }
}