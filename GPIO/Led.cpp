#include "Led.h"
#include <cassert>
#include <iostream>

namespace cfg
{
    int frequency = 50;
}

namespace gpio
{
 
Led::Led(int pin, int fs)
: _pin(pin, Pin::Output)
, _samplesPerPeriod(fs / cfg::frequency)
{}

Led::~Led()
{
    toggle(false);
    _pin.set(false);
}

//---------------------------------------------------------------------

bool Led::usingPWM()
{
    return _samplesPerPeriod > 0;
}

void Led::toggle(bool on)
{
    _on = on;
    if (!usingPWM())
        _pin.set(on);
}

void Led::alpha(float alpha)
{
    _alphaFactor = int(alpha * _samplesPerPeriod);
}

//---------------------------------------------------------------------

void Led::setState(bool state)
{
    if (state != _curState)
    {
        _curState = state;
        _pin.set(state);
    }
}

void Led::process()
{
    assert(usingPWM());

    if (_on && usingPWM())
    {
        _frameCounter = (++_frameCounter >= _samplesPerPeriod) ? 0 : _frameCounter; 

        bool state = _frameCounter < _alphaFactor;
        setState(state);
    }
    else  
    {
        _frameCounter = 0;
        setState(false);
    }
}

//---------------------------------------------------------------------

RGBLed::RGBLed(int pinR, int pinG, int pinB, int fs)
: _r(pinR, fs)
, _g(pinG, fs)
, _b(pinB, fs)
, _rgba({{1.f, 1.f, 1.f, 1.f}})
{}

//---------------------------------------------------------------------

void RGBLed::toggle(bool on)
{
    _r.toggle(on);
    _g.toggle(on);
    _b.toggle(on);
}

void RGBLed::rgb(float r, float g, float b)
{
    _rgba[0] = r;
    _rgba[1] = g;
    _rgba[2] = b;
    updateAlpha();
}

void RGBLed::alpha(float a)
{
    _rgba[3] = a;
    updateAlpha();
}

void RGBLed::updateAlpha()
{
    _r.alpha(_rgba[0] * _rgba[3]);
    _g.alpha(_rgba[1] * _rgba[3]);
    _b.alpha(_rgba[2] * _rgba[3]);
}
    
void RGBLed::process()
{
    _r.process();
    _g.process();
    _b.process();
}

}

