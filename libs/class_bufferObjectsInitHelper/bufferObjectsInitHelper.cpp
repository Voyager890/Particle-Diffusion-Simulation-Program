#include "bufferObjectsInitHelper.h"

class_bufferObjectsInitHelper::class_bufferObjectsInitHelper(const unsigned int verticesPerRingCount, const float radius) :
stride(6),
verticesPerRingCount(verticesPerRingCount),
radius(radius),

layerCount(verticesPerRingCount / 2 - 1),
displacementAngle(360.0 / verticesPerRingCount),

eboVerticesPerTriangle(3),
eboCapacity(2 * verticesPerRingCount * eboVerticesPerTriangle * layerCount),
eboElementsPerVertexRing(verticesPerRingCount * eboVerticesPerTriangle) 
{
    totalVerticesCount = verticesPerRingCount * layerCount;
    capacity = totalVerticesCount * stride;

    vboData = new float [capacity]{};
    eboData  = new unsigned int [capacity]{};

}

class_bufferObjectsInitHelper::~class_bufferObjectsInitHelper(){
    delete [] vboData;
    delete [] eboData;
}