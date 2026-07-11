#ifndef PHYSICS_ENGINE
#define PHYSICS_ENGINE

#include "class_particles/particles.h"
#include <cstddef>
#include <glm/vec3.hpp>
void physicsEngine(class_particleType**& particleTypePointer, const int particleTypesAmount, const float borderArea);

void borderCollisionHandler(class_particle& particle, const float particleRadius, const double& borderDisplacement);
void particleCollisionHandler(class_particleType**& particleTypePointer, const size_t targetType, const size_t targetIndex, const size_t particleTypesAmount);

glm::vec3 resultantDisplacment(const class_particle& particle);


void finalVelocityCalculator(const double aMass, const double bMass,glm::vec3& aVelocity, glm::vec3& bVelocity);
#endif
