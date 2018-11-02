#include "ack.h"
#include "frame.h"
#include <iostream>

using namespace std;

unsigned char generateChecksumACK(PacketACK P) {
    unsigned int sum;
    unsigned int temp;

    sum = 0x00;
    sum += P.ACK;
    temp = P.nextSequenceNumber;
    while (temp) {
        sum += temp & 0xFF;
        sum = handleCarry(sum);
        temp = temp >> 8;
    }
    return ~(sum);
}

PacketACK createACK(unsigned char ACK, unsigned int nextSequenceNumber) {
    PacketACK P;

    P.ACK = ACK;
    P.nextSequenceNumber = nextSequenceNumber;
    P.checksum = generateChecksumACK(P);

    return P;
}