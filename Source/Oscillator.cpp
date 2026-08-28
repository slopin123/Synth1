#include "Oscillator.h"
#include <cmath>

float Oscillator::polyBLEP(float t, float dt)
{
    if (t < dt)
    {
        t /= dt;

        return (2 * t) - (t * t) - 1.0f;
    }

    if (t > 1.0f - dt)
    {
        t = (t - 1.0f) / dt;

        return (t * t) + (2 * t) + 1.0f;
    }

    return 0.0f;
}
float Oscillator::getVoicePosition(int index) const 
{ 
    int count = (int)unisonVoices.size(); 

    if (count <= 1) 
        return 0.0f; 
    
    float position = (float)index / (float)(count - 1);  // find the panning position of the current unison voice, distributed evenly [0, 1]

    return position * 2.0f - 1.0f;  // convert from [0, 1] to [-1, 1] using (2x - 1)
}

void Oscillator::updateFrequencies()
{
    int count = (int)unisonVoices.size();

    if (count == 0)
        return;

    for (int i = 0; i < count; ++i)
    {

        float position = getVoicePosition(i);

        float cents = position * detuneCents;

        unisonVoices[i].frequency = frequency * std::pow(2.0f, cents / 1200.0f);
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
    phaseRandom = juce::jlimit(0.0f, 1.0f, amount);
}

void Oscillator::setStereoSpread(float amount)
{
    stereoSpread = juce::jlimit(0.0f, 1.0f, amount);
}

//wavegen 1.1
void Oscillator::getNextSample(float& left, float& right)
{
    left = 0.0f;
    right = 0.0f;

    if (unisonVoices.empty())
        return;

    const int count = (int)unisonVoices.size();

    float centerLeft = 0.0f;
    float centerRight = 0.0f;

    float extraLeft = 0.0f;
    float extraRight = 0.0f;

    int centerCount = 0;
    int extraCount = 0;

    for (int i = 0; i < count; ++i) // determine how many unison voices to calculate, calculate 1 sample per unison voice and advance phase for each voice.
    {
        auto& voice = unisonVoices[i];

        float phaseIncrement = voice.frequency / (float)sampleRate;

        float sample = 0.0f;

        // Generate waveform

        switch (waveform)
        {
        case 1: // Sine

            sample = std::sin((float)voice.phase * 2.0f * juce::MathConstants<float>::pi);

            break;


        case 2: // Saw

            sample = 2.0f * (float)voice.phase - 1.0f;

            sample -= polyBLEP((float)voice.phase, phaseIncrement);

            break;


        case 3: // Square

            if (voice.phase < 0.5)
                sample = 1.0f;
            else
                sample = -1.0f;

            sample += polyBLEP((float)voice.phase, phaseIncrement);

            sample -= polyBLEP((float)std::fmod(voice.phase + 0.5,1.0), phaseIncrement);

            break;


        case 4: // Noise

            sample = juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f;

            break;
        }

        // Determine center / extra

        bool isCenterVoice = false;

        if (count % 2 == 1) //odd number unison
        {
            int centerIndex = count / 2;  // checks which voice should sit in the center (stereo)

            isCenterVoice = (i == centerIndex);  // checks whether current voice matches the center index
        }

        else // even number unison
        {
            int leftCenterIndex = count / 2 - 1;  // same but with 2 voices in the center

            int rightCenterIndex = count / 2;

            isCenterVoice = (i == leftCenterIndex || i == rightCenterIndex);
        }


        // Calculate stereo position

        float panPosition = getVoicePosition(i); 
        
        panPosition *= stereoSpread;

        
        // Equal-power panning using sin^2+cos^2=1 
     
        float angle = (panPosition + 1.0f) * 0.25f * juce::MathConstants<float>::pi; 

        float leftGain = std::cos(angle); 

        float rightGain = std::sin(angle);

        float leftSample = sample * leftGain;

        float rightSample = sample * rightGain;


        // Add to appropriate group

        if (isCenterVoice)
        {
            centerLeft += leftSample;
            centerRight += rightSample;

            centerCount++;
        }
        else
        {
            extraLeft += leftSample;
            extraRight += rightSample;

            extraCount++;
        }

        // Advance phase

        voice.phase += phaseIncrement;

        if (voice.phase >= 1.0)
            voice.phase -= 1.0;
    }

    // Normalize groups, group avarage out

    if (centerCount > 0)
    {
        centerLeft /=
            (float)centerCount;

        centerRight /=
            (float)centerCount;
    }

    if (extraCount > 0)
    {
        extraLeft /=
            (float)extraCount;

        extraRight /=
            (float)extraCount;
    }

    // Handle 1/2 voice case


    if (extraCount == 0)
    {
        left = centerLeft;
        right = centerRight;
        return;
    }

    // Unison Mix
    

    left =
        centerLeft * (1.0f - unisonMix)
        + extraLeft * unisonMix;

    right =
        centerRight * (1.0f - unisonMix)
        + extraRight * unisonMix;
}