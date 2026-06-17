#include "programStartMenu.h"

#include "class_particleInitHelper/particleInitHelper.h"
#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
size_t programInit(class_particleInitHelper*& particleInitHelper){
    size_t count_particleTypes = 0;
    std::cout << "Number of types of particles: "; std::cin >> count_particleTypes;
    
    if(count_particleTypes != 0){
        particleInitHelper = new class_particleInitHelper(count_particleTypes);
        if(particleInitHelper == nullptr){std::cout << "ProgranInitHelper failed to be instantiated in programInit\n";}

        for(int i = 0; i < count_particleTypes; i++){
            std::cout << "Particle " << i + 1 << " Name: "; std::cin >>   particleInitHelper->name[i]; 
            std::cout << "Particle " << i + 1 << " Mass: "; std::cin >>   particleInitHelper->mass[i]; 
            std::cout << "Particle " << i + 1 << " Radius: "; std::cin >> particleInitHelper->radius[i]; 
            std::cout << "Particle " << i + 1 << " Amount: "; std::cin >> particleInitHelper->particleCount[i]; 
        
            std::cout << "Particle's RGB Color" << std::endl;
            std::cout << "Particle " << i + 1 << " R: "; std::cin >> particleInitHelper->color[i].x;
            std::cout << "Particle " << i + 1 << " G: "; std::cin >> particleInitHelper->color[i].y;
            std::cout << "Particle " << i + 1 << " B: "; std::cin >> particleInitHelper->color[i].z;
            
        }
    }else{
        count_particleTypes = defaultInit(particleInitHelper); // Temporary test option 
    }
    return count_particleTypes;
}

size_t defaultInit(class_particleInitHelper*& particleInitHelper){
    size_t count_particleTypes = 2;
    particleInitHelper = new class_particleInitHelper(count_particleTypes);
    if(particleInitHelper == nullptr){std::cout << "ProgramInitHelper failed to instantiate in defaultInit\n";}

    particleInitHelper->name[0] = "Defualt Red";
    particleInitHelper->mass[0] = 1.0f;
    particleInitHelper->radius[0] = 0.2f;
    particleInitHelper->color[0].x = 0.7f;
    particleInitHelper->color[0].y = 0.2f;
    particleInitHelper->color[0].z = 0.0f;
    particleInitHelper->particleCount[0] = 5;
    
    particleInitHelper->name[1] = "Defualt Blue";
    particleInitHelper->mass[1] = 1.0f;
    particleInitHelper->radius[1] = 0.2f;
    particleInitHelper->color[1].x = 0.1f;
    particleInitHelper->color[1].y = 0.1f;
    particleInitHelper->color[1].z = 0.8f;
    particleInitHelper->particleCount[1] = 5;
    
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
}