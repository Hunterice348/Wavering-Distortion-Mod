#pragma once
#include <JuceHeader.h>
#include "WaveguideResonator.h"
#include "DistortionEngine.h"
#include "RingModulator.h"
#include "ModulationEngine.h"
#include "SignalRouter.h"
#include "PresetManager.h"

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
    double getTailLengthSeconds() const override { return 8.0; }
    int getNumPrograms() override { return PresetManager::count(); }
    int getCurrentProgram() override { return currentPreset; }
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;
    juce::AudioProcessorValueTreeState parameters;
    float getMeterInput() const noexcept { return meterInput.load(); }
    float getMeterOutput() const noexcept { return meterOutput.load(); }
    void loadPreset(int index);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
private:
    float value(const char* id) const noexcept;
    WaveguideResonator resonator;
    DistortionEngine distortion;
    RingModulator ringMod;
    ModulationEngine modulation;
    SignalRouter router;
    std::atomic<float> meterInput { 0.0f }, meterOutput { 0.0f };
    int currentPreset = 0;
    double currentSampleRate = 44100.0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveringDistortionModAudioProcessor)
};
