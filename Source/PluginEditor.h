#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/PedalLookAndFeel.h"

class WaveringDistortionModAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    WaveringDistortionModAudioProcessorEditor (WaveringDistortionModAudioProcessor&);
    ~WaveringDistortionModAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PedalLookAndFeel lookAndFeel;
    
    // UI Components
    juce::Label titleLabel;
    juce::Slider pitchSlider;
    juce::Slider driveSlider;
    juce::Slider rateSlider;
    juce::ComboBox routingCombo;
    juce::TextButton bypassButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveringDistortionModAudioProcessorEditor)
};
