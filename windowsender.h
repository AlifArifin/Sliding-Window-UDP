#ifndef _WINDOWSENDER_H_
#define _WINDOWSENDER_H_

#include <iostream>

using namespace std;

typedef struct {
    unsigned int SWS;
    unsigned int LAR;
    unsigned int LFS;
} WindowSender;

#define SWS(S) ((S).SWS)
#define LAR(S) ((S).LAR)
#define LFS(S) ((S).LFS)

WindowSender createNew(unsigned int sws);
void receiveACK(WindowSender *S, unsigned int nextSequenceNumber);
void sendFrame(WindowSender *S, unsigned int sequenceNumber);

#endif