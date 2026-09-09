#include "SimpleLP.h"
#include <JuceHeader.h>
#include <cmath>

// 2-pole State Variable Filter (SVF)

void SimpleLP::prepare(double newSampleRate)
{
    sampleRate = (float)newSampleRate;

    reset();

    updateCoefficient();
}

void SimpleLP::setCutoff(float newCutoff)
{
    cutoff = juce::jlimit(20.0f, sampleRate * 0.45f, newCutoff);

    updateCoefficient();
}

void SimpleLP::setResonance(float newResonance)
{
    resonance = juce::jlimit(0.0f, 1.0f, newResonance);

    damping = 0.15f + 0.85f * (1.0f - resonance);
}

void SimpleLP::updateCoefficient()
{
    coefficient = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * cutoff / sampleRate);
}

float SimpleLP::processSample(float input, int channel) 
{
    // Difference equation: y[n] = (2 - g*R - g^2) * y[n-1] - (1 - g*R) * y[n-2] + g^2 * x[n]
    channel = juce::jlimit(0, 1, channel);

    //  h[n] = x[n] - y[n-1] - R * b[n-1]
    float high = input - low[channel] - damping * band[channel];

    //  b[n] = b[n-1] + g * h[n]
    float newBand = band[channel] + coefficient * high;

    //  y[n] = y[n-1] + g * b[n]
    float newLow = low[channel] + coefficient * newBand;

    band[channel] = newBand;
    low[channel] = newLow;

    return newLow;
}

void SimpleLP::reset()
{
    low[0] = 0.0f;
    low[1] = 0.0f;

    band[0] = 0.0f;
    band[1] = 0.0f;
}