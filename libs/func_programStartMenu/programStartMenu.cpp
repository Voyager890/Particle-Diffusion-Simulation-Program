#include "programStartMenu.h"

#include "class_particleInitHelper/particleInitHelper.h"
#include <cstddef>
#include <glm/ext/vector_float3.hpp>

#include <fstream>
#include <iostream>
#include <limits>
#include <locale>
#include <stdexcept>
#include <string>

void programInit(class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea){

  bool initComplete = false;
  bool initValid = false;

  do{

    std::cout << std::endl << "-- Enter the number of an option from below --" << std::endl
                          << " 1 : Defualt Values" << std::endl 
                          << " 2 : Manual CLI Entry" << std::endl
                          << " 3 : File Entry" << std::endl
                          << " 4 : Quit program" << std::endl;
    int option = 0;
    option = doubleUserInput();
    
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
      case 4:
          return;
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

        std::cout << std::endl << "-- TRY AGAIN --" << std::endl;
      }else{
        std::cout << std::endl << "-- Successfuly initialized simulation variables --" << std::endl;
      }
    }

  }while(!(initComplete && initValid));

  return;
}

void defaultInit(class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea){
    std::cout << std::endl << "-- Initializeing program with default values --" << std::endl;

    borderArea = 27;

    count_particleTypes = 3;

    particleInitHelper = new class_particleInitHelper(count_particleTypes);
    if(particleInitHelper == nullptr){std::cout << "ProgramInitHelper failed to instantiate in defaultInit\n";}

    particleInitHelper->name[0] = "Defualt_Red";
    particleInitHelper->mass[0] = 1.0f;
    particleInitHelper->radius[0] = 0.2f;
    particleInitHelper->color[0] = glm::vec3(0.6f, 0.2f, 0.0f);
    particleInitHelper->particleCount[0] = 5;
    
    particleInitHelper->name[1] = "Defualt_Blue";
    particleInitHelper->mass[1] = 1.0f;
    particleInitHelper->radius[1] = 0.2f;
    particleInitHelper->color[1] = glm::vec3(0.0f, 0.2f, 0.6f);
    particleInitHelper->particleCount[1] = 5;
    
    particleInitHelper->name[2] = "Defualt_Green";
    particleInitHelper->mass[2] = 1.0f;
    particleInitHelper->radius[2] = 0.2f;
    particleInitHelper->color[2] = glm::vec3(0.1f, 0.6f, 0.1f);
    particleInitHelper->particleCount[2] = 5;
}

void manualInit (class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea){
  do{

  std::cout << "Enter the number of types of particles (must be a positive integer)" << std::endl;
   count_particleTypes = doubleUserInput();

  std::cout << "Enter border area (must be a positive value)" << std::endl;
  borderArea = doubleUserInput();
  
  }while(count_particleTypes < 1 && borderArea > 0);
  
  particleInitHelper = new class_particleInitHelper(count_particleTypes);
  if(particleInitHelper == nullptr){std::cout << "ProgramInitHelper failed to instantiate in defaultInit\n";}
    
  for(int i = 0; i < count_particleTypes; i++){
    std::cout << "Enter name of particle type " << i + 1 << std::endl;
    std::cin >> particleInitHelper->name[i];
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the mass of particle type " << i + 1 << std::endl;
    particleInitHelper->mass[i] = doubleUserInput();

    std::cout << "Enter the radius of particle type " << i + 1 << std::endl;
    particleInitHelper->radius[i] = doubleUserInput();

    std::cout << "Enter the number of particle belonging to the particle type " << i + 1 << std::endl;
     particleInitHelper->particleCount[i] = doubleUserInput();

    rgbInput(particleInitHelper->color[i]);
    
  }
}
void rgbInput(glm::vec3& color){
    bool valid = false;
    do{
        std::cout << "Enter particle's RGB color percentage" << std::endl;
        std::cout << "R%: "; color.x = doubleUserInput();
        valid = (color.x <= 100 && color.x >= 0) ? true : false;
        std::cout << "G%: "; color.y = doubleUserInput();
        valid = (color.y <= 100 && color.y >= 0) ? true : false;
        std::cout << "B%: "; color.z = doubleUserInput();
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
  try{
    count_particleTypes = std::stoul(stringcount_particleTypes);
  }
  catch(std::invalid_argument& e){
    std::cout << "Caught exception : " << e.what() << std::endl;
    std::cout << "This means the function read a empty space instead of the required data. Ensure there are no empty lines/spaces" << std::endl;
    return;
  }

  if(count_particleTypes < 1){return;}
  
  std::string string_borderArea;
  getline(configFile, string_borderArea);
  try{
    borderArea = std::stof(string_borderArea);
  }
  catch(std::invalid_argument& e){
    std::cout << "Caught exception : " << e.what() << std::endl;
    std::cout << "This means the function read a empty space instead of the required data. Ensure there are no empty lines/spaces" << std::endl;
    return;
  }

  particleInitHelper = new class_particleInitHelper(count_particleTypes);
  const int totalHelperElements = 7;
  
  int currentElement = 1;
  int errorFlag = 0;
  for(int i = 0; i < count_particleTypes; i++){
    currentElement = 1;


    particleInitHelper->name[i] = extractString(configFile, currentElement, errorFlag);
    if(errorFlag != 0){break;}
    
    particleInitHelper->color[i][0] = std::stof(extractString(configFile, currentElement, errorFlag));
    if(errorFlag != 0){break;}
    particleInitHelper->color[i][1] = std::stof(extractString(configFile, currentElement, errorFlag));
    if(errorFlag != 0){break;}
    particleInitHelper->color[i][2] = std::stof(extractString(configFile, currentElement, errorFlag));
    if(errorFlag != 0){break;}

    particleInitHelper->mass[i] = std::stof(extractString(configFile, currentElement, errorFlag));
    if(errorFlag != 0){break;}
    particleInitHelper->radius[i] = std::stof(extractString(configFile, currentElement, errorFlag));
    if(errorFlag != 0){break;}
    particleInitHelper->particleCount[i] = std::stof(extractString(configFile, currentElement, errorFlag));
    if(errorFlag != 0){break;}

  }

  switch(errorFlag){
    case 1:
      std::cout << "ERROR at element : " << currentElement << " : Expected more statments but encountered a  ';' indicating the number of elements provided for the particle type were less than the required amount" << std::endl;
      break;
    case 2:
      std::cout << "ERROR at element : " << currentElement << " : Expcted end of particle type elements but encounted a ',' indicating that the number of elements provided for the particle type were more than the required amount" << std::endl;
     break;
    case 3:
      std::cout << "ERROR : Program tried to read beyond required number of elements for a given particle type" << std::endl;
      break;
    case 4:
      std::cout << "ERROR : Program started reading past end of file. Thus a element/particle type description termination character may be missing, or number of data types is input wrong" << std::endl;
      break;
  }

  configFile.close();
}

std::string extractString(std::ifstream& file, int& currentElement, int& errorFlag){
  const size_t totalNumberOfElements = 7;

  char charInputBuffer;
  std::string strInputBuffer;
  
  bool stop = false;
  do{
    if(file >> charInputBuffer){

      if(currentElement < totalNumberOfElements){
        if(charInputBuffer == ','){stop = true; continue;}
        if(charInputBuffer == ';'){stop = true; errorFlag = 1; continue;}
      }
      if(currentElement == totalNumberOfElements){
        if(charInputBuffer == ','){stop = true; errorFlag = 2;continue;}
        if(charInputBuffer == ';'){stop = true; continue;}
      }
      if(currentElement > totalNumberOfElements){stop == true; errorFlag = 3; continue;}

      strInputBuffer += charInputBuffer;

    }else{stop = true; errorFlag = 4; continue;} // IF file reached EOF
  }while(!stop);

  currentElement++;
  return strInputBuffer;
}

bool areOptionsValid(class_particleInitHelper* particleInitHelper, const int count_particleTypes, const int borderArea){
  bool validity = true;

  if(particleInitHelper == nullptr){return false;}

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
      std::cout << "ERROR WITH PARTICLE " << i + 1 << " / Particle Name: " << *particleInitHelper->name << " = MASS MUST BE GREATER THAN ZERO" << std::endl;
      validity = false;
    }

    if(particleInitHelper->radius[i] <= 0){
      std::cout << "ERROR WITH PARTICLE " << i + 1 << " / Particle Name: " << *particleInitHelper->name << " = RADIUS MUST BE GREATER THAN ZERO" << std::endl;
      validity = false;
    }

    if(particleInitHelper->particleCount[i] < 0 || particleInitHelper->particleCount[i] > 10000){
      std::cout << "ERROR WITH PARTICLE " << i + 1 << " / Particle Name: " << *particleInitHelper->name << " = PARTICLE COUNT MUST BE IN RANGE 1 - 10,000 INCLUSIVE" << std::endl;
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
      std::cout << "TOTAL PARTICLE VOLUME TOO GREAT. CANNOT REASONABLY FIT ALL PARTICLES (CURRENT maxThresholdRatio is " << maxThresholdRatio << ")" << std::endl;
      validity = false;
    }
  }
  return validity;
}

double doubleUserInput(){
  double input = 0;
  while(true){
    if(std::cin >> input){break;}
    else{
      std::cout << "Invalid input, try again" << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return input;
}
