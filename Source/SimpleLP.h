#pragma once

class SimpleLP
{
public:
    void prepare(double newSampleRate);

    void setCutoff(float newCutoff);
    void setResonance(float newResonance);

    float processSample(float input);

    void reset();

private:
    float sampleRate = 44100.0f;

    float cutoff = 1000.0f;
    float resonance = 0.0f;

    float low = 0.0f;
    float band = 0.0f;
    float high = 0.0f;

    float damping = 1.0f;
};