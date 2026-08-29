#pragma once

class Envelope
{
public:
    void prepare(double sampleRate);

    void setAttack(float seconds);
    void setDecay(float seconds);
    void setSustain(float level);
    void setRelease(float seconds);

    void setAttackCurve(float curve);
    void setDecayCurve(float curve);
    void setReleaseCurve(float curve);

    bool isActive() const;

    void noteOn();
    void noteOff();

    float getNextSample();

    void reset();

private:
    enum class Stage
    {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };

    Stage stage = Stage::Idle;

    float sampleRate = 44100.0f;

    float attack = 0.01f;
    float decay = 0.2f;
    float sustain = 0.5f;
    float release = 0.2f;

    float attackCurve = 0.0f;
    float decayCurve = 0.0f;
    float releaseCurve = 0.0f;

    float value = 0.0f;
    float position = 0.0f;
    float releaseStartValue = 0.0f;

};