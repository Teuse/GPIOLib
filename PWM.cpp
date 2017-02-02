#include "PWM.h"
#include <chrono>
#include <cassert>
#include <algorithm>

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
    if (_thread.joinable()) _thread.join();
}

//---------------------------------------------------------------------

void PWM::setCallback(Callback c) 
{ 
    _amplitudeChange = c; 
}

void PWM::addSignal(int id, float w)
{
    assert( w >= 0.f && w <= 1.f);
    std::lock_guard<std::mutex> lock(_mutex);

    Signal s;
    s.id = id;
    s.width = w;
    s.curState = false;
    _signals.emplace_back(s);

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

    auto sleepTimeMSec = int(1000 * cfg::_period / cfg::_resolution);
    auto sleepDuration = std::chrono::milliseconds(sleepTimeMSec);

    while (!_stopThread)
    {
        for (int i=0; i<cfg::_resolution; ++i)
        {
            processLoop( i/(cfg::_resolution-1) );
            std::this_thread::sleep_for( sleepDuration );
        }
    }
}

void PWM::processLoop(float pos)
{
    std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
    if(!lock.owns_lock()) return;

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


