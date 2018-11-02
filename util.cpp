#include "util.h"
#include <iostream>
#include <fstream>
#include "buffer.h"
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

                j = fread(data, 1024, 1024, file);
                // while(fread(temp, 1, 1, file) == 1 && j < 1024) {
                //     // cout << j << endl;
                //     data[j] = (unsigned char) temp[0]; 
                //     j++;
                //     // cout << j << endl;
                // }

                unsigned char realData[j];
                memcpy(realData, data, sizeof(realData));

                // cout << "yes" << endl;

                Frame F;
                createFrame(&F, *sequenceNumber, j, realData);
                
                // cout << "yes" << endl;
                sequenceNumber++;

                B->buffer[i] = F;
            }
        }
    }
}

void printBuffer(Buffer B) {
    for (int i = 0; i < B.size; i++) {
        // cout << B.buffer[i].sequenceNumber << " ";
    }
    // cout << endl;
}