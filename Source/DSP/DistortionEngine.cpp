#include "DistortionEngine.h"
#include <cmath>

DistortionEngine::DistortionEngine() {}

void DistortionEngine::prepare(const juce::dsp::ProcessSpec& spec) {
    sampleRate = static_cast<float>(spec.sampleRate);
    toneFilter.prepare(spec);
}

void DistortionEngine::setMode(Mode mode) {
    currentMode = mode;
}

void DistortionEngine::setDrive(float value) {
    drive = 1.0f + (value * 50.0f); // Range 1x to 51x
}

void DistortionEngine::setOutput(float value) {
    outputGain = value;
}

void DistortionEngine::setTone(float value) {
    toneFreq = 100.0f + (value * 10000.0f);
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, toneFreq);
    toneFilter.coefficients = coeffs;
}

void DistortionEngine::setSymmetry(float value) {
    symmetry = value;
}

void DistortionEngine::setMix(float value) {
    mix = value;
}

inline float DistortionEngine::softClip(float x) {
    // Tanh approximation
    return std::tanh(x);
}

inline float DistortionEngine::hardClip(float x) {
    return jlimit(-1.0f, 1.0f, x);
}

inline float DistortionEngine::waveFold(float x) {
    // Simple folding
    if (x > 1.0f) return 2.0f - x;
    if (x < -1.0f) return -2.0f - x;
    return x;
}

void DistortionEngine::process(juce::dsp::AudioBlock<float>& block) {
    for (int channel = 0; channel < block.getNumChannels(); ++channel) {
        auto* data = block.getChannelPointer(channel);
        
        for (int i = 0; i < block.getNumSamples(); ++i) {
            float dry = data[i];
            float wet = dry * drive;
            
            // Apply Symmetry bias
            wet += symmetry * 0.2f; 

            switch (currentMode) {
                case Mode::SoftClip:
                    wet = softClip(wet);
                    break;
                case Mode::HardClip:
                    wet = hardClip(wet);
                    break;
                case Mode::WaveFold:
                    wet = waveFold(wet);
                    break;
            }
            
            wet = toneFilter.processSample(channel, wet);
            wet *= outputGain;
            
            // Crossfade
            data[i] = (dry * (1.0f - mix)) + (wet * mix);
        }
    }
}
