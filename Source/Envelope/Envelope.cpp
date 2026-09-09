#include "Envelope.h"
#include <cmath>

static float applyCurve(float position, float curve)
{
    // Clamp curve parameter to expected range [-0.95, 0.95]
    curve = juce::jlimit(-0.95f, 0.95f, curve);

    if (std::abs(curve) < 0.001f)
        return position;

    // Use base-10 exponential scaling to match the exact LFO curve behavior
    float absCurve = std::abs(curve);
    float power = std::pow(10.0f, absCurve * 1.3f);

    if (curve > 0.0f)
    {
        // Convex bend (plucked / fast response)
        return std::pow(position, power);
    }

    // Concave bend (slow logarithmic swells)
    return 1.0f - std::pow(1.0f - position, power);
}

void Envelope::prepare(double newSampleRate)
{
    sampleRate = (float)newSampleRate;
    reset();
}

void Envelope::setAttack(float seconds)
{
    attack = seconds;
}

void Envelope::setDecay(float seconds)
{
    decay = seconds;
}

void Envelope::setSustain(float level)
{
    sustain = level;
}

void Envelope::setRelease(float seconds)
{
    release = seconds;
}

void Envelope::setAttackCurve(float curve)
{
    attackCurve = curve;
}

void Envelope::setDecayCurve(float curve)
{
    decayCurve = curve;
}

void Envelope::setReleaseCurve(float curve)
{
    releaseCurve = curve;
}

void Envelope::noteOn()
{
    stage = Stage::Attack;
    position = 0.0f;
}

void Envelope::noteOff()
{
    if (stage != Stage::Idle)
    {
        releaseStartValue = value;
        stage = Stage::Release;
        position = 0.0f;
    }
}

bool Envelope::isActive() const
{
    return stage != Stage::Idle;
}



float Envelope::getNextSample()
{
    if (stage == Stage::Attack)
    {
        if (attack <= 0.0f)
        {
            value = 1.0f;
            stage = Stage::Decay;
            position = 0.0f;
        }
        else
        {
            position += 1.0f / (attack * sampleRate); //normalized position

            if (position >= 1.0f)
            {
                value = 1.0f;
                stage = Stage::Decay;
                position = 0.0f;
            }
            else
            {
                value = applyCurve(position, attackCurve);
            }
        }
    }

    if (stage == Stage::Decay)
    {
        if (decay <= 0.0f)
        {
            value = sustain;
            stage = Stage::Sustain;
        }
        else
        {
            position +=
                1.0f / (decay * sampleRate);

            if (position >= 1.0f)
            {
                position = 1.0f;
                value = sustain;

                stage = Stage::Sustain;
            }
            else
            {
                float curvedPosition =
                    applyCurve(position, decayCurve);

                value = 1.0f + (sustain - 1.0f) * curvedPosition;
            }
        }
    }

    if (stage == Stage::Sustain)
    {
        value = sustain;
    }

    if (stage == Stage::Release)
    {
        if (release <= 0.0f)
        {
            value = 0.0f;
            stage = Stage::Idle;
        }
        else
        {
            position += 1.0f / (release * sampleRate);

            if (position >= 1.0f)
            {
                position = 1.0f;
                value = 0.0f;
                stage = Stage::Idle;
            }
            else
            {
                float curvedPosition = applyCurve(position, releaseCurve);

                value = releaseStartValue * (1.0f - curvedPosition);
            }
        }
    }
    return value;
}

void Envelope::reset()
{
    stage = Stage::Idle;
    value = 0.0f;
    position = 0.0f;
}