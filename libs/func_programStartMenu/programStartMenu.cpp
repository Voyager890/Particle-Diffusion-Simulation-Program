#include "programStartMenu.h"

#include <iostream>
void programInit(int &iVerticesPerRing, float &iRadius){
    bool entryChoice = false;

    std::cout << "Enter 1 to proceed with manual entry of bufferObjects properties,\n"
              << "otherwise enter 0 to proceed with te initial properties\n";
    std::cin >> entryChoice;
    if (entryChoice){
        do{
            std::cout << "Enter the number of vertices per ring, must be a power of 2 greater than 2" << std::endl;
            std::cin >> iVerticesPerRing;
        }while(iVerticesPerRing <= 2 || iVerticesPerRing > 0 && ((iVerticesPerRing & (iVerticesPerRing - 1)) != 0)); 
        
        std::cout << "Enter the radius of the bufferObjects" << std::endl;
        std::cin >> iRadius;
    }
    return;
}

