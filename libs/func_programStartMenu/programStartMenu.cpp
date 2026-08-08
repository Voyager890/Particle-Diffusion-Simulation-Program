#include "programStartMenu.h"

#include "class_particleInitHelper/particleInitHelper.h"
#include <cstddef>
#include <cstdio>
#include <glm/ext/vector_float3.hpp>

#include <fstream>
#include <iostream>
#include <string>

size_t programInit(class_particleInitHelper*& particleInitHelper){
  size_t count_particleTypes = 0;
  
  std::cout << "Use simulationConfig.txt file? (Enter a zero to decline)" << std::endl;
  int fileEntry = false;
  std::cin >> fileEntry;
  if(fileEntry != 0){
    count_particleTypes = fileInit(particleInitHelper);
  }

  if(count_particleTypes == 0){
    std::cout << std::endl << "-- Declined file input --" << std::endl << std::endl;

    std::cout << "Number of types of particles (Type 0 for defualt): " << std::endl;
    std::cin >> count_particleTypes;

    if(count_particleTypes != 0){
      particleInitHelper = new class_particleInitHelper(count_particleTypes);
      if(particleInitHelper == nullptr){std::cout << "ProgranInitHelper failed to be instantiated in programInit\n";}

      for(int i = 0; i < count_particleTypes; i++){
          std::cout << "Particle " << i + 1 << " Name: "; std::cin >>   particleInitHelper->name[i]; 
          std::cout << "Particle " << i + 1 << " Mass: "; std::cin >>   particleInitHelper->mass[i]; 
          std::cout << "Particle " << i + 1 << " Radius: "; std::cin >> particleInitHelper->radius[i]; 
          std::cout << "Particle " << i + 1 << " Amount: "; std::cin >> particleInitHelper->particleCount[i]; 
          
          rgbInput(particleInitHelper->color[i]);
      }

    }else{
      count_particleTypes = defaultInit(particleInitHelper); // Temporary test option 
    }
  }
    return count_particleTypes;
}

size_t defaultInit(class_particleInitHelper*& particleInitHelper){
    size_t count_particleTypes = 3;
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
    return count_particleTypes;
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

size_t fileInit(class_particleInitHelper *&particleInitHelper){

  std::cout << std::endl << "-- Configuring Simulation with Config File --" << std::endl;
  size_t count_particleTypes = 0;

  std::ifstream configFile("../simulationConfigs.txt");
  if(configFile.fail()){std::cout << "ERROR: UNABLE TO OPEN simulationConfigs.txt" << std::endl;return 1;}

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
  return count_particleTypes;

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
