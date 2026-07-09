#ifndef DEBUG_TOOLS
#define DEBUG_TOOLS

#include "class_bufferObjectsInitHelper/bufferObjectsInitHelper.h"
#include "glm/vec3.hpp"
void debug_vboDataDisplayer(class_bufferObjectsInitHelper &bufferObjects);
void debug_eboDataDisplayer(class_bufferObjectsInitHelper &bufferObjects);

void debug_displayVec3(const glm::vec3 vector, std::string name);
#endif
