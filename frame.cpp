#include "frame.h"
#include "util.h"
#include <iostream>

using namespace std;

unsigned char generateChecksumFrame(Frame F) {
    unsigned int sum;
    unsigned int temp;

    sum = 0x00;
    sum += SOH(F);
    temp = SequenceNumber(F);
    while (temp) {
        sum += temp & 0xFF;
        sum = handleCarry(sum);
        temp = temp >> 8;
    }
    temp = DataLength(F);
    while (temp) {
        sum += temp & 0xFF;
        sum = handleCarry(sum);
        temp = temp >> 8;
    }

    for (unsigned int i = 0; i < DataLength(F); i++) {
        sum += Data(F)[i];  
        sum = handleCarry(sum);
    }

    return ~(sum);
}

Frame createFrame(unsigned int sequenceNumber, unsigned int dataLength, unsigned char* data) {
    Frame F;

    SOH(F) = DefaultSOH;
    SequenceNumber(F) = sequenceNumber;
    DataLength(F) = dataLength;
    for (unsigned int i = 0; i < DataLength(F); i++) {
        Data(F)[i] = data[i];
    }
    Checksum(F) = generateChecksumFrame(F);

    return F; 
}