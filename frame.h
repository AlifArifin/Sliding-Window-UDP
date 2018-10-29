#ifndef _FRAME_H_
#define _FRAME_H_

#include <iostream>
#include <string>

using namespace std;

typedef struct {
    unsigned char SOH;
    unsigned int SequenceNumber;
    unsigned int DataLength;
    unsigned char* Data;
    unsigned char Checksum;
} Frame;

// definition
#define DefaultSOH 0x1;
#define MaxData 1024;

// selector
#define SOH(S) ((S).SOH)
#define SequenceNumber(S) ((S).SequenceNumber)
#define DataLength(S) ((S).DataLength)
#define Data(S) ((S).Data)
#define Checksum(S) ((S).Checksum)

// function
Frame createFrame(unsigned int sequenceNumber, unsigned int dataLength, unsigned char* data);
unsigned char generateChecksumFrame(Frame F);

#endif
