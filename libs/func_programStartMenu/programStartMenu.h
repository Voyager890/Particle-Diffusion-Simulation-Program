#ifndef PROGRAM_START_MENU
#define PROGRAM_START_MENU

#include "class_particleInitHelper/particleInitHelper.h"
#include <cstddef>

size_t programInit(class_particleInitHelper*& particleInitHelper);
size_t defaultInit(class_particleInitHelper*& particleInitHelper);
size_t fileInit(class_particleInitHelper*& particleInitHelper);


std::string extractString(std::ifstream& file);
void rgbInput(glm::vec3& color);

bool areOptionsValid(const class_particleInitHelper*& particleInitHelper, const size_t count_particleTypes);
#endif
