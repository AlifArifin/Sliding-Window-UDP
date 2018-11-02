#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>	/* needed for sockaddr_in */


using namespace std;

int main(int argc, char* argv[]) {
    string filename;
    char* destinationip;
    unsigned int windowsize, buffersize, destinationport, alen;
    struct sockaddr_in myaddr, remaddr;
    struct hostent *hp;
    int fd, slen=sizeof(remaddr);
    char buffer[1024];

    filename = argv[1];
    windowsize = atoi(argv[2]);
    buffersize = atoi(argv[3]);
    destinationip = argv[4];
    destinationport = atoi(argv[5]);

	/* create a socket */

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cerr << "cannot create socket\n";
        return 1;
    }

    cout << "Socket created. Descriptor: " << fd << endl;

	/* bind it to all local addresses and pick any port number */

    memset((char*)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port=htons(destinationport);

    if (bind(fd, (struct sockaddr*) &myaddr, sizeof(myaddr)) < 0) {
        cerr << "bind failed\n";
        return 1;
    }

	/* now define remaddr, the address to whom we want to send messages */

    memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(destinationport);
	if (inet_aton(destinationip, &remaddr.sin_addr)==0) {
        cerr << "inet_aton() failed\n";
		exit(1);
	}

	/* now let's send the messages */

    for (int i=0; i < 5; i++) {
        cout << "Sending packet " << i << " to " << destinationip << " port " << destinationport<< endl;
		sprintf(buffer, "This is packet %d", i);
		if (sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr *)&remaddr, slen)==-1)
			cerr << "sendto error\n";
	}

    /* close the socket */
    close(fd);

    return 0;
}