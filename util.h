#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <string>
#include "buffer.h"
#include "windowsender.h"

using namespace std;

unsigned int handleCarry(unsigned int sum);
void readFile(FILE* file, WindowSender *W, Buffer *B, unsigned int *sequenceNumber, unsigned int LAR);

#endif