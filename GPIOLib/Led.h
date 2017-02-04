#pragma once
#include "GPIOAccess.h"
#include <atomic>


class Led
{
public:

	Led(int pin);
	~Led();

    void toggle(bool);
    void alpha(float);

    void process(int fs, int frames);

private:

    GPIOAccess          _gpio;
    std::atomic<bool>   _on;
    std::atomic<float>  _alpha;

    bool   _curState;
    int _frameCounter;
};

//---------------------------------------------------------------------

class RGBLed
{
public:
    RGBLed(int pinR, int pinG, int pinB);
    ~RGBLed() {}

    void toggle(bool);
    void rgb(float, float, float);
    void alpha(float);
    
    void process(int fs, int frames);

private:
    void updateAlpha();
    Led _r;
    Led _g;
    Led _b;
    float _alpha;
    float _rValue;
    float _gValue;
    float _bValue;
};

