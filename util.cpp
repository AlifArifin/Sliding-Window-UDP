#include "util.h"
#include <iostream>
#include <fstream>
#include "buffer.h"
#include <chrono>

void readFile(FILE* file, Buffer *B, unsigned int *sequenceNumber, unsigned int LAR) {
    // mengecek apakah terdapat frame yang sudah di-ACK
    if (!feof(file)) {
        for (int i = 0; i < B->size; i++) {
            // melakukan pembacaan apabila terdapat frame pada buffer yang seqnum-nya <= LAR
            if (B->buffer[i].sequenceNumber <= LAR) {
                // jika sudah end of file maka break
                if (feof(file)) {
                    break;
                }

                unsigned char* data;
                char temp[1];
                int j = 0;

                while(fread(temp, 1, 1, file) == 1 && j < 1024) {
                    data[j] = (unsigned char) temp[0]; 
                    j++;
                }

                Frame F;
                F = createFrame(*sequenceNumber, j, data);
                
                sequenceNumber++;

                B->buffer[i] = F;
            }
        }
    }
}