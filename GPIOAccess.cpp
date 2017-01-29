#include "GPIOAccess.h"

#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

 
namespace file 
{
    std::string exportPath()           { return "/sys/class/gpio/export"; }
    std::string unexportPath()         { return "/sys/class/gpio/unexport"; }

    std::string directionPath(int pin) 
    { 
        std::stringstream ss;
        ss << "/sys/class/gpio/gpio" << pin << "/direction";
        return ss.str();
    }

    std::string valueIOPath(int pin)   
    { 
        std::stringstream ss;
        ss << "/sys/class/gpio/gpio" << pin << "/value";
        return ss.str();
    }

    //---------------------------------------------------------------------
    
    void write(std::string path, std::string value)
    {
        std::ofstream myfile;
        myfile.open (path);
        myfile << value; 
        myfile.close();
    }

    std::string read(std::string path)
    {
        std::string value;
        std::ifstream myfile (path);
        if (myfile.is_open())
        {
            getline(myfile, value);
            myfile.close();
        }
        return value;
    }
}

//---------------------------------------------------------------------

GPIOAccess::GPIOAccess(int pin, Direction dir)
: _pin(pin)
, _direction(dir)
{
    file::write(file::exportPath(), std::to_string(pin));

    auto dirString = (dir == Input) ? "in" : "out";
    file::write(file::directionPath(_pin), dirString);

    if (dir == Output) 
        set(false);
}

GPIOAccess::~GPIOAccess()
{
    file::write(file::unexportPath(), std::to_string(_pin));
}

//---------------------------------------------------------------------

auto GPIOAccess::direction() const -> GPIOAccess::Direction { return _direction; }
auto GPIOAccess::pin()       const -> int                   { return _pin; }

//---------------------------------------------------------------------

void GPIOAccess::set(bool value)
{
    assert( _direction == Input );

    auto valueStr = value ? "1" : "0";
    file::write(file::valueIOPath(_pin), valueStr);
}

//---------------------------------------------------------------------

bool GPIOAccess::get()
{
    assert( _direction == Output );

    auto valueStr = file::read(file::valueIOPath(_pin));
	return (valueStr != "0");
}


