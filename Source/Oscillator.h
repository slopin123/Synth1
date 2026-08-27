#pragma once

#include <JuceHeader.h>
//testing git
class Oscillator
{
public:

    void prepare(double newSampleRate);

    void setFrequency(float newFrequency);
    void setWaveform(int newWaveform);

    void setUnisonVoices(int numberOfVoices);
    void setDetune(float cents);
    void setUnisonMix(float amount);
    void setPhaseRandom(float amount);

    float getNextSample();

    void reset();

private:

    void updateFrequencies();

    struct UnisonVoice
    {
        double phase = 0.0;
        float frequency = 440.0f;
    };

    std::vector<UnisonVoice> unisonVoices;

    double sampleRate = 44100.0;
    double frequency = 440.0;
    int waveform = 0;



    //unison
    float detuneCents = 0.0f;
    float unisonMix = 0.0f;

    int numUnisonVoices = 1;
    float phaseRandom = 0.0f;

    //Anti-aliasing
    float polyBLEP(float t,float dt);

};