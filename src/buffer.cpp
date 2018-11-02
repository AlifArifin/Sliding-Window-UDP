#include "buffer.h"

using namespace std;

void createBuffer(Buffer *B, int size) {
    B->buffer = new Frame[size];
    
    for (int i = 0; i < size; i++) {
        createFrameDefault(&B->buffer[i]);
    }

    B->size = size;
}

void createWindowBuffer(WindowBuffer *W) {
    W->frameNumber = -1;
    W->sequenceNumber = 0;
    W->timeout = high_resolution_clock::now();
    W->ack = false;
    W->sent = false;
}