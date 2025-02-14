#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>	
#include "ack.h"
#include "frame.h"
#include <fstream>
#include "util.h"
#include <chrono>
#include <stdio.h>

using namespace std;
using namespace chrono;
using std::chrono::system_clock;

void readPacket (char* packet, Frame * F, bool* packetValid, bool* endOfTransfer) {
    convertToFrame(&(*F), (unsigned char *) packet);
    // memcpy(&F->sequenceNumber, packet+1, 4);
    // memcpy(&F->dataLength, packet+5, 4);

    unsigned char packetChecksum = packet[9 + F->dataLength];
    unsigned char checksum = generateChecksumFrame(*F);
    // for (int i = 0; i < 3; i++) {
    //     printf("%x ", (unsigned char*) packet[1020 + 9 + i]);
    // }
    *packetValid = (packetChecksum == checksum);

    *endOfTransfer = (F->sequenceNumber == 0);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in myaddr; /* our address*/
    struct sockaddr_in remaddr; /* remote address*/
    socklen_t addrlen = sizeof(remaddr);
    int recvlen; /* Number bytes receiver*/
    int fd;     /* Our socket */

    /* Sliding Window Variable*/
    FILE *file;
    bool done = false;
    int lfr; /* Last frame received*/
    int laf; /* Largest acceptable frame */

    char* buffer; /*receive buffer*/
    int bufferSize; /*buffer size*/
    unsigned int windowSize, maxBufferSize, port;
    char* filename;
    char* destinationip;

    /* packet buffer variable */
    char packet[1034];
    char data[1024];
    unsigned char ack[6];
    size_t datalen;
    unsigned int seq_num;
    bool packetValid;
    bool endOfTransfer;
    PacketACK ackdata;
    int buffer_offset;


    /* Read argument */
    filename = argv[1];
    windowSize = atoi(argv[2]);
    maxBufferSize = (unsigned int) 1024 * atoi(argv[3]);
    port = atoi(argv[4]);

    /* create UDP socket */
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

    cout << "Socket created. Descriptor: " << fd << endl;

    /* bind it */
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        cerr << "bind error" << endl;
		return 0;
	}

    /* receiving data */
    file = fopen(filename, "wb");
    lfr = 0;
    laf = lfr + windowSize;
    while (!done) {
        buffer = new char[maxBufferSize];
        bufferSize = 0;

        int seqCount = maxBufferSize/1024;
        bool isPacketReceived[windowSize];
        
        /*Initialing packet received with false*/
        for (int i = 0; i < windowSize; i++) {
            isPacketReceived[i] = false;
        }

        while (true) {
            /* receiving message */
            recvlen = recvfrom(fd, packet, 1034, MSG_WAITALL, (struct sockaddr*)&remaddr, &addrlen);
            if (recvlen < 0) {
                cout << "Error receiving\n";
                exit(1); 
            }

            /* read packet and safe to buffer */
            Frame F;
            readPacket(packet, &F, &packetValid, &endOfTransfer);
            seq_num = F.sequenceNumber;
            datalen = F.dataLength;
            memcpy(data, F.data, datalen);

            //readPacket(packet, &seq_num, &datalen, data, &packetValid, &endOfTransfer);

            if (seq_num <= laf) {
                if (packetValid) {
                    ackdata = createACK(DefaultACK, seq_num + 1);
                } else {
                    ackdata = createACK(DefaultNAK, seq_num);
                }

                /* create ack buffer */
                memcpy(ack, convertToAckFrame(ackdata), 6);
                
                //Send ACK
                int acklen = sendto(fd, ack, 6, MSG_WAITALL, (struct sockaddr*)&remaddr, addrlen);
                if (acklen < 0) {
                    cout << "Fail send ACK\n";
                }
                
                if (packetValid) {
                    // if (seq_num != 0) {
                    //     buffer_offset = (seq_num - 1) * 1024;
                    // }

                    if (seq_num == lfr + 1) {
                        /* copy buffer */
                        memcpy(buffer, data, datalen);
                        bufferSize += datalen;
                        unsigned int slide = 1;
                        for (unsigned int i = 1; i < windowSize; i++) {
                            if (!isPacketReceived[i]) {
                                break;
                            }
                            slide++;
                        }

                        /* SLIDING WINDOW */
                        cout << "== SLIDE ==" << endl;
                        for (unsigned int i = 0; i < windowSize - slide; i++) {
                            isPacketReceived[i] = isPacketReceived[i + slide];
                        }
                        for (unsigned int i = windowSize - slide; i < windowSize; i++) {
                            isPacketReceived[i] = false;
                        }
                        fwrite(buffer, 1, bufferSize, file);
                        bufferSize = 0;
                        lfr = lfr + slide;
                        laf = lfr + windowSize;
                    } else if (seq_num > lfr + 1) {
                        /* copy to buffer */
                        if (!isPacketReceived[seq_num - lfr + 1]) {
                            buffer_offset = (seq_num - lfr - 1) * 1024;
                            memcpy(buffer + buffer_offset, data, datalen);
                            isPacketReceived[seq_num - lfr - 1] = true;
                            bufferSize += datalen;
                        }
                    }

                    if (endOfTransfer) {
                        // bufferSize = buffer_offset + datalen;
                        if (bufferSize != 0) {
                            fwrite(buffer, 1, bufferSize, file);
                        }
                        seqCount = seq_num + 1;
                        done = true;
                        cout << "== RECEIVE PACKET ==" << endl;
                        cout << "Receive last packet " << seq_num << endl;
                        cout << "Send last ACK " << seq_num << endl;
                        
                    } else {
                        cout << "== RECEIVE PACKET ==" << endl;
                        cout << "Receiving packet " << seq_num << endl;
                        cout << "Sending ack " << seq_num << endl;
                    }
                } else {
                    cout << "== PACKET ERROR ==" << endl;
                    cout << "Packet error " << seq_num << endl;
                    cout << "Sending NAK " << seq_num << endl;
                }
            } else {
                cout << seq_num << endl;
                cout << lfr << endl;
                cout << laf << endl;
                cout << "SeqNum not in range" << endl;
            }

            if (lfr >= seqCount - 1) {
                break;
                /* SWP done*/
            }
        }
    }
    fclose(file);
    return 0;
}
