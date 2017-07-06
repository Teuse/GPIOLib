#pragma once
#include <GPIO/Pin.h>
#include <atomic>
#include <array>

namespace gpio {

class Led
{
public:

	Led(int pin, int fs=0);
	~Led();

    void toggle(bool);
    void alpha(float);

    void process();

private:

    bool usingPWM();
    void setState(bool);

    Pin  _pin;
    //int  _fs;

    std::atomic<bool> _on          = {false};
    std::atomic<int>  _samplesPerPeriod;
    std::atomic<int>  _alphaFactor = {0};

    bool _curState     = false;
    int  _frameCounter = 0;
};

//---------------------------------------------------------------------

class RGBLed
{
public:
    RGBLed(int pinR, int pinG, int pinB, int fs=0);
    ~RGBLed() {}

    void toggle(bool);
    void rgb(float, float, float);
    void alpha(float);
    
    void process();

private:
    void updateAlpha();
    Led _r;
    Led _g;
    Led _b;
    std::array<float, 4> _rgba;
};

}

