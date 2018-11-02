#include "frame.h"
#include <iostream>

using namespace std;

unsigned char generateChecksumFrame(Frame F) {
    unsigned int sum;
    unsigned int temp;

    // inisiasi nilai sum
    sum = 0x00;
    sum += F.SOH;
    temp = F.sequenceNumber;
    // memecah menjadi 1 byte
    while (temp) {
        sum += temp & 0xFF;
        sum = handleCarry(sum);
        temp = temp >> 8;
    }
    temp = F.dataLength;
    while (temp) {
        sum += temp & 0xFF;
        sum = handleCarry(sum);
        temp = temp >> 8;
    }

    for (unsigned int i = 0; i < F.dataLength; i++) {
        sum += F.data[i];  
        sum = handleCarry(sum);
    }

    return ~(sum);
}

void createFrame(Frame *F, unsigned int sequenceNumber, unsigned int dataLength, unsigned char* data) {
    F->SOH = DefaultSOH;
    F->sequenceNumber = sequenceNumber;
    F->dataLength = dataLength;
    F->data = new unsigned char[dataLength];

    for (int i = 0; i < F->dataLength; i++) {
        F->data[i] = data[i];
    }
    F->checksum = generateChecksumFrame(*F);
}

void createFrameDefault(Frame *F) {
    F->SOH = DefaultSOH;
    F->sequenceNumber = 0;
    F->dataLength = 0;
    F->checksum = generateChecksumFrame(*F);
}

unsigned int handleCarry(unsigned int sum) {
    while (sum & 0xFFFFFF00) {
        // ada carry
        sum &= 0xFF;
        sum++;
    }

    return sum;
}

void printFrame(Frame F) {
    cout << "Frame " << F.sequenceNumber << " " << F.dataLength << endl;
}