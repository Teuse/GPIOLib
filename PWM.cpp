#include "PWM.h"
#include <chrono>
#include <cassert>

namespace cfg
{
    double _period      = 1/500.0;
    int    _resolution  = 100;
}

//---------------------------------------------------------------------

PWM::PWM()
{
}

PWM::~PWM()
{
    stop();
    if (_thread.joinable()) 
        _thread.join();
}

//---------------------------------------------------------------------

void PWM::start()
{
    _thread = std::thread([this](){ runLoop(); }); 
}

void PWM::stop()
{
    _stopThread = true;
}

//---------------------------------------------------------------------

void PWM::setCallback(Callback c) 
{ 
    _amplitudeChange = c; 
}

void PWM::addSignal(int id, float w)
{
    assert( w >= 0.f && w <= 1.f);
    Signal s;
    s.id = id;
    s.width = w;
    s.curState = false;
    _signals.push_back(s);

}

void PWM::pulseWidth(int id, float w)
{ 
    assert( w >= 0.f && w <= 1.f);

    auto it = std::find_if(_signals.begin(), _signals.end(), [id](Signal const& s){
            return s.id == id;
    });

    if (it != _signals.end())
        (*it).width = w;
}

//---------------------------------------------------------------------

void PWM::runLoop()
{
    using namespace std::chrono;

    while (!_stopThread)
    {
        // auto start = system_clock::now();

        for (int i=0; i<cfg::_resolution; ++i)
        {
            processLoop( i/cfg::_resolution );
            auto microsec = int(1000000 * cfg::_period / cfg::_resolution);
            std::this_thread::sleep_for( std::chrono::microseconds(microsec) );
        }

        // auto sleep = ts + _periode - system_clock::now();
        // std::cout << "sleep: " << sleep << std::endl;
        // std::this_thread::sleep_for( sleep );
    }
}

void PWM::processLoop(float pos)
{
    for (auto& s : _signals)
    {
        bool newState = (pos < s.width);

        if (s.curState != newState)
        {
            s.curState = newState;
            _amplitudeChange(s.id, s.curState);
        }
    }
}


