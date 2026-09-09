#include "LfoMain.h"

LfoMain::LfoMain()
{
    // Initialize lookup table to a linear ramp [0.0 - 1.0]
    for (size_t i = 0; i < 512; ++i)
    {
        lookupTable[i].store(static_cast<float>(i) / 511.0f, std::memory_order_relaxed);
    }
}

void LfoMain::prepare(double newSampleRate) noexcept
{
    sampleRate = newSampleRate > 0.0 ? newSampleRate : 44100.0;
}

void LfoMain::reset() noexcept
{
    if (retrigEnabled.load(std::memory_order_relaxed))
    {
        currentPhase.store(0.0f, std::memory_order_relaxed);
        pingPongForward = true;
    }
}

void LfoMain::updateTable(const std::array<float, 512>& newTable) noexcept
{
    for (size_t i = 0; i < 512; ++i)
    {
        lookupTable[i].store(newTable[i], std::memory_order_relaxed);
    }
}

void LfoMain::setRateHz(float newRateHz) noexcept
{
    rateHz.store(juce::jmax(0.01f, newRateHz), std::memory_order_relaxed);
}

void LfoMain::setLoopMode(LoopMode newMode) noexcept
{
    loopMode.store(newMode, std::memory_order_relaxed);
    pingPongForward = true;
}

void LfoMain::setRetrig(bool shouldRetrig) noexcept
{
    retrigEnabled.store(shouldRetrig, std::memory_order_relaxed);
}

void LfoMain::setEnabled(bool isEnabled) noexcept
{
    enabled.store(isEnabled, std::memory_order_relaxed);
}

float LfoMain::getNextSample() noexcept
{
    if (!enabled.load(std::memory_order_relaxed))
        return 0.0f;

    // Fetch current phase [0.0 to 1.0]
    float phase = currentPhase.load(std::memory_order_relaxed);

    // Read lookup table using fast bitwise wrapping (512 samples)
    int index = static_cast<int>(phase * 511.0f) & 511;
    float sample = lookupTable[index].load(std::memory_order_relaxed);

    // Advance phase for next step
    advancePhase();

    return sample;
}

void LfoMain::advancePhase() noexcept
{
    float phase = currentPhase.load(std::memory_order_relaxed);
    float phaseIncrement = rateHz.load(std::memory_order_relaxed) / static_cast<float>(sampleRate);

    auto mode = loopMode.load(std::memory_order_relaxed);

    if (mode == LoopMode::Loop)
    {
        phase += phaseIncrement;
        while (phase >= 1.0f)
            phase -= 1.0f;
    }
    else if (mode == LoopMode::OneShot)
    {
        phase += phaseIncrement;
        if (phase >= 1.0f)
            phase = 1.0f;
    }
    else if (mode == LoopMode::PingPong)
    {
        if (pingPongForward)
        {
            phase += phaseIncrement;
            if (phase >= 1.0f)
            {
                phase = 1.0f - (phase - 1.0f);
                pingPongForward = false;
            }
        }
        else
        {
            phase -= phaseIncrement;
            if (phase <= 0.0f)
            {
                phase = -phase;
                pingPongForward = true;
            }
        }
    }

    currentPhase.store(juce::jlimit(0.0f, 1.0f, phase), std::memory_order_relaxed);
}