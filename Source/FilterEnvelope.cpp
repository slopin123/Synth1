#include "FilterEnvelope.h"
#include <cmath>

static float applyCurve(float position, float curve)
{
    if (curve == 0.0f)
        return position;

    if (curve > 0.0f)
    {
        return std::pow(
            position,
            1.0f + curve * 4.0f
        );
    }

    return 1.0f - std::pow(
        1.0f - position,
        1.0f + (-curve * 4.0f)
    );
}

void FilterEnvelope::prepare(double newSampleRate)
{
    sampleRate = (float)newSampleRate;
    reset();
}

void FilterEnvelope::setAttack(float seconds)
{
    attack = seconds;
}

void FilterEnvelope::setDecay(float seconds)
{
    decay = seconds;
}

void FilterEnvelope::setSustain(float level)
{
    sustain = level;
}

void FilterEnvelope::setRelease(float seconds)
{
    release = seconds;
}

void FilterEnvelope::setAttackCurve(float curve)
{
    attackCurve = curve;
}

void FilterEnvelope::setDecayCurve(float curve)
{
    decayCurve = curve;
}

void FilterEnvelope::setReleaseCurve(float curve)
{
    releaseCurve = curve;
}

void FilterEnvelope::noteOn()
{
    stage = Stage::Attack;
    stagePosition = 0.0f;
}

void FilterEnvelope::noteOff()
{
    if (stage != Stage::Idle)
    {
        stage = Stage::Release;
        stagePosition = 0.0f;
    }
}



float FilterEnvelope::getNextSample()
{
    if (stage == Stage::Attack)
    {
        if (attack <= 0.0f)
        {
            value = 1.0f;
            stage = Stage::Decay;
            stagePosition = 0.0f;
        }
        else
        {
            stagePosition +=
                1.0f / (attack * sampleRate);

            if (stagePosition >= 1.0f)
            {
                stagePosition = 1.0f;
                value = 1.0f;

                stage = Stage::Decay;
                stagePosition = 0.0f;
            }
            else
            {
                value = applyCurve(
                    stagePosition,
                    attackCurve
                );
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
            stagePosition +=
                1.0f / (decay * sampleRate);

            if (stagePosition >= 1.0f)
            {
                stagePosition = 1.0f;
                value = sustain;

                stage = Stage::Sustain;
            }
            else
            {
                float curvedPosition =
                    applyCurve(
                        stagePosition,
                        decayCurve
                    );

                value =
                    1.0f
                    + (sustain - 1.0f)
                    * curvedPosition;
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
            stagePosition +=
                1.0f / (release * sampleRate);

            if (stagePosition >= 1.0f)
            {
                stagePosition = 1.0f;
                value = 0.0f;
                stage = Stage::Idle;
            }
            else
            {
                float curvedPosition =
                    applyCurve(
                        stagePosition,
                        releaseCurve
                    );

                value *=
                    1.0f - curvedPosition;
            }
        }
    }
    return value;
}

void FilterEnvelope::reset()
{
    stage = Stage::Idle;
    value = 0.0f;
    stagePosition = 0.0f;
}