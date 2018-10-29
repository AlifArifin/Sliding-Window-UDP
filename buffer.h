#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <iostream>
#include "frame.h"

typedef struct {
    Frame frame;
    boolean valid;
} Buffer;

void initBuffer(Buffer* buffer);

typedef struct {
    Buffer* buffers;
    unsigned int size;
} Buffers;

Buffers initBuffers(unsigned int size);
boolean allValid(Buffers buffers);

#define frame(B) ((B).frame)
#define valid(B) ((B).valid)
#define buffers(B) ((B).buffers)
#define size(B) ((B).size)

#endif