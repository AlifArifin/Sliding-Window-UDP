#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <thread>

using namespace std;
using namespace chrono;
using std::chrono::system_clock;

int main() {
    auto a = high_resolution_clock::now();

    cout << a.time_since_epoch().count() << endl;
    // this_thread::sleep_for(seconds(1));
    high_resolution_clock::time_point b = high_resolution_clock::now() + microseconds(10);
    cout << b.time_since_epoch().count() << endl;
    if (b > a) {
        cout << "yes";
    }
    return 0;
}

// 1541142950295929246
// 1541143020488312190