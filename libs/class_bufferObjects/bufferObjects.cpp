#include "bufferObjects.h"

class_bufferObjects::class_bufferObjects(const unsigned int verticesPerRingCount, const float radius) :
stride(6),
verticesPerRingCount(verticesPerRingCount),
radius(radius),

layerCount(verticesPerRingCount / 2 - 1),
displacementAngle(360.0 / verticesPerRingCount),

eboVerticesPerTriangle(3),
eboCapacity((layerCount - 1) * verticesPerRingCount * 2 * eboVerticesPerTriangle + verticesPerRingCount * eboVerticesPerTriangle),
eboElementsPerVertexRing(verticesPerRingCount * eboVerticesPerTriangle) 
{
    totalVerticesCount = verticesPerRingCount * layerCount;
    capacity = totalVerticesCount * stride;

    vertices = new float [capacity]{};
    eboData  = new unsigned int [capacity]{};

}

class_bufferObjects::~class_bufferObjects(){
    delete [] vertices;
    delete [] eboData;
}