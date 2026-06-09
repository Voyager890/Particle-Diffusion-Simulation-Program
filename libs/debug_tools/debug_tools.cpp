
#include <iostream>
#include "debug_tools.h"
#include "class_bufferObjects/bufferObjects.h"
void debug_vboDataDisplayer(class_bufferObjects &bufferObjects){
    std::cout << "Vertices Per Ring: " << bufferObjects.verticesPerRingCount << std::endl
              << "Total Vertices:    " << bufferObjects.totalVerticesCount << std::endl
              << "Layer Count:       " << bufferObjects.layerCount << std::endl
              << "Array Capacity:    " << bufferObjects.capacity << std::endl;

    const int elementsPerLayer = bufferObjects.verticesPerRingCount * bufferObjects.stride;
    for(int element = 0; element < bufferObjects.capacity; element++){
        if(element % (elementsPerLayer) == 0){
            std::cout << "\nLayer: " << element / elementsPerLayer << std::endl;
        }

        if(element % 6 == 0){
            std::cout << std::endl << bufferObjects.vertices[element] << ", ";
        }else{
        std::cout << bufferObjects.vertices[element] << ", ";
        }
    }

    std::cout << std:: endl;
}
void debug_eboDataDisplayer(class_bufferObjects &bufferObjects){
    std::cout << "Total Triangles: " << bufferObjects.eboCapacity / bufferObjects.eboVerticesPerTriangle << std::endl
              << "Total Elements:  " << bufferObjects.eboCapacity << std::endl << std::endl;

    
    for(int i = 0; i < bufferObjects.eboCapacity; ++i){
        if(i == bufferObjects.eboElementsPerVertexRing || i == bufferObjects.eboCapacity - bufferObjects.eboElementsPerVertexRing){
            std::cout << std::endl << "PRIME LAYER: " << std::endl;
        }

        if(i % 3 == 0){
            std::cout << std::endl << i / 3 + 1 << " | " << bufferObjects.eboData[i];
        }else{
            std::cout << ", " << bufferObjects.eboData[i];
        } 
    }

    std::cout << std::endl;
}