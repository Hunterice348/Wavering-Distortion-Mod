#pragma once

#include <JuceHeader.h>

class WaveringDistortionModAudioProcessor final : public juce::AudioProcessor
{
public:
    WaveringDistortionModAudioProcessor();
    ~WaveringDistortionModAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 3; }
    int getCurrentProgram() override { return currentPreset; }
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destinationData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;
    float getMeterInput() const noexcept { return meterInput.load(); }
    float getMeterOutput() const noexcept { return meterOutput.load(); }

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    std::atomic<float> meterInput { 0.0f };
    std::atomic<float> meterOutput { 0.0f };
    juce::SmoothedValue<float> driveSmoother;
    juce::SmoothedValue<float> mixSmoother;
    juce::SmoothedValue<float> outputSmoother;
    int currentPreset = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveringDistortionModAudioProcessor)
};
