#pragma once
#include <JuceHeader.h>
#include <cmath>
#include <vector>

struct LfoPoint
{
    juce::Point<float> pos;
    float tension = 0.0f;
};

class LfoEditor : public juce::Component, private juce::Timer
{
public:
    enum class LoopMode
    {
        OneShot,
        Loop,
        PingPong
    };

    LfoEditor();
    ~LfoEditor() override;

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

    // Rate & Loop controls
    void setRateHz(float newRateHz);
    void setLoopMode(LoopMode newMode);
    void trigger(); // Resets playhead to start (useful for OneShot / Reset)

    // Evaluate curve value [0..1] at any phase [0..1]
    float getSampleAtPhase(float phase) const;

private:
    void timerCallback() override;

    juce::Point<float> pointToScreen(juce::Point<float> point) const;
    juce::Point<float> screenToPoint(juce::Point<float> position) const;
    int findPointAt(juce::Point<float> position) const;
    juce::Point<float> getTensionHandlePosition(int segmentIndex) const;
    int findTensionHandleAt(juce::Point<float> position) const;

    std::vector<LfoPoint> points{
        { { 0.0f, 0.0f }, 0.0f },
        { { 1.0f, 1.0f }, 0.0f }
    };

    int selectedPoint = -1;
    int selectedTension = -1;
    float dragStartMouseY = 0.0f;
    float dragStartTension = 0.0f;

    // Animation & Playback state
    float rateHz = 1.0f;
    float currentPhase = 0.0f;
    bool pingPongForward = true;
    LoopMode loopMode = LoopMode::Loop;

    uint32_t lastUpdateTimeMs = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LfoEditor)
};