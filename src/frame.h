#ifndef _FRAME_H_
#define _FRAME_H_

#include <iostream>
#include <string>

using namespace std;

typedef struct {
    unsigned char SOH;
    unsigned int sequenceNumber;
    unsigned int dataLength;
    unsigned char* data;
    unsigned char checksum;
} Frame;

// definition
#define DefaultSOH 0x1
#define MaxData 1024
#define TimeoutFrame 100

// function
void createFrame(Frame *F, unsigned int sequenceNumber, unsigned int dataLength, unsigned char* data);
unsigned char generateChecksumFrame(Frame F);
unsigned int handleCarry(unsigned int sum);
void createFrameDefault(Frame *F);
void printFrame(Frame F);

#endif
