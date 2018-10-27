#include <iostream>
#include "util.h"

using namespace std;

int main() {
    unsigned int sum;
    unsigned char result;
    unsigned int temp;

    sum = 0x00;
    sum += 0x09;
    temp = 0xFA000000;
    while (temp) {
        sum += temp & 0xFF;
        sum = handleCarry(sum);
        temp = temp >> 8;
        cout << "sum is " << hex << (int) sum << endl;
    }
    result = ~(sum);
    cout << "result is " << hex << (int) result << endl;
    if (result == 0x03) {
        cout<< "yes";
    }

    unsigned char* a;
    a[0] = 0xAA;
    a[1] = 0XFF;
    a[2] = 0x00;

    unsigned char* b;
    b = a;

    return 0;   
}