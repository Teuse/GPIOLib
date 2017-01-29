#include "GPIO.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace cfg 
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

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    std::string sensorId()             { return "28-00000482b243"; }
    std::string sensorPath()           
    {
        std::stringstream ss;
        ss << "/sys/bus/w1/devices/" << sensorId() << "w1_slave";
        return ss.str(); 
    }
}

 
namespace file 
{
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
        else std::cout << "Unable to open file";

        return value;
    }
}

//---------------------------------------------------------------------

GPIO::GPIO(int pin, Direction dir)
: _gpioPin(pin)
{
    // assert(pin > 0 && pin < 16);

    file::write(cfg::exportPath(), std::to_string(pin));

    auto dirString = (dir == Input) ? "in" : "out";
    file::write(cfg::directionPath(_gpioPin), dirString);
}

GPIO::~GPIO()
{
    file::write(cfg::unexportPath(), std::to_string(_gpioPin));
}

//---------------------------------------------------------------------

void GPIO::value(bool value)
{
    assert(value >= 0.f && value <= 1.f);

    auto valueStr = std::to_string(value);
    file::write(cfg::valueIOPath(_gpioPin), valueStr);
}

//---------------------------------------------------------------------

bool GPIO::value()
{
    auto valueStr = file::read(cfg::valueIOPath(_gpioPin));

    auto value = ::atof(valueStr.c_str());
    assert(value >= 0.f && value <= 1.f);

	return value;
}

//---------------------------------------------------------------------

float GPIO::temperature()
{
    int value = 0;
    std::ifstream myfile( cfg::sensorPath() );
    if (myfile.is_open())
    {
        std::string line;
        getline(myfile, line);

        std::vector<std::string> words;
        boost::split(words, line, boost::is_any_of(" "));
        assert( words.back() == "YES" ); // check for correct file

        getline(myfile, line);
        boost::split(words, line, boost::is_any_of("t="));
        value = std::stoi( words.back() );

        myfile.close();
    }
    else std::cout << "Unable to open file";

    return float(value) / 1000.f; 
}

