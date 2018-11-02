#include "util.h"
#include <iostream>
#include <fstream>
#include "buffer.h"
#include "frame.h"
#include <chrono>
#include <string.h>
#include "ack.h"
#include <stdio.h>

void convertToAck (PacketACK *temp, unsigned char * dataAck) {
    temp->ACK = dataAck[0];
    unsigned int seqNumber = 0;
    for (int i=1;i<=4;i++) {
        seqNumber <<= 8;
        seqNumber |= dataAck[i];
    }
    temp->nextSequenceNumber = seqNumber;
    temp->checksum = dataAck[5];
}

void readFile(FILE* file, Buffer *B, unsigned int *sequenceNumber, unsigned int LAR) {
    // mengecek apakah terdapat frame yang sudah di-ACK
    if (!feof(file)) {
        for (int i = 0; i < B->size; i++) {
            // melakukan pembacaan apabila terdapat frame pada buffer yang seqnum-nya <= LAR
            if (B->buffer[i].sequenceNumber <= LAR) {
                // jika sudah end of file maka break
                
                if (feof(file)) {
                    printBuffer(*B);
                    return;
                }

                unsigned char data[MaxData];
                // char temp[1];
                int j;

                j = fread(data, 1, MaxData, file);

                unsigned char realData[j];
                memcpy(realData, data, sizeof(realData));

                Frame F;
                createFrame(&F, *sequenceNumber, j, realData);
                printFrame(F);
                
                *sequenceNumber += 1;

                B->buffer[i] = F;
                printBuffer(*B);
            }
        }
    }
}

void printBuffer(Buffer B) {
    cout << "== BUFFER ==";
    for (int i = 0; i < B.size; i++) {
        cout << B.buffer[i].sequenceNumber << " ";
    }
    cout << endl;
}

unsigned char* convertToChar(Frame F) {
    int i;
    int length = F.dataLength + 10;
    unsigned char* dataFrame = new unsigned char[length];

    dataFrame[0] = DefaultSOH;
    for (i = 1; i <= 4; i++) {
        dataFrame[i] = F.sequenceNumber >> (8 * (4 - i));
    }
    for (i = 5; i <= 8; i++) {
        dataFrame[i] = F.dataLength >> (8 * (8 - i));
    }
    for (i = 9; i < F.dataLength + 9; i++) {
        dataFrame[i] = F.data[i-9];
    }
    dataFrame[9 + F.dataLength] = F.checksum;
    
    return dataFrame;
}

void convertToFrame(Frame *F, unsigned char *dataFrame) {
    int i;

    F->SOH = dataFrame[0];
    for (i = 1; i <= 4; i++) {
        F->sequenceNumber <<= 8;
        F->sequenceNumber |= dataFrame[i];
    }
    for (i = 5; i <= 8; i++) {
        F->dataLength <<= 8;
        F->dataLength |= dataFrame[i];
    }
    F->data = new unsigned char[F->dataLength];
    for (i = 9; i < F->dataLength + 9; i++) {
        F->data[i-9] = dataFrame[i];
    }
    
    for (i = 0; i < 3; i++) {
        printf("%x ", dataFrame[9+1020+i]);
    }
    F->checksum = dataFrame[9 + F->dataLength];
} 

unsigned char* convertToAckFrame(PacketACK inputAck){
    unsigned char* dataACK = new unsigned char[6];
    dataACK[0] = inputAck.ACK;
    for (int i = 1; i <= 4; i++) {
        dataACK[i] = inputAck.nextSequenceNumber >> (8 * (4 - i));
    }
    dataACK[5] = inputAck.checksum;
    return dataACK;
}