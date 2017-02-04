#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include <portaudio.h>

#include <GPIOLib/GPIOAccess.h>
#include <GPIOLib/Led.h>



namespace cfg 
{ 
    int sampleRate          = 44100;
    int preferredFrameSize  = 512;
}

PaStream* g_stream;
Led g_led_pwm(4);

//---------------------------------------------------------------------

int audioCallback( const void*, void *outputBuffer, unsigned long frames,
        const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
        void *userData )
{
    auto start = high_resolution_clock::now();

    g_led_pwm.process(cfg::sampleRate, frames);

    // Fill output buffer with zero to avoid sound on the soundcard!
    float *out = (float*)outputBuffer;
    std::fill(out, out+frames,0.f);

    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    auto maxDuration = float(1000000 * frames) / cfg::sampleRate;
    if (duration > maxDuration) std::cout << "Processing took too much time: " << duration << " (max: maxDuration)" << std::endl;
    return paContinue;
}

void startPortaudio()
{
    auto err = Pa_Initialize();
    if( err != paNoError ) { std::cout << "Failed to initialize Portaudio: " << Pa_GetErrorText(err) << std::endl; std::abort(); }

    err = Pa_OpenDefaultStream( &g_stream, 0, 1, // input/output channel
            paInt16, cfg::sampleRate, cfg::preferredFrameSize,
            audioCallback, nullptr );
    if( err != paNoError ) { std::cout << "Failed to open Stream: " << Pa_GetErrorText(err) << std::endl; std::abort(); }

    err = Pa_StartStream( g_stream );
    if (err != paNoError) { std::cout << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl; std::abort(); }
}

void stopPortaudio()
{
    Pa_StopStream( g_stream );
    Pa_Terminate();
}

//---------------------------------------------------------------------

int main(int argc, char *argv[]) 
{ 
    using namespace std;

    cout << "Test white LED" << endl;
    {
        GPIOAccess led(4, GPIOAccess::Output);

        this_thread::sleep_for(chrono::seconds(2));
        cout << "Switch ON" << endl;
        led.set(true);

        cout << "Test done" << endl << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }


    startPortaudio();
    cout << "Test white LED without PWM" << endl;
    {
        int steps = 10;

        for (int i=0; i<steps; ++i)
        {
            auto alpha = float(i) / float(steps-1);
            cout << "Set alpha = " << alpha << endl;
            g_led_pwm.alpha(alpha);
            this_thread::sleep_for(chrono::seconds(2));
        }

        cout << "Test done" << endl;
    }
    stopPortaudio();

    return 0;
}
