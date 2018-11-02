#ifndef _WINDOWSENDER_H_
#define _WINDOWSENDER_H_

#include <iostream>
#include "buffer.h"

using namespace std;

typedef struct {
    unsigned int SWS;
    unsigned int LAR;
    unsigned int LFS;
    WindowBuffer* buffer;
} WindowSender;

WindowSender createNew(unsigned int sws);
void receiveACK(WindowSender *S, unsigned int nextSequenceNumber);
unsigned int sendFrame(WindowSender *S, unsigned int maxSequenceNumber);
int updateWindow(WindowSender *S, Buffer *B, unsigned int sequenceNumber);

#endif