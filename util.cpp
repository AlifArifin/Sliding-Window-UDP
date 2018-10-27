#include "util.h"
#include <iostream>

using namespace std;

unsigned int handleCarry(unsigned int sum) {
    while (sum & 0xFFFFFF00) {
        // ada carry
        sum &= 0xFF;
        sum++;
    }

    return sum;
}