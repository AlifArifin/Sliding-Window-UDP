#include <iostream>
#include "frame.h"
#include "ack.h"
#include "windowsender.h"
#include "buffer.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <fstream>

using namespace std;

// global variable
struct sockaddr_in si_other;
Buffers buffers;
WindowSender windowSender;
FILE* file;
int udp;
unsigned int sequenceNumber;

void die(char *s) {
    perror(s);
    exit(1);
}

int main(argc, char* argv) {
    string filename, destinationip;
    unsigned int windowsize, buffersize, destinationport;

    if (argc == 6) {
        filename = argv[1];
        windowsize = atoi(argv[2]);
        buffersize = atoi(argv[3]);
        destinationip = argv[4];
        destinationport = atoi(argv[5]);
    } else {
        cout << "need parameter" << endl;
        cout << "./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>" << endl;
        return 1;
    }

    // socket receive timeout
    // struct timeval recvtimeout;
    // recvtimeout.tv_sec = 0;
    // recvtimeout.tv_usec = 10000;
    // setsockopt(udp, SOL_SOCKET, SO_RCVTIMEO, &recvtimeout, sizeof recvtimeout)

    // membuat buffer yang menyimpan frame
    buffers = initBuffers(buffersize);

    // receiver address
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(destinationport);
    inet_aton(destinationip, si_other.sin_addr);

    // membuat socket
    udp = socket(AF_INET, SOCK_DGRAM, 0)

    // socket fail
    if (udp < 0) {
        die("socket");
    }

    cout << "Socket already built" << endl;

    // membuat window untuk sliding window
    windowSender = createNew(windowsize);

    // mengecek file dapat dibaca atau tidak
    file = fopen(filename, "rb");
    if (file == NULL) {
        die("file not found");
    }

    // fseek(file, 0, SEEK_END);
    // unsigned int endFile = ftell(file);
    // rewind(file);
    // unsigned int nowFile = 0;

    // inisiasi
    sequenceNumber = 0;

    while(1) {

    }
}

void receiveACK() {
    PacketACK P;

    // menunggu ack dari receiver
    while (true) {
        int size = sizeof(P);
        int len;
        len = recvfrom(udp, P, size, 0, (struct sockaddr*) &si_other, sizeof(si_other));

        unsigned int checksum = generateChecksumACK(P);

        if (checksum == checksum(P)) {
            // meng-update sliding window
            receiveACK(&windowSender, NextSequenceNumber(P));

            // membaca file untuk mengisi buffer

        }
    }
}