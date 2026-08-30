#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    setAudioChannels(0, 2);

    //ADSR
    // amplitude Attack
    amplitudeAttackLabel.setText("Attack", juce::dontSendNotification);
    amplitudeAttackLabel.setJustificationType(juce::Justification::centred);
    amplitudeAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    amplitudeAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    amplitudeAttackSlider.setRange(0.001, 5.0, 0.001);
    amplitudeAttackSlider.setValue(0.01);

    // amplitude Decay
    amplitudeDecayLabel.setText("Decay", juce::dontSendNotification);
    amplitudeDecayLabel.setJustificationType(juce::Justification::centred);
    amplitudeDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    amplitudeDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    amplitudeDecaySlider.setRange(0.001, 5.0, 0.001);
    amplitudeDecaySlider.setValue(0.2);

    // amplitude Sustain
    amplitudeSustainLabel.setText("Sustain", juce::dontSendNotification);
    amplitudeSustainLabel.setJustificationType(juce::Justification::centred);
    amplitudeSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    amplitudeSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    amplitudeSustainSlider.setRange(0.0, 1.0, 0.01);
    amplitudeSustainSlider.setValue(0.5);

    // amplitude Release
    amplitudeReleaseLabel.setText("Release", juce::dontSendNotification);
    amplitudeReleaseLabel.setJustificationType(juce::Justification::centred);
    amplitudeReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    amplitudeReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    amplitudeReleaseSlider.setRange(0.001, 5.0, 0.001);
    amplitudeReleaseSlider.setValue(0.2);

    // amplitude Attack Curve
    amplitudeAttackCurveLabel.setText("Attack Curve", juce::dontSendNotification);
    amplitudeAttackCurveLabel.setJustificationType(juce::Justification::centred);
    amplitudeAttackCurveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    amplitudeAttackCurveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    amplitudeAttackCurveSlider.setRange(-1.0, 1.0, 0.01);
    amplitudeAttackCurveSlider.setValue(0.0);

    // amplitude Decay Curve
    amplitudeDecayCurveLabel.setText("Decay Curve", juce::dontSendNotification);
    amplitudeDecayCurveLabel.setJustificationType(juce::Justification::centred);
    amplitudeDecayCurveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    amplitudeDecayCurveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    amplitudeDecayCurveSlider.setRange(-1.0, 1.0, 0.01);
    amplitudeDecayCurveSlider.setValue(0.0);


    // amplitude Release Curve
    amplitudeReleaseCurveLabel.setText("Release Curve", juce::dontSendNotification);
    amplitudeReleaseCurveLabel.setJustificationType(juce::Justification::centred);
    amplitudeReleaseCurveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    amplitudeReleaseCurveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    amplitudeReleaseCurveSlider.setRange(-1.0, 1.0, 0.01);
    amplitudeReleaseCurveSlider.setValue(0.0);


    addAndMakeVisible(amplitudeAttackCurveSlider);
    addAndMakeVisible(amplitudeDecayCurveSlider);
    addAndMakeVisible(amplitudeReleaseCurveSlider);
    addAndMakeVisible(amplitudeAttackCurveLabel);
    addAndMakeVisible(amplitudeDecayCurveLabel);
    addAndMakeVisible(amplitudeReleaseCurveLabel);

    addAndMakeVisible(amplitudeAttackSlider);
    addAndMakeVisible(amplitudeDecaySlider);
    addAndMakeVisible(amplitudeSustainSlider);
    addAndMakeVisible(amplitudeReleaseSlider);
    addAndMakeVisible(amplitudeAttackLabel);
    addAndMakeVisible(amplitudeDecayLabel);
    addAndMakeVisible(amplitudeSustainLabel);
    addAndMakeVisible(amplitudeReleaseLabel);

    amplitudeAttackSlider.onValueChange = [this]
        {
            float value = (float)amplitudeAttackSlider.getValue();

            for (auto& voice : voices)
                voice.amplitudeEnvelope.setAttack(value);
        };


    amplitudeDecaySlider.onValueChange = [this]
        {
            float value = (float)amplitudeDecaySlider.getValue();

            for (auto& voice : voices)
                voice.amplitudeEnvelope.setDecay(value);
        };


    amplitudeSustainSlider.onValueChange = [this]
        {
            float value = (float)amplitudeSustainSlider.getValue();

            for (auto& voice : voices)
                voice.amplitudeEnvelope.setSustain(value);
        };


    amplitudeReleaseSlider.onValueChange = [this]
        {
            float value = (float)amplitudeReleaseSlider.getValue();

            for (auto& voice : voices)
                voice.amplitudeEnvelope.setRelease(value);
        };


    amplitudeAttackCurveSlider.onValueChange = [this]
        {
            float value = (float)amplitudeAttackCurveSlider.getValue();

            for (auto& voice : voices)
                voice.amplitudeEnvelope.setAttackCurve(value);
        };


    amplitudeDecayCurveSlider.onValueChange = [this]
        {
            float value = (float)amplitudeDecayCurveSlider.getValue();

            for (auto& voice : voices)
                voice.amplitudeEnvelope.setDecayCurve(value);
        };


    amplitudeReleaseCurveSlider.onValueChange = [this]
        {
            float value = (float)amplitudeReleaseCurveSlider.getValue();

            for (auto& voice : voices)
                voice.amplitudeEnvelope.setReleaseCurve(value);
        };

    //ADSR
    // 
    //Filter
    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    cutoffSlider.setRange(20.0, 20000.0, 1.0);
    cutoffSlider.setValue(filterCutoff);
    cutoffSlider.setSkewFactorFromMidPoint(1000.0);
    cutoffSlider.setTextValueSuffix(" Hz");
    cutoffLabel.setText("Cutoff", juce::dontSendNotification);
    cutoffLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(cutoffSlider);
    addAndMakeVisible(cutoffLabel);

    cutoffSlider.onValueChange = [this]
        {
            float filterCutoff = (float)cutoffSlider.getValue();

            for (auto& voice : voices)
            {
                voice.filter.setCutoff(filterCutoff);
            }
        };

    /*poleBox.addItem("6 dB / octave", 1);
    poleBox.addItem("12 dB / octave", 2);
    poleBox.addItem("18 dB / octave", 3);
    poleBox.addItem("24 dB / octave", 4);

    poleBox.setSelectedId(1);

    poleBox.onChange = [this]
        {
            int selectedPoles = poleBox.getSelectedId();

            for (auto& voice : voices)
            {
                voice.filter.setPoles(selectedPoles);
            }
        };

    addAndMakeVisible(poleBox);*/

    resonanceLabel.setText("Resonance", juce::dontSendNotification);
    resonanceLabel.setJustificationType(juce::Justification::centred);

    resonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    resonanceSlider.setRange(0.0, 1.0, 0.01);
    resonanceSlider.setValue(0.0);

    resonanceSlider.onValueChange = [this]
        {
            float resonance = (float)resonanceSlider.getValue();

            for (auto& voice : voices)
            {
                voice.filter.setResonance(resonance);
            }
        };

    addAndMakeVisible(resonanceSlider);
    addAndMakeVisible(resonanceLabel);

    // Filter ADSR Amount
    filterEnvAmountLabel.setText("Filter Env Amount", juce::dontSendNotification);
    filterEnvAmountLabel.setJustificationType(juce::Justification::centred);

    filterEnvAmountSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterEnvAmountSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    filterEnvAmountSlider.setRange(0.0, 10000.0, 1.0);
    filterEnvAmountSlider.setValue(3000.0);

    addAndMakeVisible(filterEnvAmountSlider);
    addAndMakeVisible(filterEnvAmountLabel);

    // Filter Attack
    filterAttackSlider.setSliderStyle(juce::Slider::LinearVertical);
    filterAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    filterAttackSlider.setRange(0.001, 5.0, 0.001);
    filterAttackSlider.setValue(0.01);

    // Filter Decay
    filterDecaySlider.setSliderStyle(juce::Slider::LinearVertical);
    filterDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    filterDecaySlider.setRange(0.001, 5.0, 0.001);
    filterDecaySlider.setValue(0.2);

    // Filter Sustain
    filterSustainSlider.setSliderStyle(juce::Slider::LinearVertical);
    filterSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    filterSustainSlider.setRange(0.0, 1.0, 0.01);
    filterSustainSlider.setValue(0.5);

    // Filter Release
    filterReleaseSlider.setSliderStyle(juce::Slider::LinearVertical);
    filterReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    filterReleaseSlider.setRange(0.001, 5.0, 0.001);
    filterReleaseSlider.setValue(0.2);

    // Filter Attack Curve
    filterAttackCurveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterAttackCurveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    filterAttackCurveSlider.setRange(-1.0, 1.0, 0.01);
    filterAttackCurveSlider.setValue(0.0);

    // Filter Decay Curve
    filterDecayCurveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterDecayCurveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    filterDecayCurveSlider.setRange(-1.0, 1.0, 0.01);
    filterDecayCurveSlider.setValue(0.0);

   
    // Filter Release Curve
    filterReleaseCurveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterReleaseCurveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    filterReleaseCurveSlider.setRange(-1.0, 1.0, 0.01);
    filterReleaseCurveSlider.setValue(0.0);


    addAndMakeVisible(filterAttackCurveSlider);
    addAndMakeVisible(filterDecayCurveSlider);
    addAndMakeVisible(filterReleaseCurveSlider);

    addAndMakeVisible(filterAttackSlider);
    addAndMakeVisible(filterDecaySlider);
    addAndMakeVisible(filterSustainSlider);
    addAndMakeVisible(filterReleaseSlider);


    filterAttackSlider.onValueChange = [this]
        {
            float value = (float)filterAttackSlider.getValue();

            for (auto& voice : voices)
                voice.filterEnvelope.setAttack(value);
        };


    filterDecaySlider.onValueChange = [this]
        {
            float value = (float)filterDecaySlider.getValue();

            for (auto& voice : voices)
                voice.filterEnvelope.setDecay(value);
        };


    filterSustainSlider.onValueChange = [this]
        {
            float value = (float)filterSustainSlider.getValue();

            for (auto& voice : voices)
                voice.filterEnvelope.setSustain(value);
        };


    filterReleaseSlider.onValueChange = [this]
        {
            float value = (float)filterReleaseSlider.getValue();

            for (auto& voice : voices)
                voice.filterEnvelope.setRelease(value);
        };

    filterEnvAmountSlider.onValueChange = [this]
        {
            float value = (float)filterEnvAmountSlider.getValue();

            filterEnvAmount = value;
        };

    filterAttackCurveSlider.onValueChange = [this]
        {
            float value = (float)filterAttackCurveSlider.getValue();

            for (auto& voice : voices)
                voice.filterEnvelope.setAttackCurve(value);
        };


    filterDecayCurveSlider.onValueChange = [this]
        {
            float value = (float)filterDecayCurveSlider.getValue();

            for (auto& voice : voices)
                voice.filterEnvelope.setDecayCurve(value);
        };


    filterReleaseCurveSlider.onValueChange = [this]
        {
            float value = (float)filterReleaseCurveSlider.getValue();

            for (auto& voice : voices)
                voice.filterEnvelope.setReleaseCurve(value);
        };
    //FILTER
    // 
    //Frequency slider
    frequencySlider.setRange(50.0, 10000.0, 1.0);
    frequencySlider.setValue(440.0);
    frequencySlider.setTextValueSuffix(" Hz");
    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencySlider.onValueChange = [this]
        {
            //frequency = (float)frequencySlider.getValue();
        };
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(frequencyLabel);

    //Volume slider
    volumeSlider.setRange(-60.0, 0.0, 0.1);
    volumeSlider.setValue(-12.0);
    volumeSlider.setTextValueSuffix(" dB");
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeSlider.onValueChange = [this]
        {
            volume = (float)volumeSlider.getValue();
        };
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeLabel);

    //Waveform box
    waveformBox.addItem("Sine", 1);
    waveformBox.addItem("Saw", 2);
    waveformBox.addItem("Square", 3);
    waveformBox.addItem("Noise", 4);

    waveformBox.setSelectedId(1);
    waveformBox.onChange = [this]
        {
            int selectedWaveform =
                waveformBox.getSelectedId();

            for (auto& voice : voices)
            {
                voice.oscillator.setWaveform(selectedWaveform);
            }
        };
    addAndMakeVisible(waveformBox);

    // Octave box
    octaveBox.addItem("+4", 9);
    octaveBox.addItem("+3", 8);
    octaveBox.addItem("+2", 7);
    octaveBox.addItem("+1", 6);
    octaveBox.addItem("0", 5);
    octaveBox.addItem("-1", 4);
    octaveBox.addItem("-2", 3);
    octaveBox.addItem("-3", 2);
    octaveBox.addItem("-4", 1);

    octaveBox.setSelectedId(5);

    octaveBox.onChange = [this]
        {
            int selectedOctave = octaveBox.getSelectedId() - 5;;

            for (auto& voice : voices)
            {
                voice.oscillator.setOctave(selectedOctave);
            }
        };

    addAndMakeVisible(octaveBox);

    //envelope selector box
    envelopeSelectorBox.addItem("Amplitude", 1);
    envelopeSelectorBox.addItem("Filter", 2);

    envelopeSelectorBox.onChange = [this]
        {
            int selectedEnvelope = envelopeSelectorBox.getSelectedId();

            switch (selectedEnvelope)
            {
            case 1:
                //Filter OFF
                filterAttackSlider.setVisible(false);
                filterDecaySlider.setVisible(false);
                filterSustainSlider.setVisible(false);
                filterReleaseSlider.setVisible(false);

                filterAttackCurveSlider.setVisible(false);
                filterDecayCurveSlider.setVisible(false);
                filterReleaseCurveSlider.setVisible(false);

                //Amplitude ON
                amplitudeAttackSlider.setVisible(true);
                amplitudeDecaySlider.setVisible(true);
                amplitudeSustainSlider.setVisible(true);
                amplitudeReleaseSlider.setVisible(true);

                amplitudeAttackCurveSlider.setVisible(true);
                amplitudeDecayCurveSlider.setVisible(true);
                amplitudeReleaseCurveSlider.setVisible(true);

                amplitudeAttackSlider.toFront(false);
                amplitudeDecaySlider.toFront(false);
                amplitudeSustainSlider.toFront(false);
                amplitudeReleaseSlider.toFront(false);

                amplitudeAttackCurveSlider.toFront(false);
                amplitudeDecayCurveSlider.toFront(false);
                amplitudeReleaseCurveSlider.toFront(false);
                break;

            case 2:
                //Amplitude OFF
                amplitudeAttackSlider.setVisible(false);
                amplitudeDecaySlider.setVisible(false);
                amplitudeSustainSlider.setVisible(false);
                amplitudeReleaseSlider.setVisible(false);

                amplitudeAttackCurveSlider.setVisible(false);
                amplitudeDecayCurveSlider.setVisible(false);
                amplitudeReleaseCurveSlider.setVisible(false);

                //filter ON
                filterAttackSlider.setVisible(true);
                filterDecaySlider.setVisible(true);
                filterSustainSlider.setVisible(true);
                filterReleaseSlider.setVisible(true);
 
                filterAttackCurveSlider.setVisible(true);
                filterDecayCurveSlider.setVisible(true);
                filterReleaseCurveSlider.setVisible(true);

                filterAttackSlider.toFront(false);
                filterDecaySlider.toFront(false);
                filterSustainSlider.toFront(false);
                filterReleaseSlider.toFront(false);

                filterAttackCurveSlider.toFront(false);
                filterDecayCurveSlider.toFront(false);
                filterReleaseCurveSlider.toFront(false);

                break;

            default:
                
                break;
            }
            repaint();
        };
    envelopeSelectorBox.setSelectedId(1);
    addAndMakeVisible(envelopeSelectorBox);

    // poly/mono button
    polyModeButton.setButtonText("Poly / Mono");
    polyModeButton.setToggleState(true, juce::dontSendNotification);

    polyModeButton.onClick = [this]
        {
            polyMode = polyModeButton.getToggleState();
        };

    addAndMakeVisible(polyModeButton);

    //Panning slider
    panSlider.setRange(-1.0, 1.0, 0.01);
    panSlider.setValue(pan);
    panSlider.setTextValueSuffix(" Pan");

    panSlider.onValueChange = [this]
        {
            pan = (float)panSlider.getValue();
        };

    addAndMakeVisible(panSlider);

    //Unison
    unisonVoicesBox.addItem("1", 1);
    unisonVoicesBox.addItem("2", 2);
    unisonVoicesBox.addItem("3", 3);
    unisonVoicesBox.addItem("4", 4);
    unisonVoicesBox.addItem("5", 5);
    unisonVoicesBox.addItem("6", 6);
    unisonVoicesBox.addItem("7", 7);
    unisonVoicesBox.addItem("8", 8);
    unisonVoicesBox.addItem("9", 9);
    unisonVoicesBox.addItem("10", 10);
    unisonVoicesBox.addItem("11", 11);
    unisonVoicesBox.addItem("12", 12);
    unisonVoicesBox.addItem("13", 13);
    unisonVoicesBox.addItem("14", 14);
    unisonVoicesBox.addItem("15", 15);
    unisonVoicesBox.addItem("16", 16);

    unisonVoicesBox.setSelectedId(1);

    addAndMakeVisible(unisonVoicesBox);

    unisonVoicesBox.onChange = [this]
        {
            unisonVoices =
                unisonVoicesBox.getSelectedId();

            for (auto& voice : voices)
            {
                voice.oscillator.setUnisonVoices(
                    unisonVoices
                );
            }
        };

    unisonDetuneSlider.setRange(0.0, 100.0, 0.1);
    unisonDetuneSlider.setValue(0.0);
    unisonDetuneSlider.setTextValueSuffix(" cents");
    unisonDetuneSlider.setSliderStyle(juce::Slider::LinearVertical);

    addAndMakeVisible(unisonDetuneSlider);

    unisonDetuneSlider.onValueChange = [this]
        {
            unisonDetune =
                (float)unisonDetuneSlider.getValue();

            for (auto& voice : voices)
            {
                voice.oscillator.setDetune(
                    unisonDetune
                );
            }
        };

    unisonMixSlider.setRange(0.0, 1.0, 0.01);
    unisonMixSlider.setValue(0.0);
    unisonMixSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(unisonMixSlider);

    unisonMixSlider.onValueChange = [this]
        {
            unisonMix =
                (float)unisonMixSlider.getValue();

            for (auto& voice : voices)
            {
                voice.oscillator.setUnisonMix(
                    unisonMix
                );
            }
        };

    phaseRandomSlider.setRange(0.0, 1.0, 0.01);
    phaseRandomSlider.setValue(0.0);
    phaseRandomSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(phaseRandomSlider);

    phaseRandomSlider.onValueChange = [this]
        {
            phaseRandom = (float)phaseRandomSlider.getValue();

            for (auto& voice : voices)
            {
                voice.oscillator.setPhaseRandom(phaseRandom);
            }
        };

    stereoSpreadSlider.setRange(0.0, 1.0, 0.01);
    stereoSpreadSlider.setValue(0.0);
    stereoSpreadSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(stereoSpreadSlider);

    stereoSpreadSlider.onValueChange = [this]
        {
            stereoSpread = (float)stereoSpreadSlider.getValue();

            for (auto& voice : voices)
            {
                voice.oscillator.setStereoSpread(stereoSpread);
            }
        };

    stereoSpreadLabel.setText("Stereo", juce::dontSendNotification);
    phaseRandomLabel.setJustificationType(juce::Justification::centred);

    unisonDetuneLabel.setText("Detune",juce::dontSendNotification);
    unisonDetuneLabel.setJustificationType(juce::Justification::centred);

    unisonMixLabel.setText("Mix",juce::dontSendNotification);
    unisonMixLabel.setJustificationType(juce::Justification::centred);

    phaseRandomLabel.setText("Phase",juce::dontSendNotification);
    phaseRandomLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(stereoSpreadLabel);
    addAndMakeVisible(unisonDetuneLabel);
    addAndMakeVisible(unisonMixLabel);
    addAndMakeVisible(phaseRandomLabel);

    setWantsKeyboardFocus(true);

    auto midiInputs = juce::MidiInput::getAvailableDevices();

    if (midiInputs.size() > 0)
    {
        midiInput = juce::MidiInput::openDevice(
            midiInputs[0].identifier,
            this
        );

        if (midiInput != nullptr)
            midiInput->start();
    }

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

void MainComponent::resized()
{

    frequencyLabel.setBounds(20, 20, 100, 30);
    frequencySlider.setBounds(120, 20, getWidth() - 140, 30);

    volumeLabel.setBounds(20, 70, 100, 30);
    volumeSlider.setBounds(120, 70, getWidth() - 140, 30);

    panSlider.setBounds(120, 120, getWidth() - 140, 30);

    waveformBox.setBounds(20, 150, 100, 30);

    octaveBox.setBounds(60, 150, 100, 30);

    envelopeSelectorBox.setBounds(100, 300, 100, 30);

    polyModeButton.setBounds(20, 200, 100, 30);

    // amp adsr
    amplitudeAttackLabel.setBounds(100, 350, 50, 20);
    amplitudeDecayLabel.setBounds(150, 350, 50, 20);
    amplitudeSustainLabel.setBounds(200, 350, 50, 20);
    amplitudeReleaseLabel.setBounds(250, 350, 50, 20);

    amplitudeAttackSlider.setBounds(100, 370, 40, 120);
    amplitudeDecaySlider.setBounds(150, 370, 40, 120);
    amplitudeSustainSlider.setBounds(200, 370, 40, 120);
    amplitudeReleaseSlider.setBounds(250, 370, 40, 120);

    amplitudeAttackCurveSlider.setBounds(100, 500, 40, 60);
    amplitudeDecayCurveSlider.setBounds(150, 500, 40, 60);
    amplitudeReleaseCurveSlider.setBounds(250, 500, 40, 60);

    cutoffLabel.setBounds(320, 350, 80, 20);
    cutoffSlider.setBounds(320, 350, 80, 200);
    //poleBox.setBounds(320, 370, 100, 30);
    resonanceLabel.setBounds(400, 350, 80, 20);
    resonanceSlider.setBounds(400, 350, 80, 200);

    filterEnvAmountLabel.setBounds(360, 200, 80, 20);
    filterEnvAmountSlider.setBounds(360, 200, 80, 200);

    filterAttackLabel.setBounds(100, 350, 50, 20);
    filterDecayLabel.setBounds(150, 350, 50, 20 );
    filterSustainLabel.setBounds(200, 350, 50, 20);
    filterReleaseLabel.setBounds(250, 350, 50, 20);

    filterAttackSlider.setBounds(100, 370, 40, 120);
    filterDecaySlider.setBounds(150, 370, 40, 120);
    filterSustainSlider.setBounds(200, 370, 40, 120);
    filterReleaseSlider.setBounds(250, 370, 40, 120);

    filterAttackCurveSlider.setBounds(100, 500, 40, 60);
    filterDecayCurveSlider.setBounds(150, 500, 40, 60);
    filterReleaseCurveSlider.setBounds(250, 500, 40, 60);

    unisonVoicesBox.setBounds(500, 150, 100, 30);

    unisonDetuneSlider.setBounds(500, 200, 40, 120);
    unisonMixSlider.setBounds(550, 200, 40, 120);
    phaseRandomSlider.setBounds(600, 200, 40, 120);
    stereoSpreadSlider.setBounds(650, 200, 40, 120);

    unisonDetuneLabel.setBounds(500, 180, 50, 20);
    unisonMixLabel.setBounds(550, 180, 50, 20);
    phaseRandomLabel.setBounds(600, 180, 50, 20);
    stereoSpreadLabel.setBounds(650, 180, 50, 20);

    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    deviceSampleRate = sampleRate;

    for (auto& voice : voices)
    {
        
        voice.amplitudeEnvelope.prepare(deviceSampleRate);
        voice.filterEnvelope.prepare(deviceSampleRate);
        voice.filter.prepare(deviceSampleRate);
        voice.oscillator.prepare(deviceSampleRate);

        voice.amplitudeEnvelope.setAttack((float)amplitudeAttackSlider.getValue());
        voice.amplitudeEnvelope.setDecay((float)amplitudeDecaySlider.getValue());
        voice.amplitudeEnvelope.setSustain((float)amplitudeSustainSlider.getValue());
        voice.amplitudeEnvelope.setRelease((float)amplitudeReleaseSlider.getValue());
        voice.amplitudeEnvelope.setAttackCurve((float)amplitudeAttackCurveSlider.getValue());
        voice.amplitudeEnvelope.setDecayCurve((float)amplitudeDecayCurveSlider.getValue());
        voice.amplitudeEnvelope.setReleaseCurve((float)amplitudeReleaseCurveSlider.getValue());

        voice.filter.setCutoff((float)cutoffSlider.getValue());
        voice.filter.setResonance((float)resonanceSlider.getValue());

        voice.filterEnvelope.setAttack((float)filterAttackSlider.getValue());
        voice.filterEnvelope.setDecay((float)filterDecaySlider.getValue());
        voice.filterEnvelope.setSustain((float)filterSustainSlider.getValue());
        voice.filterEnvelope.setRelease((float)filterReleaseSlider.getValue());
        voice.filterEnvelope.setAttackCurve((float)filterAttackCurveSlider.getValue());
        voice.filterEnvelope.setDecayCurve((float)filterDecayCurveSlider.getValue());
        voice.filterEnvelope.setReleaseCurve((float)filterReleaseCurveSlider.getValue());

        voice.oscillator.setUnisonVoices(unisonVoices);
        voice.oscillator.setDetune(unisonDetune);
        voice.oscillator.setUnisonMix(unisonMix);
        voice.oscillator.setPhaseRandom(phaseRandom);

        // voice.filter.setPoles(poleBox.getSelectedId());

        voice.filter.reset();


    }

        

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* buffer = bufferToFill.buffer;
    auto numSamples = bufferToFill.numSamples;

    float gain = juce::Decibels::decibelsToGain(volume);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float leftValue = 0.0f;
        float rightValue = 0.0f;

        // Generate one sample from every active voice
        for (auto& voice : voices)
        {
            if (!voice.active)
                continue;

            //oscilator
            float voiceLeft = 0.0f;
            float voiceRight = 0.0f;

            voice.oscillator.getNextSample(voiceLeft, voiceRight);

            //ADSR
            float amplitudeEnvelopeValue = voice.amplitudeEnvelope.getNextSample();

            voiceLeft *= amplitudeEnvelopeValue;
            voiceRight *= amplitudeEnvelopeValue;

            //FILTER
            float filterEnvelopeValue = voice.filterEnvelope.getNextSample();
            float baseCutoff = (float)cutoffSlider.getValue();
            float envelopeAmount = (float)filterEnvAmountSlider.getValue();

            float modulatedCutoff = baseCutoff + filterEnvelopeValue * envelopeAmount;
            voice.filter.setCutoff(modulatedCutoff);

            voiceLeft = voice.filter.processSample(voiceLeft,0);
            voiceRight = voice.filter.processSample(voiceRight,1);

            //MIX
            leftValue += voiceLeft; //
            rightValue += voiceRight;

            //deactivate voice after release
            if (!voice.amplitudeEnvelope.isActive())
            {
                voice.active = false;
                voice.noteHeld = false;
            }
        }

        // Apply master volume
        leftValue *= gain;
        rightValue *= gain;

        // Panning
        float panAngle =
            (pan + 1.0f) * 0.25f *
            juce::MathConstants<float>::pi;

        float leftGain = std::cos(panAngle);
        float rightGain = std::sin(panAngle);

        //limiter and safeguard
        if (!std::isfinite(leftValue) || !std::isfinite(rightValue))
        {
            leftValue = 0.0f;
            rightValue = 0.0f;
        }

        // Write to output channels
        if (buffer->getNumChannels() >= 2)
        {
            buffer->setSample(
                0,
                bufferToFill.startSample + sample,
                leftValue * leftGain
            );

            buffer->setSample(
                1,
                bufferToFill.startSample + sample,
                rightValue * rightGain
            );
        }
    }
}

void MainComponent::releaseResources()
{


    juce::Logger::getCurrentLogger()->writeToLog("Releasing audio resources");
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setFont(30.0f);
    g.setColour(juce::Colours::white);
    // You can add your drawing code here!
}

bool MainComponent::keyPressed(const juce::KeyPress& key)
{
    int keyCode = key.getKeyCode();

    if (keysDown.find(keyCode) != keysDown.end())
        return true;

    int midiNote = keyToMidiNote(keyCode);

    if (midiNote == -1)
        return false;

    keysDown.insert(keyCode);

    startVoice(midiNote);

    return true;
}

//Keyboard to MIDI
int MainComponent::keyToMidiNote(int keyCode)
{
    switch (keyCode)
    {
        // Lower octave - white keys
    case 'Z': return 60;
    case 'X': return 62;
    case 'C': return 64;
    case 'V': return 65;
    case 'B': return 67;
    case 'N': return 69;
    case 'M': return 71;
    case ',': return 72;
    case '.': return 74;

        // Lower octave - black keys
    case 'S': return 61;
    case 'D': return 63;
    case 'G': return 66;
    case 'H': return 68;
    case 'J': return 70;

        // Upper octave - white keys
    case 'Q': return 72;
    case 'W': return 74;
    case 'E': return 76;
    case 'R': return 77;
    case 'T': return 79;
    case 'Y': return 81;
    case 'U': return 83;
    case 'I': return 84;
    case 'O': return 86;

        // Upper octave - black keys
    case '2': return 73;
    case '3': return 75;
    case '5': return 78;
    case '6': return 80;
    case '7': return 82;
    case '9': return 85;

    default:
        return -1;
    }
}

bool MainComponent::keyStateChanged(bool isKeyDown)
{
    if (!isKeyDown)
    {
        for (auto it = keysDown.begin(); it != keysDown.end();)
        {
            int keyCode = *it;

            if (!juce::KeyPress::isKeyCurrentlyDown(keyCode))
            {
                int midiNote = keyToMidiNote(keyCode);

                if (midiNote != -1)
                    stopVoice(midiNote);

                it = keysDown.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    return true;
}

void MainComponent::startVoice(int midiNote)
{
    if (!polyMode)
    {
        Voice& voice = voices[0];

        voice.midiNote = midiNote;

        voice.oscillator.setFrequency((float)juce::MidiMessage::getMidiNoteInHertz(midiNote));

        voice.oscillator.setWaveform(waveformBox.getSelectedId());

        voice.oscillator.setUnisonVoices(unisonVoices);
        voice.oscillator.setDetune(unisonDetune);
        voice.oscillator.setUnisonMix(unisonMix);
        voice.oscillator.setPhaseRandom(phaseRandom);

        voice.oscillator.reset();

        //voice.filterEnvelope.setParameters(adsrParameters);
        voice.amplitudeEnvelope.noteOn();

        voice.filterEnvelope.noteOn();

        voice.noteHeld = true;
        voice.active = true;

        return;
    }

    Voice* voiceToUse = nullptr;

    // First priority: completely free voice
    for (auto& voice : voices)
    {
        if (!voice.active)
        {
            voiceToUse = &voice;
            break;
        }
    }

    // Second priority: steal a voice that is already releasing
    if (voiceToUse == nullptr)
    {
        for (auto& voice : voices)
        {
            if (!voice.noteHeld)
            {
                voiceToUse = &voice;
                break;
            }
        }
    }

    // Last resort: all voices are currently being held.
    // Steal the first one.
    if (voiceToUse == nullptr)
    {
        voiceToUse = &voices[0];
    }

    voiceToUse->midiNote = midiNote;

    voiceToUse->oscillator.setFrequency((float)juce::MidiMessage::getMidiNoteInHertz(midiNote));

    voiceToUse->oscillator.setWaveform(waveformBox.getSelectedId());

    voiceToUse->oscillator.setUnisonVoices(unisonVoices);
    voiceToUse->oscillator.setDetune(unisonDetune);
    voiceToUse->oscillator.setUnisonMix(unisonMix);
    voiceToUse->oscillator.setPhaseRandom(phaseRandom);

    voiceToUse->oscillator.reset();

    //voiceToUse->envelope.setParameters(adsrParameters);
    voiceToUse->amplitudeEnvelope.noteOn();

    voiceToUse->filterEnvelope.noteOn();

    voiceToUse->noteHeld = true;
    voiceToUse->active = true;
}

void MainComponent::stopVoice(int midiNote)
{
    for (auto& voice : voices)
    {
        if (voice.active &&
            voice.noteHeld &&
            voice.midiNote == midiNote)
        {
            voice.noteHeld = false;
            voice.amplitudeEnvelope.noteOff();
            voice.filterEnvelope.noteOff();

            return;
        }
    }
}

void MainComponent::handleIncomingMidiMessage(
    juce::MidiInput* source,
    const juce::MidiMessage& message)
{
    if (message.isNoteOn())
    {
        int midiNote = message.getNoteNumber();

        startVoice(midiNote);
    }
    else if (message.isNoteOff())
    {
        int midiNote = message.getNoteNumber();

        stopVoice(midiNote);
    }
}
