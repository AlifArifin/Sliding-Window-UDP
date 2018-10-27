#include "ack.h"
#include "util.h"
#include <iostream>

using namespace std;

unsigned char generateChecksumACK(PacketACK ACK) {
    unsigned int sum;
    unsigned int temp;

    sum = 0x00;
    sum += ACK(ACK);
    temp = NextSequenceNumber(ACK);
    while (temp) {
        sum += temp & 0xFF;
        sum = handleCarry(sum);
        temp = temp >> 8;
    }
    return ~(sum);
}

PacketACK createACK(unsigned char ACK, unsigned int nextSequenceNumber) {
    PacketACK P;

    ACK(P) = ACK;
    NextSequenceNumber(P) = nextSequenceNumber;
    Checksum(P) = generateChecksumACK(P);

    return P;
}