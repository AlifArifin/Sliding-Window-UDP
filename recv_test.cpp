#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>	/* needed for sockaddr_in */

using namespace std;

int main(int argc, char *argv[]) {
    /* Socket Variable*/
    struct sockaddr_in myaddr; /* our address*/
    struct sockaddr_in remaddr; /* remote address*/
    socklen_t addrlen = sizeof(remaddr);
    int recvlen; /* Number bytes receiver*/
    int fd;     /* Our socket */

    /* Sliding Window Variable*/
    FILE *file;

    char buffer[1024]; /*receive buffer*/
    unsigned int windowsize, maxbuffersize, port;
    char* filename;
    char* destinationip;

    /* Read argument */
    filename = argv[1];
    windowsize = atoi(argv[2]);
    maxbuffersize = (unsigned int) 1024 * atoi(argv[3]);
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

    while (true) {
        cout << "Waiting on port " << port << endl;
        recvlen = recvfrom(fd, buffer, 1024, 0, (struct sockaddr*)&remaddr, &addrlen);
        cout << "Received " << recvlen << " bytes\n";
        if (recvlen > 0) {
            buffer[recvlen] = 0;
            cout << "received message: " << buffer << endl;
        }
    }
    /* never exites */
}