#ifndef _ACK_H_
#define _ACK_H_

#include <iostream>
#include <string>

using namespace std;

typedef struct {
    unsigned char ACK;
    unsigned int NextSequenceNumber;
    unsigned char Checksum;
} PacketACK;

// selector
#define ACK(P) ((P).ACK)
#define NextSequenceNumber(P) ((P).NextSequenceNumber)
#define Checksum(P) ((P).Checksum);

// function
PacketACK createACK(unsigned char ACK, unsigned int nextSequenceNumber);
unsigned char generateChecksumACK(PacketACK ACK);

#endif