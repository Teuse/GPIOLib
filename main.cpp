#include <chrono>
#include <thread>
#include <iostream>

#include "GPIO.h"


int main(int argc, char *argv[]) 
{ 
    using namespace std;

    cout << "Export pin 18 " << endl;
    GPIO led(18, GPIO::Output);

    for(int i=0; i<10; ++i)
    {
        this_thread::sleep_for(chrono::seconds(2));
        cout << "Switch ON" << endl;
        led.value(true);

        cout << "Switch OFF" << endl;
        led.value(false);

    }

    cout << "Unexport pin 18" << endl;

    return 0;
}
