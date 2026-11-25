#pragma once
#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>

class ModulationEngine {
public:
    ModulationEngine();
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(int numSamples);
    
    float getLFOValue();
    float getEnvelopeValue();

    void setLfoRate(float rate);
    void setLfoDepth(float depth);
    void setSource(int source); // 0:LFO, 1:Env

private:
    juce::dsp::Oscillator<float> lfo;
    juce::dsp::BallisticsFilter<float> envelopeFollower;
    float currentLfoValue = 0.0f;
    float currentEnvValue = 0.0f;
    float lfoDepth = 0.0f;
    int source = 0;
};
