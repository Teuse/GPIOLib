#pragma once
#include <thread>
#include <atomic>
#include <functional>
#include <vector>
#include <mutex>


class PWM 
{
    using Callback = std::function<void(int, bool)>;

public:

	PWM();
	~PWM();

    void setCallback(Callback c); 

    void start();
    void stop();

    void addSignal(int id, float w=0.f);
    void pulseWidth(int id, float w);

private:

    struct Signal {
        int id;
        float width;
        bool curState;
    };

    void runLoop();
    void processLoop(float pos);

    Callback            _amplitudeChange;
    std::atomic<bool>   _stopThread = {false};
    std::mutex          _mutex;
    std::thread         _thread;

    std::vector<Signal> _signals;
};

