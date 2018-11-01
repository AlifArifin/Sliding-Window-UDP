#include "windowsender.h"
#include <iostream>

using namespace std;

WindowSender createNew(unsigned int sws) {
    WindowSender S;
    S.SWS = sws;

    WindowBuffer WB[S.SWS];
    S.LAR = NULL;
    S.LFS = NULL;

    for (int i = 0; i < S.SWS; i++) {
        WB[i] = createWindowBuffer();
    }

    S.buffer = WB;

    return S;
}

void receiveACK(WindowSender *S, unsigned int nextSequenceNumber) {
    S->LAR = nextSequenceNumber - 1;
}

unsigned int sendFrame(WindowSender *S) {
    if (S->LAR == NULL) {
        if (S->LFS == NULL) {
            S->LFS = 0;
        } else if (S->LFS < S->SWS) {
            S->LFS++;
        } else {
            return NULL;
        }
    } else {
        if (S->LFS - S->LAR < S->SWS) {
            S->LFS++;
        } else {
            return NULL;
        }
    }

    return S->LFS;
}

int updateWindow(WindowSender *S, Buffer *B, unsigned int sequenceNumber) {
    // find minimum sequenceNumber in window
    // karena pasti ada tempat baru
    int min = -1;
    int idx; 
    for (int i = 0; i < S->SWS; i++) {
        if (S->buffer[i].frameNumber == -1) {
            min = i;
            break;
        } else {
            min = S->buffer[i].sequenceNumber < min ? S->buffer[i].sequenceNumber : min;
        }
    }

    for (int i = 0; i < B->size; i++) {
        if (B->buffer[i].sequenceNumber == sequenceNumber) {
            idx = i;
            break;
        }
    }

    S->buffer[min].frameNumber = idx;
    S->buffer[min].sequenceNumber = sequenceNumber;

    return min;
}