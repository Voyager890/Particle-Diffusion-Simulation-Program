#include "programStartMenu.h"

#include "class_programInitHelper/programInitHelper.h"
#include <glm/ext/vector_float3.hpp>
#include <iostream>
void programInit(class_programInitHelper*& programInitHelper){
    if(1){
        defaultInit(programInitHelper);
    }else{
        long count_particleTypes = 0;
        std::cout << "Number of types of particles: "; std::cin >> count_particleTypes;
        programInitHelper = new class_programInitHelper(count_particleTypes);
        if(programInitHelper == nullptr){std::cout << "ProgranInitHelper Failed to get assigned a object\n";}

        for(int i = 0; i < count_particleTypes; i++){
            std::cout << "Particle " << i + 1 << " Name: "; std::cin >>   programInitHelper->name[i]; std::cout << std::endl;
            std::cout << "Particle " << i + 1 << " Mass: "; std::cin >>   programInitHelper->mass[i]; std::cout << std::endl;
            std::cout << "Particle " << i + 1 << " Radius: "; std::cin >> programInitHelper->radius[i]; std::cout << std::endl;
        
            std::cout << "Particle's RGB Color" << std::endl;
            std::cout << "Particle " << i + 1 << " R: "; std::cin >> programInitHelper->color[i].x; std::cout << std::endl;
            std::cout << "Particle " << i + 1 << " G: "; std::cin >> programInitHelper->color[i].y; std::cout << std::endl;
        std::cout << "Particle " << i + 1 << " B: "; std::cin >> programInitHelper->color[i].z; std::cout << std::endl;
        }
    }

}

void defaultInit(class_programInitHelper*& programInitHelper){
    long count_particleTypes = 1;
    programInitHelper = new class_programInitHelper(count_particleTypes);

    programInitHelper->name[0] = "Defualt";
    programInitHelper->mass[0] = 1.0f;
    programInitHelper->radius[0] = 1.0f;
    programInitHelper->color[0] = glm::vec3(1.0f);

}