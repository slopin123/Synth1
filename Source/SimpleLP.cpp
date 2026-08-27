#include "SimpleLP.h"
#include <JuceHeader.h>
#include <cmath>

void SimpleLP::prepare(double newSampleRate)
{
    sampleRate = (float)newSampleRate;

    reset();
}

void SimpleLP::setCutoff(float newCutoff)
{
    cutoff = juce::jlimit(
        20.0f,
        sampleRate * 0.45f,
        newCutoff
    );
}

void SimpleLP::setResonance(float newResonance)
{
    resonance = juce::jlimit(
        0.0f,
        1.0f,
        newResonance
    );

    damping =
        0.15f
        + 0.85f * (1.0f - resonance);
}

float SimpleLP::processSample(float input, int channel)
{
    channel = juce::jlimit(0, 1, channel);

    float coefficient =
        1.0f - std::exp(
            -2.0f
            * juce::MathConstants<float>::pi
            * cutoff
            / sampleRate
        );

    float high =
        input
        - low[channel]
        - damping * band[channel];

    float newBand =
        band[channel]
        + coefficient * high;

    float newLow =
        low[channel]
        + coefficient * newBand;

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