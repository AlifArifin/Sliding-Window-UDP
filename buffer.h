#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <iostream>
#include "frame.h"
#include <chrono>

using namespace std;
using namespace chrono;
using std::chrono::system_clock;

typedef struct {
    Frame* buffer;
    unsigned int size;
} Buffer;

typedef struct {
    int frameNumber;
    unsigned int sequenceNumber;
    high_resolution_clock::time_point timeout;
    bool ack;
} WindowBuffer;

Buffer createBuffer(int size);
WindowBuffer createWindowBuffer();

#endif