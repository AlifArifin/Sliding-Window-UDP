#ifndef _ACK_H_
#define _ACK_H_

#include <iostream>
#include <string>

using namespace std;

typedef struct {
    unsigned char ACK;
    unsigned int nextSequenceNumber;
    unsigned char checksum;
} PacketACK;

#define DefaultACK 0x1
#define DefaultNAK 0x0

// function
PacketACK createACK(unsigned char ACK, unsigned int nextSequenceNumber);
unsigned char generateChecksumACK(PacketACK ACK);

#endif