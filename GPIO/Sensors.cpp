#include "Sensors.h"


#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>
#include <string>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace cfg 
{
    const std::string sensorId = "28-00000482b243";

    std::string sensorPath()           
    {
        std::stringstream ss;
        ss << "/sys/bus/w1/devices/" << sensorId << "/w1_slave";
        return ss.str(); 
    }
}

//---------------------------------------------------------------------

namespace gpio 
{

    float Sensors::temperature()
    {
        using namespace std;

        ifstream file( cfg::sensorPath() );
        assert( file.is_open() );

        string line;
        getline(file, line);

        vector<string> words;
        boost::split(words, line, boost::is_any_of(" "));
        assert( words.back() == "YES" ); // check for correct file

        getline(file, line);
        boost::split(words, line, boost::is_any_of("t="));
        auto value = stoi( words.back() );

        file.close();

        return float(value) / 1000.f; 
    }

}

