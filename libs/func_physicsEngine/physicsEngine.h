#ifndef PHYSICS_ENGINE
#define PHYSICS_ENGINE

#include "class_particles/particles.h"

void physicsEngine(class_particleType**& particleTypePointer, const int& particleTypesAmount);

void particleCollisionHandler(class_particle& particle, const class_particleType**& particleTypePointer, const int particleTypesAmount);
void borderCollisionHandler(class_particle& particle, const double& borderDisplacement);

#endif