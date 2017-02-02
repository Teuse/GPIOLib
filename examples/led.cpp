#include <chrono>
#include <thread>
#include <iostream>

#include "../GPIOAccess.h"
#include "../Led.h"


int main(int argc, char *argv[]) 
{ 
    using namespace std;

    cout << "Test white LED" << endl;
    {
        GPIOAccess led(4, GPIOAccess::Output);

        for(int i=0; i<3; ++i)
        {
            this_thread::sleep_for(chrono::seconds(2));
            cout << "Switch ON" << endl;
            led.set(true);

            this_thread::sleep_for(chrono::seconds(2));
            cout << "Switch OFF" << endl;
            led.set(false);
        }

        cout << "Test done" << endl << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }


    cout << "Test white LED without PWM" << endl;
    {
        Led led2(4);

        int steps = 10;

        for (int i=0; i<steps; ++i)
        {
            auto alpha = float(i) / float(steps-1);
            cout << "Set alpha = " << alpha << endl;
            led2.alpha(alpha);
            this_thread::sleep_for(chrono::seconds(2));
        }

        cout << "Test done" << endl;
    }

    return 0;
}
