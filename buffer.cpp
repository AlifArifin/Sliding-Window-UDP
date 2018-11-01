#include "buffer.h"

using namespace std;

Buffer createBuffer(unsigned int size) {
    Buffer B;
    Frame buffer[size];
    
    B.buffer = buffer;
    B.size = size;
    
    return B;
}

WindowBuffer createWindowBuffer() {
    WindowBuffer W;

    W.frameNumber = -1;
    W.timeout = -1;

    return W;
}