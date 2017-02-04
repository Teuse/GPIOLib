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
    int sampleRate          = 8000;
    int preferredFrameSize  = 512;
}

PaStream* g_stream;
RGBLed g_led_pwm(23,18,24);

//---------------------------------------------------------------------

int audioCallback( const void*, void *outputBuffer, unsigned long frames,
        const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
        void *userData )
{
    std::cout << "audio callback" << std::endl;

    g_led_pwm.process(cfg::sampleRate, frames);

    // Fill output buffer with zero to avoid sound on the soundcard!
    float *out = (float*)outputBuffer;
    std::fill(out, out+frames,0.f);
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

        for(int i=0; i<3; ++i)
        {
            this_thread::sleep_for(chrono::seconds(2));
            cout << "Switch ON" << endl;
            led.set(true);

            this_thread::sleep_for(chrono::seconds(2));
            cout << "Switch OFF" << endl;
            led.set(false);
        }

        cout << "Test done" << endl << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }


    startPortaudio();
    cout << "Test white LED without PWM" << endl;
    {
        int steps = 10;

        g_led_pwm.rgb(0.1, 0.1, 0.1);
        this_thread::sleep_for(chrono::seconds(2));

        g_led_pwm.rgb(0.1, 0.9, 0.1);
        this_thread::sleep_for(chrono::seconds(2));

        g_led_pwm.rgb(0.1, 0.5, 0.2);
        this_thread::sleep_for(chrono::seconds(2));

        g_led_pwm.rgb(0.6, 0.6, 0.1);
        this_thread::sleep_for(chrono::seconds(2));

        g_led_pwm.rgb(0.8, 0.8, 0.8);
        this_thread::sleep_for(chrono::seconds(2));

        g_led_pwm.rgb(1.f, 1.f, 1.f);
        this_thread::sleep_for(chrono::seconds(2));


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
