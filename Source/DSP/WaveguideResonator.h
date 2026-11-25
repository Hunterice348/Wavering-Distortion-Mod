#pragma once
#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>

class WaveguideResonator {
public:
    WaveguideResonator();
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(juce::dsp::AudioBlock<float>& block);

    // Parameters
    void setPitch(float value); // 0.0 - 1.0 mapped to frequency
    void setDecay(float value); // 0.0 - 1.0
    void setFeedback(float value); // 0.0 - 0.99
    void setMaterial(int index); // 0:Wood, 1:Metal, 2:Glass, 3:String
    void setDispersion(float value);
    void setDamping(float value);

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayLine;
    juce::dsp::IIR::Filter<float> lowPassFilter;
    juce::dsp::IIR::Filter<float> highPassFilter;
    
    float sampleRate = 44100.0f;
    float currentPitchHz = 440.0f;
    float currentFeedback = 0.5f;
    float currentDecay = 0.5f;
    
    // Material coefficients (simplified EQ curves)
    struct MaterialProps {
        float lpFreq;
        float hpFreq;
        float dispersionFactor;
    };
    std::vector<MaterialProps> materials = {
        {2000.0f, 100.0f, 0.1f}, // Wood
        {8000.0f, 200.0f, 0.8f}, // Metal
        {12000.0f, 500.0f, 0.9f}, // Glass
        {5000.0f, 80.0f, 0.3f}   // String
    };
    int currentMaterialIndex = 0;
};
