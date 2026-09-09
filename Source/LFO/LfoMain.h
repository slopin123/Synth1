#pragma once
#include <JuceHeader.h>
#include <array>
#include <atomic>

class LfoMain
{
public:
    enum class LoopMode
    {
        Loop,
        OneShot,
        PingPong
    };

    LfoMain();
    ~LfoMain() = default;

    void prepare(double newSampleRate) noexcept;
    void reset() noexcept;

    // Called by GUI thread when curve changes
    void updateTable(const std::array<float, 512>& newTable) noexcept;

    // Audio thread processing
    float getNextSample() noexcept;

    // Control setters (GUI thread -> Audio thread)
    void setRateHz(float newRateHz) noexcept;
    void setLoopMode(LoopMode newMode) noexcept;
    void setRetrig(bool shouldRetrig) noexcept;
    void setEnabled(bool isEnabled) noexcept;

    // Telemetry getters for LfoEditor UI sync (Audio thread -> GUI thread)
    float getAudioPhase() const noexcept { return currentPhase.load(std::memory_order_relaxed); }
    bool isVoiceActive() const noexcept { return enabled.load(std::memory_order_relaxed); }

private:
    void advancePhase() noexcept;

    double sampleRate = 44100.0;

    std::array<std::atomic<float>, 512> lookupTable;
    std::atomic<float> currentPhase{ 0.0f };
    std::atomic<float> rateHz{ 1.0f };

    std::atomic<LoopMode> loopMode{ LoopMode::Loop };
    std::atomic<bool> retrigEnabled{ true };
    std::atomic<bool> enabled{ true };

    bool pingPongForward = true;
};