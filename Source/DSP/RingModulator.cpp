#include "RingModulator.h"

RingModulator::RingModulator() {
    carrier.initialise([](float x) { return std::sin(x); }, 128);
}

void RingModulator::prepare(const juce::dsp::ProcessSpec& spec) {
    sampleRate = static_cast<float>(spec.sampleRate);
    carrier.prepare(spec);
}

void RingModulator::setRate(float value) {
    carrier.setFrequency(value);
}

void RingModulator::setDepth(float value) {
    depth = value;
}

void RingModulator::setWaveform(int index) {
    waveformIndex = index;
    // In a real implementation, you would swap the lambda function here
    // For brevity, we stick to Sine or basic variations
    if (index == 0) carrier.initialise([](float x) { return std::sin(x); }, 128);
    else if (index == 1) carrier.initialise([](float x) { return std::abs(std::sin(x)) * 2.0f - 1.0f; }, 128); // Triangle approx
    else if (index == 2) carrier.initialise([](float x) { return (x / M_PI) - 1.0f; }, 128); // Saw approx
    else carrier.initialise([](float x) { return std::sin(x) > 0 ? 1.0f : -1.0f; }, 128); // Square
}

void RingModulator::setMix(float value) {
    mix = value;
}

void RingModulator::process(juce::dsp::AudioBlock<float>& block) {
    for (int channel = 0; channel < block.getNumChannels(); ++channel) {
        auto* data = block.getChannelPointer(channel);
        
        for (int i = 0; i < block.getNumSamples(); ++i) {
            float input = data[i];
            float carrierVal = carrier.processSample(channel);
            
            // Ring Mod: Input * Carrier
            float modulated = input * carrierVal;
            
            // Apply Depth (blend with dry)
            float wet = (input * (1.0f - depth)) + (modulated * depth);
            
            // Final Mix
            data[i] = (input * (1.0f - mix)) + (wet * mix);
        }
    }
}
