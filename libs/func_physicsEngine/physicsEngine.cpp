#include "physicsEngine.h"

#include <cmath>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <iostream>

#include "debug_tools/debug_tools.h"

void physicsEngine(class_particleType **&particleTypePointer,
                   const int particleTypesAmount, const float borderArea) {
  const double borderDisplacement = std::cbrt(borderArea) / 2.0;

  for (int currentType = 0; currentType < particleTypesAmount; currentType++) {
    for (int currentParticle = 0;
         currentParticle < particleTypePointer[currentType]->particleCount;
         currentParticle++) {

      particleCollisionHandler(particleTypePointer, currentType,
                               currentParticle, particleTypesAmount);

      borderCollisionHandler(
          particleTypePointer[currentType]->particle[currentParticle],
          particleTypePointer[currentType]->particleRadius, borderDisplacement);

      particleTypePointer[currentType]->particle[currentParticle].position = resultantDisplacment(particleTypePointer[currentType]->particle[currentParticle]);
      particleTypePointer[currentType]->particle[currentParticle].displacementBuffer = glm::vec3(0.0);
      
    }
  }
}

void borderCollisionHandler(class_particle &particle, const float particleRadius, const double &borderDisplacement){
  // Toggles direction based on which axis border was crossed

  bool borderCollision = false;

  do {
    borderCollision = false;
    const glm::vec3 nextPosition = resultantDisplacment(particle);
    for (int i = 0; i < 3; i++) {
      if ((glm::abs(nextPosition[i]) + particleRadius) < borderDisplacement) {
        continue;
      }
      borderCollision = true;
      const double trespassMagnitude = glm::abs(nextPosition[i]) - borderDisplacement;
      const double deflectionVector = std::copysign(deflectionVector, particle.position[i]);
      particle.displacementBuffer[i] += deflectionVector;
      particle.velocity[i] *= -1;
      

      debug_displayVec3(particle.displacementBuffer, "displacementBuffer");
    }
  } while (borderCollision);
}

void particleCollisionHandler(class_particleType **&particleTypePointer,
                              const size_t targetType, const size_t targetIndex,
                              const size_t particleTypesAmount) {
  for (int currentType = 0; currentType < particleTypesAmount; currentType++) {
    for (int currentParticle = 0;
         currentParticle < particleTypePointer[currentType]->particleCount;
         currentParticle++) {
      if (targetType == currentType && targetIndex == currentParticle) {
        continue;
      }

      class_particle &target =
          particleTypePointer[targetType]->particle[targetIndex];
      class_particle &comperand =
          particleTypePointer[currentType]->particle[currentParticle];

      const double distance =
          glm::distance(target.position, comperand.position);
      const double maxDistance =
          particleTypePointer[targetType]->particleRadius +
          particleTypePointer[currentType]->particleRadius;

      if (distance > maxDistance) {
        continue;
      }

      const double targetMass = particleTypePointer[targetType]->mass;
      const double comperandMass = particleTypePointer[currentType]->mass;

      const double aMass = particleTypePointer[targetType]->mass;
      glm::vec3 &aVelocity =
          particleTypePointer[targetType]->particle[targetIndex].velocity;
      const double bMass = particleTypePointer[currentType]->mass;
      glm::vec3 &bVelocity =
          particleTypePointer[currentType]->particle[currentParticle].velocity;

      finalVelocityCalculator(aMass, bMass, aVelocity, bVelocity);
    }
  }
}

void finalVelocityCalculator(const double aMass, const double bMass,
                             glm::vec3 &aVelocity, glm::vec3 &bVelocity) {
  const float coeffOfResititution = 1.0f;

  const float totalMass = aMass + bMass;

  glm::vec3 aVelocityFinal(0.0f);
  glm::vec3 bVelocityFinal(0.0f);
  for (int i = 0; i < 3; i++) {
    const double totalMomentum = aMass * aVelocity[i] + bMass * bVelocity[i];

    const double initialVelocityDifference = aVelocity[i] - bVelocity[i];

    bVelocityFinal[i] =
        (aMass * (coeffOfResititution * initialVelocityDifference) +
         totalMomentum) /
        totalMass;
    aVelocityFinal[i] = (totalMomentum - (bMass * bVelocityFinal[i])) / aMass;
  }
  aVelocity = aVelocityFinal;
  bVelocity = bVelocityFinal;
}

glm::vec3 resultantDisplacment(class_particle &particle) {
  glm::vec3 resultant(0.0);

  for (int i = 0; i < 3; i++) {
    if (particle.displacementBuffer[i] != 0) {
      resultant[i] = particle.displacementBuffer[i];
    } else {
      resultant[i] = particle.position[i] + particle.velocity[i];
    }
  }
  
  
  return resultant;
}
