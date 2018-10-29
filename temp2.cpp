#include <iostream>
#include <fstream>

using namespace std;

int main() {
    FILE* file = fopen("tes.txt", "r");

    fseek(file, 0, SEEK_END);
    unsigned int endFile = ftell(file);

    cout << endFile;
    return 0;
}