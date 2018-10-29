#include "windowsender.h"
#include <iostream>

using namespace std;

WindowSender createNew(unsigned int sws) {
    WindowSender S;

    SWS(S) = sws;
    LAR(S) = NULL;
    LFS(S) = NULL:

    return S;
}

void receiveACK(WindowSender *S, unsigned int nextSequenceNumber) {
    LAR(*S) = 
}

void sendFrame(WindowSender *S, unsigned int sequenceNumber) {
    
}