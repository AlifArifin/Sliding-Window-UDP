all: sendfile

sendfile: ack.h buffer.h frame.h util.h windowsender.h
	g++ -std=c++11 -pthread ack.cpp buffer.cpp frame.cpp util.cpp windowsender.cpp sendfile.cpp -o sendfile

remove :
	rm -f *.o *.gch