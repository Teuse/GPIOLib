#pragma once
#include "GPIOAccess.h"
#include "PWM.h"

class RGBLed
{
public:

	RGBLed(int pinR, int pinG, int pinB);
	~RGBLed();

    void rgb(float r, float g, float b);

private:

    GPIOAccess _gpioR;
    GPIOAccess _gpioG;
    GPIOAccess _gpioB;

    PWM _pwm;

};

