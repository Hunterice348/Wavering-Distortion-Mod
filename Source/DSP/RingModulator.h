#pragma once
#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>

class RingModulator {
public:
    RingModulator();
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::dsp::AudioBlock<float>& block);

    void setRate(float value); // Hz
    void setDepth(float value); // 0-1
    void setWaveform(int index); // 0:Sine, 1:Tri, 2:Saw, 3:Square
    void setMix(float value);

private:
    juce::dsp::Oscillator<float> carrier;
    float sampleRate = 44100.0f;
    float depth = 1.0f;
    float mix = 1.0f;
    int waveformIndex = 0;
};
