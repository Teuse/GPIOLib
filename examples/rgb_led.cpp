#include <chrono>
#include <thread>
#include <iostream>

#include "../GPIOAccess.h"


int main(int argc, char *argv[]) 
{ 
    using namespace std;

    cout << "Test without PWM" << endl;

    GPIOAccess red(23, GPIOAccess::Output);
    GPIOAccess green(18, GPIOAccess::Output);
    GPIOAccess blue(24, GPIOAccess::Output);

    cout << "Swith on RED" << endl;
    red.set(true);
    this_thread::sleep_for(chrono::seconds(2));
    red.set(false);

    cout << "Swith on GREEN" << endl;
    green.set(true);
    this_thread::sleep_for(chrono::seconds(2));
    green.set(false);

    cout << "Swith on BLUE" << endl;
    blue.set(true);
    this_thread::sleep_for(chrono::seconds(2));
    blue.set(false);

    cout << "Swith on RED/GREEN" << endl;
    red.set(true);
    green.set(true);
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Swith on RED/GREEN/BLUE" << endl;
    blue.set(true);
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Swith off all colors... " << endl;
    green.set(false);
    red.set(false);
    blue.set(false);

    cout << "Test done" << endl;
    return 0;
}
