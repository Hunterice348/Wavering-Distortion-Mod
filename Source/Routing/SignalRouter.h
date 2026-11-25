#pragma once
#include "../DSP/WaveguideResonator.h"
#include "../DSP/DistortionEngine.h"
#include "../DSP/RingModulator.h"
#include "../DSP/ModulationEngine.h"

class SignalRouter {
public:
    enum class RoutingMode { Serial, Alternative, Parallel };

    SignalRouter();
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);
    
    void setRoutingMode(RoutingMode mode);
    void setMasterMix(float mix);
    void setMasterOutput(float gain);

    // Accessors to modules for parameter setting
    WaveguideResonator& getWaveguide() { return waveguide; }
    DistortionEngine& getDistortion() { return distortion; }
    RingModulator& getRingMod() { return ringMod; }
    ModulationEngine& getModulation() { return modulation; }

private:
    WaveguideResonator waveguide;
    DistortionEngine distortion;
    RingModulator ringMod;
    ModulationEngine modulation;
    
    RoutingMode currentMode = RoutingMode::Serial;
    float masterMix = 1.0f;
    float masterGain = 0.8f;
    
    juce::AudioBuffer<float> tempBuffer;
};
