#pragma once

namespace gpio
{

class GPIOAccess
{
public:

    enum Direction {
        Input,
        Output
    };

	GPIOAccess(int pin, Direction dir);
	~GPIOAccess();


    auto pin()       const -> int;
    auto direction() const -> Direction;

    auto get()        -> bool;
    void set(bool value);

private:
    bool      _setupDone = false;
    Direction _direction;
    int       _pin;
};
}

