#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <iostream>
#include "frame.h"

typedef struct {
    Frame* buffer;
    unsigned int size;
} Buffer;

typedef struct {
    int frameNumber;
    unsigned int sequenceNumber;
    int timeout;
} WindowBuffer;

Buffer createBuffer(unsigned int size);
WindowBuffer createWindowBuffer();

#endif