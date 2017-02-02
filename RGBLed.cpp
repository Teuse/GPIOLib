#include "RGBLed.h"

 
RGBLed::RGBLed(int pinR, int pinG, int pinB)
: _gpioR(pinR, GPIOAccess::Output)
, _gpioG(pinG, GPIOAccess::Output)
, _gpioB(pinB, GPIOAccess::Output)
{
    _pwm.addSignal(0);
    _pwm.addSignal(1);
    _pwm.addSignal(2);

    _pwm.setCallback([this](int id, bool value)
    { 
        if      (id == 0) _gpioR.set(value);
        else if (id == 1) _gpioG.set(value);
        else if (id == 2) _gpioB.set(value);
    });
    _pwm.start();
}

RGBLed::~RGBLed()
{
    _pwm.stop();
    _gpioR.set(false);
    _gpioG.set(false);
    _gpioB.set(false);
}

//---------------------------------------------------------------------

void RGBLed::rgb(float r, float g, float b)
{
    _pwm.pulseWidth(0, r);
    _pwm.pulseWidth(1, g);
    _pwm.pulseWidth(2, b);
}


