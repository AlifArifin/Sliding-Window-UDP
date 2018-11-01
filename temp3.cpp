#include <iostream>
#include <thread>

using namespace std;

void foo(int *x, bool *done) {
    *x = 5;
    while (!*done) {
        cout << 
    }
}

int main() {
    int x = 1;
    bool done = false;

    thread th1(foo, &x, &done);

    while (x != 5) {  
    }  
    cout << x;
    done = true;
    th1.join();
    return 0;
}