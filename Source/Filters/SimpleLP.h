#pragma once

class SimpleLP
{
public:
    void prepare(double newSampleRate);

    void setCutoff(float newCutoff);
    void setResonance(float newResonance);

    float processSample(float input, int channel);

    void reset();

private:
    void updateCoefficient();

    float sampleRate = 44100.0f;

    float cutoff = 1000.0f;
    float resonance = 0.0f;
    float damping = 1.0f;

    float coefficient = 0.0f;

    float low[2] = {};
    float band[2] = {};

    
};