#ifndef READOBJ_H_
#define READOBJ_H_
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "vectorfunctions.h"

void parseFile(string OBJfile_name_);

void createNormals();

unsigned char * flipImage(unsigned char * image, int width, int height);

#endif