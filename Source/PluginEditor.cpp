#include "PluginEditor.h"

WaveringDistortionModAudioProcessorEditor::WaveringDistortionModAudioProcessorEditor (WaveringDistortionModAudioProcessor& p)
    : AudioProcessorEditor (&p)
{
    setLookAndFeel(&lookAndFeel);
    
    // Title
    titleLabel.setText("WAVERING DISTORTION MOD", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Sliders (Examples)
    pitchSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(pitchSlider);
    
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(driveSlider);
    
    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(rateSlider);
    
    // Routing
    routingCombo.addItem("Serial", 1);
    routingCombo.addItem("Alternative", 2);
    routingCombo.addItem("Parallel", 3);
    addAndMakeVisible(routingCombo);
    
    // Bypass
    bypassButton.setButtonText("BYPASS");
    bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    addAndMakeVisible(bypassButton);
    
    setSize (600, 400);
}

WaveringDistortionModAudioProcessorEditor::~WaveringDistortionModAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void WaveringDistortionModAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(0xff1a1a1a)); // Dark metallic
    
    // Chassis effect
    g.setGradientFill(juce::ColourGradient(juce::Colours::darkgrey, 0, 0, juce::Colours::black, 0, getHeight(), false));
    g.fillRect(getLocalBounds());
    
    // Border
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 2);
}

void WaveringDistortionModAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    titleLabel.setBounds(area.removeFromTop(30));
    
    auto sliderArea = area.removeFromTop(150);
    pitchSlider.setBounds(sliderArea.removeFromLeft(100));
    driveSlider.setBounds(sliderArea.removeFromLeft(100));
    rateSlider.setBounds(sliderArea.removeFromLeft(100));
    
    routingCombo.setBounds(area.removeFromTop(30));
    bypassButton.setBounds(area.removeFromBottom(40));
}
