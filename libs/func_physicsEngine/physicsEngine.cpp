#include "physicsEngine.h"
#include "class_particles/particles.h"
#include <cmath>
#include <glm/geometric.hpp>


void physicsEngine(class_particleType **&particleTypePointer, const int particleTypesAmount){
    for(int i = 0; i < particleTypesAmount; i++){
        for(int j = 0; j < particleTypesAmount; j++){
            if(i == j){continue;}



        }
    }
}

bool isCollidinig(class_particle &subjectParticle, class_particle &possibleCollisionParticle){
    const double positionDifferenceX = subjectParticle.position.x - possibleCollisionParticle.position.x;
    const double positionDifferenceY = subjectParticle.position.y - possibleCollisionParticle.position.y;
    const double positionDifferenceZ = subjectParticle.position.z - possibleCollisionParticle.position.z;
}

bool isPastBoundry(class_particle& particle, const double& borderDisplacement){

}
