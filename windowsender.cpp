#include "windowsender.h"
#include <iostream>

using namespace std;

void createWindowSender(WindowSender *S, unsigned int sws) {
    S->SWS = sws;

    S->buffer = new WindowBuffer[S->SWS];
    S->LAR = 0;
    S->LFS = 0;

    for (int i = 0; i < S->SWS; i++) {
        createWindowBuffer(&S->buffer[i]);
    }
}

void receiveACK(WindowSender *S, unsigned int nextSequenceNumber) {
    for (int i = 0; i < S->SWS; i++) {
        if (S->buffer[i].sequenceNumber == nextSequenceNumber - 1) {
            S->buffer[i].ack = true;
            S->buffer[i].sent = false;
        }
    }
    cout << "rcv test" << endl;
    cout << "LAR" << S->LAR << endl;

    while (true) {
        for (int i = 0; i < S->SWS; i++) {
            if (S->LAR == S->LFS) {
                return;
            }
            if (S->buffer[i].sequenceNumber == S->LAR + 1) {
                if (S->buffer[i].ack) {
                    S->LAR++;
                } else {
                    return;
                }
            }
        }
    }
}

unsigned int sendFrame(WindowSender *S, unsigned int maxSequenceNumber) {
    if (S->LFS == maxSequenceNumber) {
        return 0;
    } else if (S->LFS - S->LAR < S->SWS) {
        S->LFS++;
        return S->LFS;
    } else {
        return 0;
    }
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
    S->buffer[min].ack = false;
    S->buffer[min].timeout = high_resolution_clock::now();

    return min;
}

void printWindow(WindowSender W) {
    cout << "Window ";
    for (int i = 0; i < W.SWS; i++) {
        cout << W.buffer[i].sequenceNumber << " ";
    }
    cout << endl;
}