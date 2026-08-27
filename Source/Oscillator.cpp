#include "Oscillator.h"
#include <cmath>

float Oscillator::polyBLEP(float t, float dt)
{
    if (t < dt)
    {
        t /= dt;

        return t + t
            - t * t
            - 1.0f;
    }

    if (t > 1.0f - dt)
    {
        t = (t - 1.0f) / dt;

        return t * t
            + t
            + t
            + 1.0f;
    }

    return 0.0f;
}

void Oscillator::updateFrequencies()
{
    int count = (int)unisonVoices.size();

    if (count == 0)
        return;

    for (int i = 0; i < count; ++i)
    {
        float position;

        if (count == 1)
        {
            position = 0.0f;
        }
        else
        {
            position =
                (float)i /
                (float)(count - 1);

            position =
                position * 2.0f - 1.0f;
        }

        float cents =
            position * detuneCents;

        unisonVoices[i].frequency =
            frequency *
            std::pow(
                2.0f,
                cents / 1200.0f
            );
    }
}

void Oscillator::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;

    unisonVoices.resize(1);

    unisonVoices[0].frequency = frequency;
    unisonVoices[0].phase = 0.0;
}
void Oscillator::setFrequency(float newFrequency)
{
    frequency = newFrequency;

    updateFrequencies();
}

void Oscillator::setWaveform(int newWaveform)
{
    waveform = newWaveform;
}

void Oscillator::reset()
{
    auto& random =
        juce::Random::getSystemRandom();

    for (auto& voice : unisonVoices)
    {
        if (phaseRandom <= 0.0f)
        {
            voice.phase = 0.0;
        }
        else
        {
            voice.phase =
                random.nextFloat() * phaseRandom;
        }
    }
}

//unison
void Oscillator::setUnisonVoices(int numberOfVoices)
{
    numUnisonVoices = juce::jlimit(1, 16, numberOfVoices);

    unisonVoices.resize(numUnisonVoices);

    for (auto& voice : unisonVoices)
        voice.phase = 0.0;

    updateFrequencies();
}

void Oscillator::setDetune(float cents)
{
    detuneCents = juce::jlimit(0.0f, 100.0f, cents);

    updateFrequencies();
}

void Oscillator::setUnisonMix(float amount)
{
    unisonMix = juce::jlimit(0.0f, 1.0f, amount);
}

void Oscillator::setPhaseRandom(float amount)
{
    phaseRandom =
        juce::jlimit(0.0f, 1.0f, amount);
}

//wavegen
float Oscillator::getNextSample()
{
    if (unisonVoices.empty())
        return 0.0f;

    float centerSum = 0.0f;
    float extraSum = 0.0f;

    int centerCount = 0;
    int extraCount = 0;

    const int count =
        (int)unisonVoices.size();

    for (int i = 0; i < count; ++i)
    {
        auto& voice = unisonVoices[i];

        float phaseIncrement =
            voice.frequency /
            (float)sampleRate;

        float sample = 0.0f;

        switch (waveform)
        {
        case 1: // Sine

            sample = std::sin(
                (float)voice.phase
                * 2.0f
                * juce::MathConstants<float>::pi
            );

            break;


        case 2: // Saw

            sample =
                2.0f * (float)voice.phase - 1.0f;

            sample -= polyBLEP(
                (float)voice.phase,
                phaseIncrement
            );

            break;


        case 3: // Square

            sample =
                voice.phase < 0.5
                ? 1.0f
                : -1.0f;

            sample += polyBLEP(
                (float)voice.phase,
                phaseIncrement
            );

            sample -= polyBLEP(
                (float)std::fmod(
                    voice.phase + 0.5,
                    1.0
                ),
                phaseIncrement
            );

            break;


        case 4: // Noise

            sample =
                juce::Random::getSystemRandom()
                .nextFloat() * 2.0f - 1.0f;

            break;
        }

        // Determine whether this is a center
        // or extra oscillator.

        if (count % 2 == 1)
        {
            // Odd number:
            // one oscillator exactly in the middle

            int centerIndex =
                count / 2;

            if (i == centerIndex)
            {
                centerSum += sample;
                centerCount++;
            }
            else
            {
                extraSum += sample;
                extraCount++;
            }
        }
        else
        {
            // Even number:
            // two oscillators around the center

            int centerIndex1 =
                count / 2 - 1;

            int centerIndex2 =
                count / 2;

            if (i == centerIndex1 ||
                i == centerIndex2)
            {
                centerSum += sample;
                centerCount++;
            }
            else
            {
                extraSum += sample;
                extraCount++;
            }
        }

        // Advance phase

        voice.phase += phaseIncrement;

        if (voice.phase >= 1.0)
            voice.phase -= 1.0;
    }

    // Normalize each group

    float centerMix = 0.0f;
    float extraMix = 0.0f;

    if (centerCount > 0)
    {
        centerMix =
            centerSum /
            (float)centerCount;
    }

    if (extraCount > 0)
    {
        extraMix =
            extraSum /
            (float)extraCount;
    }

    // Crossfade between center and extras

    if (extraCount == 0)
    {
        return centerMix;
    }

    return
        centerMix * (1.0f - unisonMix)
        + extraMix * unisonMix;
}