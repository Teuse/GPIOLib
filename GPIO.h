#pragma once
#include <string>

/* GPIO Class
 * Purpose: Each object instatiated from this class will control a GPIO pin
 * The GPIO pin number must be passed to the overloaded class constructor
 */
class GPIO
{
public:

    enum Direction {
        Input,
        Output
    };

	GPIO(int pin, Direction dir);
	~GPIO();

    void  value(bool value);
    bool value();

    static float temperature();

private:
    int _gpioPin;
};

