all: remove sendfile recvfile

sendfile: src/ack.h src/buffer.h src/frame.h src/util.h src/windowsender.h
	g++ -std=c++11 -pthread src/ack.cpp src/buffer.cpp src/frame.cpp src/util.cpp src/windowsender.cpp src/sendfile.cpp -o sendfile

recvfile : src/ack.h src/frame.h src/util.h
	g++ -std=c++11 src/ack.cpp src/frame.cpp src/util.cpp src/recvfile.cpp -o recvfile

remove :
	rm -f *.o *.gch sendfile recvfile

casesend:
	./sendfile data/tes.jpg 3 10 127.0.0.1 8000

testsend: remove sendfile casesend

caserecv:
	./recvfile tes2.jpg 5 10 8000

testrecv: remove recvfile caserecv