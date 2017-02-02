#include "Led.h"

 
Led::Led(int pin)
: _gpio(pin, GPIOAccess::Output)
{
    _pwm.addSignal(0);
    alpha(0.f);

    _pwm.setCallback([this](int id, bool value)
    { 
        _gpio.set(value);
    });
    _pwm.start();
}

Led::~Led()
{
    _pwm.stop();
    _gpio.set(false);
}

//---------------------------------------------------------------------

void Led::alpha(float alpha)
{
    _pwm.pulseWidth(0, alpha);
}


