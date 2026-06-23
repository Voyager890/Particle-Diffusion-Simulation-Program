
#include <cmath>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>
#include <math.h>
#include <cmath>

#include "class_bufferObjectsInitHelper/bufferObjectsInitHelper.h"

#include "proceduralGeometry.h"

void vertexRingGenerator(class_bufferObjectsInitHelper &bufferObjects){
    for(int currentLayer = 0; currentLayer < bufferObjects.layerCount; currentLayer++){
        const float pitch = glm::radians(90.0 - ((currentLayer + 1) * bufferObjects.displacementAngle));
        const float layerRadius = (cos(pitch) * bufferObjects.radius); 

        for(int step = 0; step < bufferObjects.verticesPerRingCount; step++){
            const float yaw = glm::radians(bufferObjects.displacementAngle * step);
            const int currentVertex = (bufferObjects.stride * step) + (bufferObjects.verticesPerRingCount * bufferObjects.stride * (currentLayer));

            // Calculate and scale vertex position to fit the radius bufferObjects
            bufferObjects.vboData[currentVertex]     = cos(yaw)   * layerRadius;      // X position Attribute    
            bufferObjects.vboData[currentVertex + 1] = sin(yaw)   * layerRadius;      // Y position Attribute
            bufferObjects.vboData[currentVertex + 2] = sin(pitch) * bufferObjects.radius;    // Z position Attribute
            
            // Setting the normalized vertex normal data attribute
            const double vectorLength = std::sqrt(bufferObjects.vboData[currentVertex] * bufferObjects.vboData[currentVertex] 
                                      + bufferObjects.vboData[currentVertex + 1] * bufferObjects.vboData[currentVertex + 1]
                                      + bufferObjects.vboData[currentVertex + 2] * bufferObjects.vboData[currentVertex + 2]);
            bufferObjects.vboData[currentVertex + 3] = bufferObjects.vboData[currentVertex]     / vectorLength;
            bufferObjects.vboData[currentVertex + 4] = bufferObjects.vboData[currentVertex + 1] / vectorLength;
            bufferObjects.vboData[currentVertex + 5] = bufferObjects.vboData[currentVertex + 2] / vectorLength;
            
            // Correcting for floating point precision error in angle to radians conversion for basis vector positions
            for(int i = 0; i < 3; ++i){
                bufferObjects.vboData[currentVertex + i] = std::abs(bufferObjects.vboData[currentVertex + i]) < 0.0001 ? 0.0f : bufferObjects.vboData[currentVertex + i]; 
                bufferObjects.vboData[currentVertex + i] = std::abs(bufferObjects.vboData[currentVertex + i]) > 0.9999 * bufferObjects.radius ? std::copysign(1.0 * bufferObjects.radius, bufferObjects.vboData[currentVertex + i]) : bufferObjects.vboData[currentVertex + i];
            }
        }    
    }
    /*
    What is referred to as prime/primary vboData are the 2 vboData at the top and bottom of 
    the bufferObjectss generation. They are the only 2 vboData that are alone in their "layer".
    The layer the prime vboData reside in are not included in the layerCount attribute.
    */
    primaryVertexInit(bufferObjects);
    elementBufferGenerator(bufferObjects);
}

void primaryVertexInit(class_bufferObjectsInitHelper &bufferObjects){
    float *newBuffer = new float[bufferObjects.capacity + (2 * bufferObjects.stride)]{0.0f};

    const int offset = bufferObjects.stride * 1; // How many starting elements to offset by 
    for(int i = 0; i < bufferObjects.capacity; ++i){newBuffer[i + offset] = bufferObjects.vboData[i];};
    
    // Position data
    newBuffer[0] = 0.0f * bufferObjects.radius;  
    newBuffer[1] = 0.0f * bufferObjects.radius;  
    newBuffer[2] = 1.0f * bufferObjects.radius;
    // Normal data  
    newBuffer[3] = 0.0f;  
    newBuffer[4] = 0.0f;  
    newBuffer[5] = 1.0f;
    
    // Second prime vertex
    const int lastVertexElementStart = bufferObjects.capacity + bufferObjects.stride; 
    // Position Data
    newBuffer[lastVertexElementStart]     =  0.0f * bufferObjects.radius;
    newBuffer[lastVertexElementStart + 1] =  0.0f * bufferObjects.radius;
    newBuffer[lastVertexElementStart + 2] = -1.0f * bufferObjects.radius;
    // Normal data
    newBuffer[lastVertexElementStart + 3] = 0.0f;
    newBuffer[lastVertexElementStart + 4] = 0.0f;
    newBuffer[lastVertexElementStart + 5] = -1.0f;


    bufferObjects.totalVerticesCount += 2;
    bufferObjects.capacity = bufferObjects.totalVerticesCount * bufferObjects.stride;
    delete[] bufferObjects.vboData;
    bufferObjects.vboData = newBuffer;
}

void elementBufferGenerator(class_bufferObjectsInitHelper &bufferObjects){
    // Initialize first prime's element data
    bufferObjects.eboData[0] = 0;
    bufferObjects.eboData[1] = 1;      
    int inputData = 2;
    for(int i = 2; i + 2 < bufferObjects.eboElementsPerVertexRing; i += bufferObjects.eboVerticesPerTriangle){
        bufferObjects.eboData[i]     = inputData;
        bufferObjects.eboData[i + 2] = inputData;
        inputData++;
    }
    bufferObjects.eboData[bufferObjects.eboElementsPerVertexRing - 1] = 1; 
    
    // Initialize second prime's element data
    const int finalVertexNumber = bufferObjects.totalVerticesCount - 1; // aka last lone vertex
    const int primeLayerStartVertex = bufferObjects.totalVerticesCount - bufferObjects.verticesPerRingCount - 1; // -1 since ebo values start at 0
    const int primeLayerStartElement = bufferObjects.eboCapacity - bufferObjects.eboElementsPerVertexRing;
    
    bufferObjects.eboData[primeLayerStartElement]     = finalVertexNumber; 
    bufferObjects.eboData[primeLayerStartElement + 1] = primeLayerStartVertex; 
    bufferObjects.eboData[primeLayerStartElement + 2] = bufferObjects.eboData[primeLayerStartElement + 1] + 1;

    for(int i = primeLayerStartElement + bufferObjects.eboVerticesPerTriangle; i < bufferObjects.eboCapacity; i += 3){
        bufferObjects.eboData[i]     = finalVertexNumber; 
        bufferObjects.eboData[i + 1] = bufferObjects.eboData[i - 1];
        bufferObjects.eboData[i + 2] = bufferObjects.eboData[i - 1] + 1;
    }

    bufferObjects.eboData[bufferObjects.eboCapacity - 1] = primeLayerStartVertex;

    // Generating element buffer data for the layers
    inputData = 1;
    const int firstPhaseOffset = bufferObjects.eboElementsPerVertexRing;
    for(int currentLayer = 0; currentLayer < bufferObjects.layerCount - 1; currentLayer++){
        int index = (bufferObjects.eboElementsPerVertexRing * currentLayer) + firstPhaseOffset;
        for(int i = index; i < index + bufferObjects.eboElementsPerVertexRing; i += 3){
            bufferObjects.eboData[i]     = inputData;
            bufferObjects.eboData[i + 1] = inputData + 1;
            bufferObjects.eboData[i + 2] = inputData + bufferObjects.verticesPerRingCount;

            inputData++;
        }
        bufferObjects.eboData[index + bufferObjects.eboElementsPerVertexRing - 2] =  bufferObjects.verticesPerRingCount * currentLayer + 1;
    }
    
    inputData = 1;
    const int secondPhaseOffset = bufferObjects.eboElementsPerVertexRing + ((bufferObjects.layerCount - 1) * bufferObjects.eboElementsPerVertexRing);
    for(int currentLayer = 0; currentLayer < bufferObjects.layerCount - 1; currentLayer++){
        int index = (bufferObjects.eboElementsPerVertexRing * currentLayer)+ secondPhaseOffset;
        for(int i = index; i < index + bufferObjects.eboElementsPerVertexRing; i += 3){
            bufferObjects.eboData[i]     = inputData; 
            bufferObjects.eboData[i + 1] = inputData + bufferObjects.verticesPerRingCount - 1;
            bufferObjects.eboData[i + 2] = inputData + bufferObjects.verticesPerRingCount;

            inputData++;
        }
        bufferObjects.eboData[index + 1] = (currentLayer + 1) * bufferObjects.verticesPerRingCount + bufferObjects.verticesPerRingCount;
    }

    
}