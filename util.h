#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <string>
#include "buffer.h"
#include "windowsender.h"
#include <chrono>
#include "frame.h"
#include "ack.h"

using namespace std;
using namespace chrono;
using std::chrono::system_clock;

PacketACK convertToAck (unsigned char * dataAck);
unsigned int handleCarry(unsigned int sum);
void readFile(FILE* file, Buffer *B, unsigned int *sequenceNumber, unsigned int LAR);
void printBuffer(Buffer B);
unsigned char* convertToChar(Frame F);
Frame convertToFrame(unsigned char *dataFrame);
unsigned char* convertToAckFrame(PacketACK inputAck);

#endif