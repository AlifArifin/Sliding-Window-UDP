#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>

#include <iostream>
using namespace std;

#define DATA_LENGTH_MAX 1024
#define ACK_NUM 6
#define NAK_NUM 33

struct Frame {
    int length;
    uint8_t Data[DATA_LENGTH_MAX];
    uint32_t sequenceNumber;
    Frame(){
        sequenceNumber = 0;
        length = 0;
    }
};

class FilesWriter {
    /* ATTRIBUTES */
    private:
        const uint32_t MAX_BUFFER_SIZE;
        uint8_t *buffer;
        uint32_t offset;
        FILE *writer;
        void Flush() {
            fwrite(buffer, sizeof(uint8_t), offset, writer);
            offset = 0;
        }
    
    /* METHOD */
    public:
        /* CONSTRUCTOR */
        FilesWriter(char *filesName, uint32_t bufferSize): MAX_BUFFER_SIZE(bufferSize) {
            writer = fopen(filesName, "wb+");
			buffer = (uint8_t *) malloc (MAX_BUFFER_SIZE);
            offset = 0;
        }
        
        /* DESTRUCTOR */
        ~FilesWriter() {
            Flush();
            fclose(writer);
        }
        
        /* PROCEDURE */
        void writeData(uint8_t *data, uint32_t length) {
            if (length == 0)
                return;
            else if (offset == MAX_BUFFER_SIZE)
                Flush();
                
            uint32_t FLength = min(MAX_BUFFER_SIZE - offset, length);
            memcpy(buffer + offset, data, FLength);
            offset += FLength;
            writeData(data + FLength, length - FLength);
        }
};

/* FUNCTION AND PROCEDURE */
void intToBytes(uint8_t *data, uint32_t value) {
    int i = 3;
    do {
        data[i] = value & 0xFFFF;
        value >>= 0x8;
        i--;
    } while (i >= 0);
}

uint32_t bytesToInt(uint8_t *data) {
    uint32_t Resource = 0;
    int i = 0;
    do {
        Resource <<= 0x8;
        Resource += data[i];
        ++i;
    } while (i <= 3);
    return Resource;
}

uint8_t generateChecksum(const uint8_t *data, uint32_t length) {
    uint32_t result, n = 0x0;
    do {
        result += data[n];
        ++n;
    } while (n < length);
    result += result >> 0x8;
    return ~((uint8_t)result);
}

bool validatePacket(const uint8_t *data, uint32_t len) {
    return ((data[0] == 0x1) and (generateChecksum(data, len - 1) == data[len - 1]));
}

void sendRespond(int header, int nextSequenceNumber, int sd, sockaddr_in &my_addr) {
    uint8_t Ack[6];
    Ack[0] = header;
    intToBytes(Ack + 1, nextSequenceNumber);
    Ack[5] = generateChecksum(Ack, 5);
    if (sendto(sd, Ack, 6, 0, (struct sockaddr *) &my_addr, sizeof my_addr) < 0)
        perror("Acknowledge sent error!");
}

/* MAIN PROGRAM */
int main(int args, char **argv) {
    char *filesName = argv[1];
    int sizeWindow = atoi(argv[2]);
    int sizeBuffer = atoi(argv[3]);
    char *port = argv[4];
    FilesWriter writer(filesName, sizeBuffer *DATA_LENGTH_MAX);
    
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Socket failed to initialize!");
        exit(1);
    }
    
    struct sockaddr_in my_addr;
    struct sockaddr_in rem_addr;
    
    socklen_t addrlen = sizeof rem_addr;
    memset((char*) &my_addr, 0, sizeof my_addr);
    my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.sin_port = htons(atoi(port));
    
    if (bind(sd, (struct sockaddr*) &my_addr, sizeof my_addr) < 0) {
        perror("Socket failed to bind!");
        exit(1);
    }    
    
    /* Timeout */
    struct timeval tv;
    tv.tv_sec = 2;
	tv.tv_usec = 0;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof tv); 

	/* Start receiving data */
	uint8_t packet[1035];
	Frame *data = (Frame *) malloc (sizeWindow * sizeof(Frame));
	uint32_t lfr = 0, laf = 10;
	while (true) {
		/* Waiting for packet */
		int recvlen = recvfrom(sd, packet, DATA_LENGTH_MAX + 10, 0, (struct sockaddr *) &my_addr, &addrlen);
		if ((recvlen > 0) and (recvlen <= DATA_LENGTH_MAX + 10)) {
			if (validatePacket(packet, recvlen)) {
				uint32_t sequenceNumber = bytesToInt(packet + 1);
				cout << "Valid packet received no : " << sequenceNumber << endl;
				cout << "Packet size : " << recvlen << endl;
                
				if ((sequenceNumber > lfr) and (sequenceNumber <= laf)) {
					int idx = sequenceNumber % sizeWindow;
					if (data[idx].sequenceNumber != sequenceNumber){
						data[idx].sequenceNumber = sequenceNumber;
						data[idx].length = bytesToInt(packet + 5);
						memcpy(data[idx].Data, packet + 9, data[idx].length);
						
                        /* Move Window */
						while (data[(lfr + 1) % sizeWindow].sequenceNumber == lfr + 1) {
							writer.writeData(data[idx].Data, data[idx].length);
							++lfr;
						}
                        
						laf = lfr + sizeWindow;
					}
				}
                
				/* Send ACK */
				sendRespond(ACK_NUM, lfr + 1, sd, my_addr);
				if (bytesToInt(packet + 5) == 0) {
					cout << "Packets delivered succesfully! Connection will be closed." << endl;
					break;
				}
			}
            
			/* Send NAK */
			else
				sendRespond(NAK_NUM, lfr + 1, sd, my_addr);
		}
		else 
			cout << "Waiting for packets ..." << endl;
	}
	return 0;
}
