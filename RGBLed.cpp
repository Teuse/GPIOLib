#include "RGBLed.h"

 
RGBLed::RGBLed(int pinR, int pinG, int pinB)
{
    Color r(pinR);
    _colors.push_back(r);
    _pwm.addSignal(0);

    Color g(pinG);
    _colors.push_back(g);
    _pwm.addSignal(1);

    Color b(pinB);
    _colors.push_back(b);
    _pwm.addSignal(2);

    _pwm.setCallback([this](int id, bool value){ _colors[id].gpio.set(value); });
    _pwm.start();
}

RGBLed::~RGBLed()
{
}

//---------------------------------------------------------------------

void RGBLed::rgb(float r, float g, float b)
{
    _colors[0].value = r;
    _pwm.pulseWidth(0, r);

    _colors[1].value = g;
    _pwm.pulseWidth(1, g);

    _colors[2].value = b;
    _pwm.pulseWidth(2, b);
}


