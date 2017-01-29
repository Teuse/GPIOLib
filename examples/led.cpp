#include <chrono>
#include <thread>
#include <iostream>

#include "GPIOAccess.h"


int main(int argc, char *argv[]) 
{ 
    using namespace std;

    cout << "Export pin 18 " << endl;
    GPIOAccess led(18, GPIOAccess::Output);

    for(int i=0; i<5; ++i)
    {
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Switch ON" << endl;
        led.set(true);

        cout << "Switch OFF" << endl;
        led.set(false);
    }

    cout << "Unexport pin 18" << endl;

    return 0;
}
