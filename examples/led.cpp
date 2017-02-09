#include <chrono>
#include <thread>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include <portaudio.h>

#include <GPIOLib/Led.h>



namespace cfg 
{ 
    int sampleRate          = 5000;
    int preferredFrameSize  = 1;
}

PaStream* g_stream;
gpio::Led g_led_pwm(4, cfg::sampleRate);

//---------------------------------------------------------------------

int audioCallback( const void*, void *outputBuffer, unsigned long frames,
        const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
        void *userData )
{
    // using namespace std::chrono;
    // auto start = high_resolution_clock::now();

    g_led_pwm.process();

    // Fill output buffer with zero to avoid sound on the soundcard!
    // float *out = (float*)outputBuffer;
    // std::fill(out, out+frames,0.f);
    //
    // auto end         = high_resolution_clock::now();
    // auto duration    = duration_cast<microseconds>( end - start ).count();
    // auto maxDuration = float(1000000 * frames) / cfg::sampleRate;
    // if (duration > maxDuration) std::cout << "Processing took too much time: " << duration << " (max: " << maxDuration << ")" << std::endl;

    return paContinue;
}

void startPortaudio()
{
    auto err = Pa_Initialize();
    if( err != paNoError ) { std::cout << "Failed to initialize Portaudio: " << Pa_GetErrorText(err) << std::endl; std::abort(); }

    err = Pa_OpenDefaultStream( &g_stream, 0, 1, // input/output channel
            paInt16, double(cfg::sampleRate), cfg::preferredFrameSize,
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
        gpio::Led led(4) ;

        cout << "Switch ON" << endl;
        led.toggle(true);
        this_thread::sleep_for(chrono::seconds(2));
        led.toggle(false);

        cout << "Test done" << endl << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }


    startPortaudio();
    cout << "Test white LED without PWM" << endl;
    {
        std::cout << "Switch on with alpha = 1.0" << std::endl;
        g_led_pwm.toggle(true);
        g_led_pwm.alpha(1.f);
        this_thread::sleep_for(chrono::seconds(2));
        int steps = 10;

        for (int i=0; i<steps; ++i)
        {
            auto alpha = float(i) / float(steps-1);
            cout << "Set alpha = " << alpha << endl;
            g_led_pwm.alpha(alpha);
            this_thread::sleep_for(chrono::seconds(2));
        }

        g_led_pwm.toggle(false);
        cout << "Test done" << endl;
    }
    stopPortaudio();

    return 0;
}
