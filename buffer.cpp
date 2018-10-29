#include "buffer.h"

using namespace std;

void initBuffer(Buffer* buffer) {
    buffer->valid = false;
}

Buffers initBuffers(unsigned int size) {
    Buffers B;
    buffers buffer[size];
    
    for (unsigned int i = 0; i < size; i++) {
        initBuffer(&buffer[i]);
    }

    buffers(B) = buffer[size];
    size(B) = size;

    return B;
}

boolean allValid(Buffers buffers) {
    for (unsigned int i = 0; i < size(buffers); i++) {
        if (!valid(buffer(buffers)[i])) {
            return false;
        }
    }
    return true;    
}
