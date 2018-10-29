#include <iostream>
#include "frame.h"
#include "ack.h"
#include "windowsender.h"
#include "buffer.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <fstream>

using namespace std;

void die(char *s) {
    perror(s);
    exit(1);
}

int main(argc, char* argv) {
    string filename, destinationip;
    unsigned int windowsize, buffersize, destinationport;

    filename = argv[1];
    windowsize = atoi(argv[2]);
    buffersize = atoi(argb[3]);
    destinationip = argv[4];
    destinationport = atoi(argv[5]);

    int udp;
    struct sockaddr_in si_other;

    Buffers buffers;
    buffers = initBuffers(buffersize);

    if (udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) == -1) {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(destinationport);

    if (inet_aton(destinationip, si_other.sin_addr) == 0) {
        cerr << "inet_aton() failed";
        exit(1);
    }

    WindowSender windowSender;
    windowSender = createNew(windowsize);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        die("file not found");
    }

    fseek(file, 0, SEEK_END);
    unsigned int endFile = ftell(file);
    rewind(file);
    unsigned int nowFile = 0;

    unsigned int slen = sizeof(si_other);

    while(1) {
        if (nowFile < endFile) {

        }
    }
}