#ifndef PROGRAM_START_MENU
#define PROGRAM_START_MENU

#include "class_particleInitHelper/particleInitHelper.h"

size_t programInit(class_particleInitHelper*& particleInitHelper);
size_t defaultInit(class_particleInitHelper*& particleInitHelper);

void rgbInput(glm::vec3& color);
#endif