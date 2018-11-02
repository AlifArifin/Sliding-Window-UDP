all: remove sendfile recvfile

sendfile: ack.h buffer.h frame.h util.h windowsender.h
	g++ -std=c++11 -pthread ack.cpp buffer.cpp frame.cpp util.cpp windowsender.cpp sendfile.cpp -o sendfile

recvfile : ack.h frame.h
	g++ -std=c++11 ack.cpp frame.cpp util.cpp recvfile.cpp -o recvfile

remove :
	rm -f *.o *.gch sendfile recvfile

casesend:
	./sendfile tes.txt 5 10 127.0.0.1 8000

testsend: remove sendfile casesend

caserecv:
	./recvfile tes2.txt 5 10 8000

testrecv: remove recvfile caserecv