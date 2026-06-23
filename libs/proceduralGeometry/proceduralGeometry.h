#ifndef PROCEDURAL_SPHERE
#define PROCEDURAL_SPHERE

#include "class_bufferObjectsInitHelper/bufferObjectsInitHelper.h"

#include "glad/glad.h"

void vertexRingGenerator(class_bufferObjectsInitHelper &bufferObjects);
void primaryVertexInit(class_bufferObjectsInitHelper &bufferObjects);
void elementBufferGenerator(class_bufferObjectsInitHelper &bufferObjects);

GLuint initBorderBuffer(const double borderDisplacement);

#endif
