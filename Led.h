#pragma once
#include "GPIOAccess.h"
#include "PWM.h"

class Led
{
public:

	Led(int pin);
	~Led();

    void alpha(float alpha);

private:

    GPIOAccess _gpio;
    PWM _pwm;

};

