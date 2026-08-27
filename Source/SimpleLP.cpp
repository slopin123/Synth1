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

float SimpleLP::processSample(float input)
{
    float coefficient =
        1.0f - std::exp(
            -2.0f
            * juce::MathConstants<float>::pi
            * cutoff
            / sampleRate
        );

    float high =
        input
        - low
        - damping * band;

    float newBand =
        band
        + coefficient * high;

    float newLow =
        low
        + coefficient * newBand;

    band = newBand;
    low = newLow;

    return low;
}

void SimpleLP::reset()
{
    low = 0.0f;
    band = 0.0f;
}