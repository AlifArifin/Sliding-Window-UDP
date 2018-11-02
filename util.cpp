#include "util.h"
#include <iostream>
#include <fstream>
#include "buffer.h"
#include "frame.h"
#include <chrono>
#include <string.h>

void readFile(FILE* file, Buffer *B, unsigned int *sequenceNumber, unsigned int LAR) {
    // mengecek apakah terdapat frame yang sudah di-ACK
    if (!feof(file)) {
        // cout << "yes" << endl;
        for (int i = 0; i < B->size; i++) {
            // cout << "yes " << i << endl;
            // melakukan pembacaan apabila terdapat frame pada buffer yang seqnum-nya <= LAR
            if (B->buffer[i].sequenceNumber <= LAR) {
                // jika sudah end of file maka break
                // cout << "yes " << 'a' << endl;
                
                if (feof(file)) {
                    // cout << "yes " << 'd' << endl;
                    printBuffer(*B);
                    return;
                }

                // cout << "yes " << 'b' << endl;

                unsigned char data[MaxData];
                // char temp[1];
                int j;

                // cout << "yes " << 'c' << endl;

                j = fread(data, 1, MaxData, file);
                // while(fread(temp, 1, 1, file) == 1 && j < 1024) {
                //     // cout << j << endl;
                //     data[j] = (unsigned char) temp[0]; 
                //     j++;
                //     // cout << j << endl;
                // }
                cout << j << endl;

                unsigned char realData[j];
                memcpy(realData, data, sizeof(realData));

                // cout << "yes" << endl;

                Frame F;
                createFrame(&F, *sequenceNumber, j, realData);
                cout << "yes" << endl;
                printFrame(F);
                
                // cout << "yes" << endl;
                *sequenceNumber += 1;

                B->buffer[i] = F;
                printBuffer(*B);
            }
        }
    }
}

void printBuffer(Buffer B) {
    cout << "Buffer ";
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

Frame convertToFrame(unsigned char *dataFrame) {
    Frame F;
    int i;

    F.SOH = dataFrame[0];
    for (i = 1; i <= 4; i++) {
        F.sequenceNumber <<= 8;
        F.sequenceNumber |= dataFrame[i];
    }
    for (i = 5; i <= 8; i++) {
        F.dataLength <<= 8;
        F.dataLength |= dataFrame[i];
    }
    for (i = 9; i < F.dataLength + 9; i++) {
        F.data[i-9] = dataFrame[i];
    }
    F.checksum = dataFrame[9 + F.dataLength];

    return F;
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