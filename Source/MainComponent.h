#pragma once

#include <JuceHeader.h>
#include "FilterEnvelope.h"
#include "SimpleLP.h"
#include <set>
#include "Oscillator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

//POLYPHONIC
class Voice
{
public:
    
    int midiNote = 0;
    float filterEnvelopeAmount = 0.0f;

    Oscillator oscillator;

    bool active = false;
    bool noteHeld = false;

    juce::ADSR envelope;

    SimpleLP filter;
    FilterEnvelope filterEnvelope;
};

class MainComponent : public juce::AudioAppComponent,
                      public juce::MidiInputCallback
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;


    bool keyPressed(const juce::KeyPress& key) override;
    bool keyStateChanged(bool isKeyDown) override;

    void handleIncomingMidiMessage(
        juce::MidiInput* source,
        const juce::MidiMessage& message) override;
    

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    Voice voices[16];
    std::set<int> keysDown;

    void startVoice(int midiNote);
    void stopVoice(int midiNote);
    int keyToMidiNote(int keyCode);


    double deviceSampleRate = 44100.0;
    
    float volume = -12.0f;
    float pan = 0.0f;

    float startPhase = 0.0f;

    int unisonVoices = 1;
    float unisonDetune = 0.0f;
    float unisonMix = 0.0f;
    float phaseRandom = 0.0f;
    float stereoSpread = 0.0f;

    float filterCutoff = 10000.0f;
    float filterEnvAmount = 3000.0f;

    bool polyMode = true;

    juce::ToggleButton polyModeButton;

    juce::MidiKeyboardState midiKeyboardState;
    std::unique_ptr<juce::MidiInput> midiInput;

    juce::ADSR::Parameters adsrParameters;

    juce::Slider frequencySlider;
    juce::Slider volumeSlider;
    juce::Slider panSlider;
    juce::Label frequencyLabel;
    juce::Label volumeLabel;
    juce::ComboBox waveformBox;
    


    //filter
    juce::ComboBox poleBox;

    juce::Slider cutoffSlider;
    juce::Label cutoffLabel;

    juce::Slider resonanceSlider;
    juce::Label resonanceLabel;
   
    juce::Slider filterEnvAmountSlider;
    juce::Label filterEnvAmountLabel;

    //adsr
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;

    juce::Slider filterAttackSlider;
    juce::Slider filterDecaySlider;
    juce::Slider filterSustainSlider;
    juce::Slider filterReleaseSlider;
    juce::Label filterAttackLabel;
    juce::Label filterDecayLabel;
    juce::Label filterSustainLabel;
    juce::Label filterReleaseLabel;

    juce::Slider filterAttackCurveSlider;
    juce::Slider filterDecayCurveSlider;
    juce::Slider filterReleaseCurveSlider;
    juce::Label filterAttackCurveLabel;
    juce::Label filterDecayCurveLabel;
    juce::Label filterReleaseCurveLabel;

    //Unison
    juce::ComboBox unisonVoicesBox;

    juce::Slider unisonDetuneSlider;
    juce::Slider unisonMixSlider;
    juce::Slider phaseRandomSlider;
    juce::Slider stereoSpreadSlider;

    juce::Label unisonDetuneLabel;
    juce::Label unisonMixLabel;
    juce::Label phaseRandomLabel;
    juce::Label stereoSpreadLabel;
    
    
    

    juce::Random random;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
