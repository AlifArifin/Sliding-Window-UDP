#include <iostream>
#include "frame.h"
#include "ack.h"
#include "windowsender.h"
#include "buffer.h"
#include "util.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <fstream>
#include <chrono>
#include <mutex>
#include <thread>
#include <cmath>

using namespace std;
using namespace chrono;
using std::chrono::system_clock;

// global variable
struct sockaddr_in si_other;
socklen_t si_other_size;
Buffer buffer;
WindowSender windowSender;
FILE* file;
int udp;
unsigned int sequenceNumber;
unsigned int maxSequenceNumber;
mutex m;
bool done = false;
bool gotNAK = false;
bool final = false; // all frame already sent, sent a sentinel

void threadReceive() {
    PacketACK P;
    char* segment = (char*) &P;
    int sizeP = sizeof(P);
    int len;

    cout << "Receive thread active" << endl;
    // menunggu ack dari receiver
    while (!done) {
        len = recvfrom(udp, segment, sizeP, 0, (struct sockaddr*) &si_other, &si_other_size);

        unsigned int checksum = generateChecksumACK(P);

        if (checksum == P.checksum) {
            m.lock();
            // meng-update sliding window

            if (P.ACK == DefaultACK) {
                receiveACK(&windowSender, P.nextSequenceNumber);
                if (windowSender.LFS == maxSequenceNumber) {
                    if (windowSender.LFS == windowSender.LAR) {
                        done = true;
                    }
                }
                cout << "got ACK " << P.nextSequenceNumber << endl;
            } else {
                gotNAK = true;
                cout << "got NAK" << endl;
            }
            m.unlock();
        }
    }

    while (!final) {
        len = recvfrom(udp, segment, sizeP, 0, (struct sockaddr*) &si_other, &si_other_size);
        unsigned int checksum = generateChecksumACK(P);
        if (checksum == P.checksum) {
            final = true;
        }
    }
}

int main(int argc, char* argv[]) {
    char* filename;
    char* destinationip;
    int windowsize, buffersize, destinationport;

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
    // setsockopt(udp, SOL_SOCKET, SO_RCVTImO, &recvtimeout, sizeof recvtimeout)

    // membuat buffer yang menyimpan frame
    createBuffer(&buffer, buffersize);

    // receiver address
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(destinationport);
    si_other.sin_addr.s_addr = inet_addr(destinationip);

    // membuat socket
    udp = socket(AF_INET, SOCK_DGRAM, 0);

    // socket fail
    if (udp < 0) {
        cerr << "socket fail" << endl;;
        return 1;
    }

    cout << "Socket already built" << endl;

    // membuat window untuk sliding window
    createWindowSender(&windowSender, windowsize);

    // mengecek file dapat dibaca atau tidak
    file = fopen(filename, "rb");
    if (file == NULL) {
        cerr << "file not found" << endl;
        return 1;
    }

    fseek(file, 0, SEEK_END);
    unsigned int endFile = ftell(file);
    maxSequenceNumber = (unsigned int) ceil(endFile/1024.0);
    rewind(file);
    // unsigned int nowFile = 0;

    
    // inisiasi
    sequenceNumber = 1;
    int sizeSI = sizeof(si_other);

    // init thread
    thread recv_thread(threadReceive);

    cout << "Max Frame " << maxSequenceNumber << endl;

    // int count = 0;    
    printWindow(windowSender);
    printBuffer(buffer);
    
    while (!done) {
        m.lock();
        // count++;
        // if (count % 10000 == 0) {
        //     cout << "yes";
        // }
        if (gotNAK) {
            // send LAR + 1 frame;
            for (int i = 0; i < windowSender.SWS; i++) {
                if (windowSender.buffer[i].sequenceNumber == windowSender.LAR + 1) {
                    windowSender.buffer[i].timeout = high_resolution_clock::now() + milliseconds(TimeoutFrame);
                    
                    Frame frameSend = buffer.buffer[windowSender.buffer[i].frameNumber];
                    char* segment = (char*) &frameSend;
                    // send frame
                    sendto(udp, 
                        segment, sizeof(frameSend),
                        0, (struct sockaddr*) &si_other, sizeSI);

                    cout << "retransmit error frame " << windowSender.buffer[i].sequenceNumber << endl;
                }
            }

            gotNAK = false;
        }
        
        readFile(file, &buffer, &sequenceNumber, windowSender.LAR);
        
        unsigned int temp;
        temp = sendFrame(&windowSender, maxSequenceNumber);
        
        // check timeout
        high_resolution_clock::time_point now = high_resolution_clock::now();
        // cout << now.time_since_epoch().count() << endl;
        for (int i = 0; i < windowSender.SWS; i++) {
            if (!windowSender.buffer[i].ack && windowSender.buffer[i].sent) {
                if (windowSender.buffer[i].timeout < now) {
                    windowSender.buffer[i].timeout = high_resolution_clock::now() + milliseconds(TimeoutFrame);
                    
                    Frame frameSend = buffer.buffer[windowSender.buffer[i].frameNumber];
                    char* segment = (char*) &frameSend;

                    // send frame
                    sendto(udp, 
                        segment, sizeof(frameSend),
                        0, (struct sockaddr*) &si_other, sizeSI);

                    cout << "retransmit timeout frame " << windowSender.buffer[i].sequenceNumber << endl;
                }
            }
        }

        if (temp != 0) {
            // find next frameNum in windowSender
            int frameNum;
            
            frameNum = updateWindow(&windowSender, &buffer, temp);
            // add timeout
            
            windowSender.buffer[frameNum].timeout = high_resolution_clock::now() + milliseconds(TimeoutFrame);
            windowSender.buffer[frameNum].sent = true;
            Frame frameSend = buffer.buffer[windowSender.buffer[frameNum].frameNumber];
            char* segment = (char*) &frameSend;
            // send frame
            this_thread::sleep_for(microseconds(TimeoutFrame));
            sendto(udp, 
                segment, sizeof(frameSend),
                0, (struct sockaddr*) &si_other, sizeSI);

            cout << "transmit frame " << windowSender.buffer[frameNum].sequenceNumber << " " << windowSender.buffer[frameNum].timeout.time_since_epoch().count() << endl;
        }
        m.unlock();
    }

    bool sentinelSent = false;
    Frame sentinelFrame;
    high_resolution_clock::time_point timeout = high_resolution_clock::now() + milliseconds(TimeoutFrame);
    createFrame(&sentinelFrame, 0, 0, NULL);
    char* segment = (char*) &sentinelFrame;

    while (!final) {
        if (!sentinelSent) {
            // send frame
            this_thread::sleep_for(microseconds(TimeoutFrame));
            sendto(udp, 
                segment, sizeof(sentinelFrame),
                0, (struct sockaddr*) &si_other, sizeSI);
            sentinelSent = true;
        } else {
            auto now = high_resolution_clock::now();
            if (now > timeout) {
                this_thread::sleep_for(microseconds(TimeoutFrame));
                sendto(udp, 
                    segment, sizeof(sentinelFrame),
                    0, (struct sockaddr*) &si_other, sizeSI);
            }
        }
    }

    recv_thread.detach();
    fclose(file);

    return 0;
}