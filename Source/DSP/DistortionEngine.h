#pragma once
#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>

class DistortionEngine {
public:
    DistortionEngine();
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);

    enum class Mode { SoftClip, HardClip, WaveFold };
    
    void setMode(Mode mode);
    void setDrive(float value); // 0-1
    void setOutput(float value); // 0-1
    void setTone(float value); // Lowpass cutoff
    void setSymmetry(float value); // -1 to 1
    void setMix(float value); // 0-1 Dry/Wet

private:
    float drive = 1.0f;
    float outputGain = 0.5f;
    float toneFreq = 5000.0f;
    float symmetry = 0.0f;
    float mix = 1.0f;
    Mode currentMode = Mode::SoftClip;
    
    juce::dsp::IIR::Filter<float> toneFilter;
    float sampleRate = 44100.0f;
    
    // Waveshaping functions
    inline float softClip(float x);
    inline float hardClip(float x);
    inline float waveFold(float x);
};
