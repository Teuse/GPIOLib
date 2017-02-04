#include "Led.h"


 
Led::Led(int pin)
: _gpio(pin, GPIOAccess::Output)
, _on(false)
, _alpha(1.f)
, _curState(false)
{}

Led::~Led()
{
    toggle(false);
    _gpio.set(false);
}

//---------------------------------------------------------------------

void Led::toggle(bool on)
{
    _on = on;
    if (_alpha >= 1.f)
        _gpio.set(on);
}

void Led::alpha(float alpha)
{
    _alpha = alpha;
}

//---------------------------------------------------------------------

void Led::process(int fs, int frames)
{
    _frameCounter += frames;
    if (_on && _alpha < 1.f)
    {
        auto pos = float(_frameCounter) / float(fs);
        bool state = pos < _alpha;
        if (state != _curState)
        {
            _curState = state;
            _gpio.set(state);
        }
    }

    if (_frameCounter > fs) 
        _frameCounter -= fs;
}

//---------------------------------------------------------------------

RGBLed::RGBLed(int pinR, int pinG, int pinB)
: _r(pinR)
, _g(pinG)
, _b(pinB)
, _alpha(1.f)
, _rValue(1.f)
, _gValue(1.f)
, _bValue(1.f)
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
    _rValue = r;
    _gValue = g;
    _bValue = b;
    updateAlpha();
}

void RGBLed::alpha(float a)
{
    _alpha = a;
    updateAlpha();
}

void RGBLed::updateAlpha()
{
    _r.alpha(_rValue * _alpha);
    _g.alpha(_gValue * _alpha);
    _b.alpha(_bValue * _alpha);
}
    
void RGBLed::process(int fs, int frames)
{
    _r.process(fs, frames);
    _g.process(fs, frames);
    _b.process(fs, frames);
}

