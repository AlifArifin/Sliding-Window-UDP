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

// function
PacketACK createACK(unsigned char ACK, unsigned int nextSequenceNumber);
unsigned char generateChecksumACK(PacketACK ACK);

#endif