#include "programStartMenu.h"

#include "class_particleInitHelper/particleInitHelper.h"
#include <glm/ext/vector_float3.hpp>

#include <fstream>
#include <iostream>
#include <string>

void programInit(class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea){

  std::cout << std::endl << "-- Enter the number of an option from below --" << std::endl
                         << " 1 : Defualt Values" << std::endl 
                         << " 2 : Manual CLI Entry" << std::endl
                         << " 3 : File Entry" << std::endl;
  int option = 0;
  std::cin >> option;
  
  bool initComplete = false;
  bool initValid = false;
  do{

    switch(option){
      case 1:
        defaultInit(particleInitHelper, count_particleTypes, borderArea);
        initComplete = true;
        break;
      case 2:
        manualInit(particleInitHelper, count_particleTypes, borderArea);
        initComplete = true;
        break;
      case 3:
        fileInit(particleInitHelper, count_particleTypes, borderArea);
        initComplete = true;
        break;
      default:
        std::cout << "INVALID ENTRY, TRY AGAIN" << std::endl;
        initComplete = false;
        break;
    }
    
    if(initComplete){
      initValid = areOptionsValid(particleInitHelper, count_particleTypes, borderArea);
      if(!initValid){
        delete particleInitHelper;
        initComplete = false;
      }
    }

  }while(!(initComplete && initValid));

}

void defaultInit(class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea){
    std::cout << std::endl << "-- Initializeing program with default values --" << std::endl;

    borderArea = 27;

    count_particleTypes = 3;

    particleInitHelper = new class_particleInitHelper(count_particleTypes);
    if(particleInitHelper == nullptr){std::cout << "ProgramInitHelper failed to instantiate in defaultInit\n";}

    particleInitHelper->name[0] = "Defualt Red";
    particleInitHelper->mass[0] = 1.0f;
    particleInitHelper->radius[0] = 0.2f;
    particleInitHelper->color[0] = glm::vec3(0.6f, 0.2f, 0.0f);
    particleInitHelper->particleCount[0] = 5;
    
    particleInitHelper->name[1] = "Defualt Blue";
    particleInitHelper->mass[1] = 1.0f;
    particleInitHelper->radius[1] = 0.2f;
    particleInitHelper->color[1] = glm::vec3(0.0f, 0.2f, 0.6f);
    particleInitHelper->particleCount[1] = 5;
    
    particleInitHelper->name[2] = "Defualt Green";
    particleInitHelper->mass[2] = 1.0f;
    particleInitHelper->radius[2] = 0.2f;
    particleInitHelper->color[2] = glm::vec3(0.1f, 0.6f, 0.1f);
    particleInitHelper->particleCount[2] = 5;
}

void manualInit (class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea){
  do{

  std::cout << "Enter the number of types of particles (must be a positive integer)" << std::endl;
  std::cin >> count_particleTypes;

  std::cout << "Enter border area (must be a positive value)" << std::endl;
  std::cin >> borderArea;
  
  }while(count_particleTypes < 1 && borderArea > 0);
  
  particleInitHelper = new class_particleInitHelper(count_particleTypes);
  if(particleInitHelper == nullptr){std::cout << "ProgramInitHelper failed to instantiate in defaultInit\n";}
    
  for(int i = 0; i < count_particleTypes; i++){
    std::cout << "Enter name of particle type " << i + 1 << std::endl;
    std::cin >> particleInitHelper->name[i];

    std::cout << "Enter the mass of particle type " << i + 1 << std::endl;
    std::cin >> particleInitHelper->mass[i];

    std::cout << "Enter the radius of particle type " << i + 1 << std::endl;
    std::cin >> particleInitHelper->radius[i];

    std::cout << "Enter the number of particle belonging to the particle type " << i + 1 << std::endl;
    std::cin >> particleInitHelper->particleCount[i];

    rgbInput(particleInitHelper->color[i]);
    
  }
}
void rgbInput(glm::vec3& color){
    bool valid = false;
    do{
        std::cout << "Enter particle's RGB color percentage" << std::endl;
        std::cout << "R%: "; std::cin >> color.x;
        valid = (color.x <= 100 && color.x >= 0) ? true : false;
        std::cout << "G%: "; std::cin >> color.y;
        valid = (color.y <= 100 && color.y >= 0) ? true : false;
        std::cout << "B%: "; std::cin >> color.z;
        valid = (color.z <= 100 && color.z >= 0) ? true : false;
        
        if(!valid){std::cout << "Percentages must be in range 0 to 100 inclusive. Try again" << std::endl;}
    }while(!valid);

    color *= 0.01;
}


void fileInit(class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea){

  std::cout << std::endl << "-- Configuring Simulation with Config File --" << std::endl;
  count_particleTypes = 0;

  std::ifstream configFile("../simulationConfigs.txt");
  if(configFile.fail()){std::cout << "ERROR: UNABLE TO OPEN simulationConfigs.txt" << std::endl; return;}
  
  std::string stringcount_particleTypes;
  getline(configFile, stringcount_particleTypes);

  count_particleTypes = std::stoul(stringcount_particleTypes);
  particleInitHelper = new class_particleInitHelper(count_particleTypes);
  const int totalHelperElements = 7;
  
  for(int i = 0; i < count_particleTypes; i++){
    
    particleInitHelper->name[i] = extractString(configFile);
    
    particleInitHelper->color[i][0] = std::stof(extractString(configFile));
    particleInitHelper->color[i][1] = std::stof(extractString(configFile));
    particleInitHelper->color[i][2] = std::stof(extractString(configFile));

    particleInitHelper->mass[i] = std::stof(extractString(configFile));
    particleInitHelper->radius[i] = std::stof(extractString(configFile));
    particleInitHelper->particleCount[i] = std::stof(extractString(configFile));

  }

  configFile.close();
}

std::string extractString(std::ifstream& file){
  char charInputBuffer;
  std::string strInputBuffer;
  
  bool stop = false;
  do{

    file >> charInputBuffer;
    if(charInputBuffer == ','){stop = true; continue;}
    strInputBuffer += charInputBuffer;

  }while(!stop);
  return strInputBuffer;
}

bool areOptionsValid(class_particleInitHelper* particleInitHelper, const int count_particleTypes, const int borderArea){
  bool validity = true;

  std::cout  << std::endl << "-- Validating Input Configurations --" << std::endl;
  

  if(count_particleTypes < 1){
    std::cout << "ERROR: THERE MUST BE A POSITIVE INTEGER VALUE FOR THE count_particleTypes\n";
    validity = false;
  }

  if(borderArea <= 0){
    std::cout << "ERROR: THERE MUST BE A POSITIVE VALUE FOR THE borderArea\n";
    validity = false;
  }

  for(int i = 0; i < count_particleTypes; i++){

    if(particleInitHelper->mass[i] <= 0){
      std::cout << "ERROR WITH PARTICLE " << i + 1 << " / Particle Name: " << *particleInitHelper->name << " || MASS MUST BE GREATER THAN ZERO" << std::endl;
      validity = false;
    }

    if(particleInitHelper->radius[i] <= 0){
      std::cout << "ERROR WITH PARTICLE " << i + 1 << " / Particle Name: " << *particleInitHelper->name << " || RADIUS MUST BE GREATER THAN ZERO" << std::endl;
      validity = false;
    }

    if(particleInitHelper->particleCount[i] < 0 || particleInitHelper->particleCount[i] > 10000){
      std::cout << "ERROR WITH PARTICLE " << i + 1 << " / Particle Name: " << *particleInitHelper->name << " || PARTICLE COUNT MUST BE IN RANGE 1 - 10,000 INCLUSIVE" << std::endl;
      validity = false;
    }
  }
  
  // Finally ensure all the particles can fit inside the given area
  if(validity){
    double totalParticlesVolume = 0;
    for(int i = 0; i < count_particleTypes; i++){
      const double radiusCubed = particleInitHelper->radius[i] * particleInitHelper->radius[i] * particleInitHelper->radius[i];
      totalParticlesVolume += particleInitHelper->particleCount[i] * (4.0/3.0) * 3.14 * radiusCubed;
    }
    const float maxThresholdRatio = 0.8;
    if(maxThresholdRatio <= (totalParticlesVolume/borderArea)){
      std::cout << "TOO MANY PARTICLES :: CANNOT FIT INSIDE SIMULATION BORDER" << std::endl;
      validity = false;
    }
  }
  return validity;
}
