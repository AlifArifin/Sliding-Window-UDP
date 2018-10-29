#include <iostream>
#include <sstream>
#include <typeinfo>
using namespace std;

unsigned int bufferSize;

int main(int argc, char *argv[])
{
    /* Read FileName*/
    char* fileName = argv[1];
    
    /* Read windowSize*/
    unsigned int windowSize;    
    stringstream str_arg2; 
    str_arg2 << argv[2];
    str_arg2 >> windowSize;
    
    /* Read buffer*/
    stringstream str_arg3;
    str_arg3 << argv[3];
    str_arg3 >> bufferSize;

    /* Read port*/
    int port;
    stringstream str_arg4;
    str_arg4 << argv[4];
    str_arg4 >> port; 

    cout << fileName << endl;
    cout << windowSize << endl;
    cout << bufferSize << endl;
    cout << port << endl;

    return 0;
}
