#pragma once

class GPIOAccess
{
public:

    enum Direction {
        Input,
        Output
    };

	GPIOAccess();
	GPIOAccess(int pin, Direction dir);
	~GPIOAccess();

    void setup(int pin, Direction dir);

    auto pin()       const -> int;
    auto direction() const -> Direction;

    auto get()        -> bool;
    void set(bool value);

private:
    bool      _setupDone = false;
    Direction _direction;
    int       _pin;
};

