#ifndef PHYSICS_ENGINE
#define PHYSICS_ENGINE

#include "class_particles/particles.h"


void physicsEngine(class_particleType **&particleTypePointer, const int particleTypesAmount, const float borderArea);
void borderCollisionHandler(class_particle& particle, const float particleRadius, const double& borderDisplacement);

#endif