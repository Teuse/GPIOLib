#pragma once

namespace gpio
{

class Pin
{
public:

    enum Direction {
        Input,
        Output
    };

	Pin(int pin, Direction dir);
	~Pin();


    auto pin()       const -> int;
    auto direction() const -> Direction;

    auto get()        -> bool;
    void set(bool value);

private:
    Direction _direction;
    int       _pin;
};
}

