#ifndef PROGRAM_START_MENU
#define PROGRAM_START_MENU

#include "class_particleInitHelper/particleInitHelper.h"

void programInit(class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea);


void defaultInit(class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea);
void manualInit (class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea);
void fileInit   (class_particleInitHelper*& particleInitHelper, int& count_particleTypes, float& borderArea);

double doubleUserInput();

std::string extractString(std::ifstream& file, int& currentElement, int& errorFlag);
void rgbInput(glm::vec3& color);

bool areOptionsValid(class_particleInitHelper* particleInitHelper, const int count_particleTypes, const int borderArea);
#endif
