#include <chrono>
#include <thread>
#include <iostream>

#include "../RGBLed.h"


int main(int argc, char *argv[]) 
{ 
    using namespace std;

    cout << "Test RGB LED with Pulse Width Modulation" << endl;

    RGBLed led(23,18,24);

    cout << "White" << std::endl;
    led(1.f, 1.f, 1.f);
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Color BLUE" << std::endl;
    led(0.f, 0.f, 1.f);
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Color GREEN" << std::endl;
    led(0.f, 1.f, 0.f);
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Color RED" << std::endl;
    led(1.f, 0.f, 0.f);
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Color MIX 1" << std::endl;
    led(1.f, 0.6, 0.2);
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Color MIX 2" << std::endl;
    led(0.5, 0.2, 0.8);
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Color MIX 3" << std::endl;
    led(0.5, 0.5, 0.5);
    this_thread::sleep_for(chrono::seconds(2));


    led(0.f, 0.f, 0.f);
    cout << "Test done" << endl;
    return 0;
}
