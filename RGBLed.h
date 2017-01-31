#pragma once
#include "GPIOAccess.h"
#include "PWM.h"
#include <vector>

class RGBLed
{
public:

	RGBLed(int pinR, int pinG, int pinB);
	~RGBLed();

    void rgb(float r, float g, float b);

private:

    struct Color 
    {
        Color(int pin) : value(0.f) { gpio.setup(pin,GPIOAccess::Output); }
        float       value;
        GPIOAccess  gpio;
    };

    PWM _pwm;
    std::vector<Color> _colors;

};

