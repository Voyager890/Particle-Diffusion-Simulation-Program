#include "physicsEngine.h"

#include <cmath>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include <iostream>
#include "class_particles/particles.h"
#include "debug_tools/debug_tools.h"

void physicsEngine(class_particleType **&particleTypePointer,
                   const int particleTypesAmount, const float borderArea) {
  const double borderDisplacement = std::cbrt(borderArea) / 2.0;
  
  // dual processing? 
  for (int currentType = 0; currentType < particleTypesAmount; currentType++) {
    for (int currentParticle = 0; currentParticle < particleTypePointer[currentType]->particleCount; currentParticle++) {
      
      particleCollisionHandler(particleTypePointer, currentType,
                               currentParticle, particleTypesAmount);

      borderCollisionHandler(
          particleTypePointer[currentType]->particle[currentParticle],
          particleTypePointer[currentType]->particleRadius, borderDisplacement);

      particleTypePointer[currentType]->particle[currentParticle].position += particleTypePointer[currentType]->particle[currentParticle].velocity + particleTypePointer[currentType]->particle[currentParticle].positionCorrectionBuffer;
      particleTypePointer[currentType]->particle[currentParticle].positionCorrectionBuffer = glm::vec3(0.0);
      
    }
  }
}

void borderCollisionHandler(class_particle &particle, const float particleRadius, const double &borderDisplacement){
  // Toggles direction based on which axis border was crossed

  bool borderCollision = false;

  do {
    borderCollision = false;
    const glm::vec3 nextPosition = particle.position + particle.velocity + particle.positionCorrectionBuffer;
    for (int i = 0; i < 3; i++) {
      if (glm::abs(nextPosition[i]) + particleRadius < borderDisplacement) {continue;}
      

      borderCollision = true;
      //const double trespassMagnitude = glm::abs(nextPosition[i] + particleRadius) - borderDisplacement;
      //const double deflectionVector = std::copysign(glm::abs(particle.velocity[i]) + trespassMagnitude, particle.position[i]);

      particle.velocity[i] *= -1;
      const double trespassMagnitude = glm::abs(nextPosition[i]) + particleRadius - borderDisplacement;
      const double deflectionVector = std::copysign(trespassMagnitude, particle.velocity[i]);
      particle.positionCorrectionBuffer[i] += deflectionVector;

      //debug_instanceCounter("Border collision");
      std::cout << "DeflectionVector: " << deflectionVector << std::endl;
      debug_displayVec3(nextPosition, "Next Position");
      debug_displayVec3(particle.positionCorrectionBuffer, "positionCorrectionBuffer");
    }
  } while (borderCollision);
}

void particleCollisionHandler(class_particleType **&particleTypePointer, const size_t targetType, const size_t targetIndex, const size_t particleTypesAmount) {
  // To avoid dual proccessing of a particle pair. Start comparing particles after target particles memory location. Particles before it would have already processed itself with target. 
  bool firstLoop = true;

  for (int currentType = targetIndex; currentType < particleTypesAmount; currentType++) {
    for (int currentParticle = 0; currentParticle < particleTypePointer[currentType]->particleCount; currentParticle++) {

      if (firstLoop){
        firstLoop = false;
        currentParticle = targetIndex;
        continue;
      }

      const class_particle &target = particleTypePointer[targetType]->particle[targetIndex];
      const class_particle &comperand = particleTypePointer[currentType]->particle[currentParticle];
      
      const glm::vec3 targetNextPosition = target.position + target.velocity + target.positionCorrectionBuffer; 
      const glm::vec3 comperandNextPosition = comperand.position + comperand.velocity + comperand.positionCorrectionBuffer; 
      const glm::vec3 vectorDifference = targetNextPosition - comperandNextPosition;

      const float displacement = glm::length(vectorDifference);

      const double maxDisplacement = particleTypePointer[targetType]->particleRadius + particleTypePointer[currentType]->particleRadius;

      if (displacement > maxDisplacement) {continue;}
      
      //debug_instanceCounter(" : Particle collision");
      const double targetMass = particleTypePointer[targetType]->mass;
      const double comperandMass = particleTypePointer[currentType]->mass;

      const double aMass   = particleTypePointer[targetType]->mass;
      glm::vec3 &aVelocity = particleTypePointer[targetType]->particle[targetIndex].velocity;
      const double bMass   = particleTypePointer[currentType]->mass;
      glm::vec3 &bVelocity = particleTypePointer[currentType]->particle[currentParticle].velocity;

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

