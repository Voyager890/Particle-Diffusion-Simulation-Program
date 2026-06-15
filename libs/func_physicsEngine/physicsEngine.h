#ifndef PHYSICS_ENGINE
#define PHYSICS_ENGINE

#include "class_particles/particles.h"

void physicsEngine(class_particleType**& particleTypePointer, const int particleTypesAmount);

bool isColliding(class_particle& subjectParticle, class_particle& possibleCollisionParticle);
bool isPastBoundry(class_particle& particle, const double& borderDisplacement);
#endif