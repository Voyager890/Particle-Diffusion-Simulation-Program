#include "particles.h"

#include <iostream>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/geometric.hpp>
#include <random>

class_particle::class_particle(){
  positionCorrectionBuffer = glm::vec3(0.0);
}

class_particleType::class_particleType(std::string particleName, glm::vec3 objectColor, float mass, float particleRadius, long particleCount):
particleName(particleName),
objectColor(objectColor),
mass(mass),
particleRadius(particleRadius),
particleCount(particleCount)
{
    particle = new class_particle[particleCount];
}

class_particleType::~class_particleType(){
    delete [] particle;
}

void initParticleProperties(class_particleType**& particleTypePointer, const size_t countParticleType, const float borderArea, const float speed){
  const double borderDisplacement = std::cbrt(borderArea) / 2;

  std::random_device rd;
  std::uniform_real_distribution<float>dist(-1.0,1.0);

  for(int typeN = 0; typeN < countParticleType; typeN++){
      const float maxDisplacementInBound = borderDisplacement - particleTypePointer[typeN]->particleRadius;
      for(int particleN = 0; particleN < particleTypePointer[typeN]->particleCount; particleN++){
          particleTypePointer[typeN]->particle[particleN].position = glm::vec3(dist(rd), dist(rd), dist(rd));
          particleTypePointer[typeN]->particle[particleN].position *= maxDisplacementInBound;

          particleTypePointer[typeN]->particle[particleN].velocity = glm::vec3(dist(rd), dist(rd), dist(rd));
          particleTypePointer[typeN]->particle[particleN].velocity *= speed;

      }
  }

  double totalParticlesVolume = 0;
  for(int i = 0; i < countParticleType; i++){
    const double radiusCubed = particleTypePointer[i]->particleRadius * particleTypePointer[i]->particleRadius * particleTypePointer[i]->particleRadius;
    totalParticlesVolume += particleTypePointer[i]->particleCount * (4.0/3.0) * 3.14 * radiusCubed;
  }
  
  if(0.8 > totalParticlesVolume/borderArea){
  std::cout << std::endl << "-- Proceeding to remove overlap --" << std::endl;
  removeOverlap(particleTypePointer, countParticleType, borderDisplacement); 
  }else{
    std::cout << "TOO MANY PARTICLES :: CANNOT FIT INSIDE SIMULATION BORDER :: SKIPPING PARTICLE OVERLAP REMOVAL" << std::endl;
  }
}

void removeOverlap(class_particleType**& particleTypePointer, const size_t countParticleType, const double borderDisplacement){

    std::random_device rd;
    std::uniform_real_distribution<float>dist(-1.0,1.0);

    for(int typeN = 0; typeN < countParticleType; typeN++){
        const float maxDisplacementInBound = borderDisplacement - particleTypePointer[typeN]->particleRadius;
        for(int particleN = 0; particleN < particleTypePointer[typeN]->particleCount; particleN++){
            while(isOverlapping(particleTypePointer, countParticleType, typeN, particleN)){
                particleTypePointer[typeN]->particle[particleN].position = glm::vec3(dist(rd), dist(rd), dist(rd));
                particleTypePointer[typeN]->particle[particleN].position *= maxDisplacementInBound;
            }
            
        }
    }
}

bool isOverlapping(class_particleType**& particleTypePointer,const size_t countParticleType, const size_t targetType, const size_t targetParticle){
    bool isOverlapping = false;
    
    const glm::vec3 targetPosition = particleTypePointer[targetType]->particle[targetParticle].position;
    const double targetRadius = particleTypePointer[targetType]->particleRadius;

    for(int typeN = 0; typeN < countParticleType; typeN++){
        const double maxDistance = targetRadius + particleTypePointer[typeN]->particleRadius;
        for(int particleN = 0; particleN < particleTypePointer[typeN]->particleCount; particleN++){
            if(typeN == targetType && particleN == targetParticle){continue;}

            const double distance = glm::distance(targetPosition,  particleTypePointer[typeN]->particle[particleN].position);
            if(distance <= maxDistance){isOverlapping = true;}
        }
    }
    
    return isOverlapping;
}
