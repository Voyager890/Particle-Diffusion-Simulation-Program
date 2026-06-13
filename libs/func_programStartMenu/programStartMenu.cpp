#include "programStartMenu.h"

#include "class_particleInitHelper/particleInitHelper.h"
#include <glm/ext/vector_float3.hpp>
#include <iostream>
void programInit(class_particleInitHelper*& particleInitHelper){
    if(!1){
        defaultInit(particleInitHelper);
    }else{
        long count_particleTypes = 0;
        std::cout << "Number of types of particles: "; std::cin >> count_particleTypes;
        particleInitHelper = new class_particleInitHelper(count_particleTypes);
        if(particleInitHelper == nullptr){std::cout << "ProgranInitHelper Failed to get assigned a object\n";}

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
    }

}

void defaultInit(class_particleInitHelper*& particleInitHelper){
    long count_particleTypes = 1;
    particleInitHelper = new class_particleInitHelper(count_particleTypes);

    particleInitHelper->name[0] = "Defualt";
    particleInitHelper->mass[0] = 1.0f;
    particleInitHelper->radius[0] = 1.0f;
    particleInitHelper->color[0] = glm::vec3(1.0f);

}