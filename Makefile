all: remove sendfile

sendfile: ack.h buffer.h frame.h util.h windowsender.h
	g++ -std=c++11 -pthread ack.cpp buffer.cpp frame.cpp util.cpp windowsender.cpp sendfile.cpp -o sendfile

recvfile :

remove :
	rm -f *.o *.gch sendfile recvfile

case:
	./sendfile tes.jpg 5 10 127.0.0.1 8000

test: remove sendfile case