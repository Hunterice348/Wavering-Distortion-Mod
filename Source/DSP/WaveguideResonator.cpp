#include "WaveguideResonator.h"

WaveguideResonator::WaveguideResonator() {
    delayLine.setMaximumDelayInSamples(44100); // 1 second max delay
}

void WaveguideResonator::prepare(const juce::dsp::ProcessSpec& spec) {
    sampleRate = static_cast<float>(spec.sampleRate);
    delayLine.prepare(spec);
    lowPassFilter.prepare(spec);
    highPassFilter.prepare(spec);
    reset();
}

void WaveguideResonator::reset() {
    delayLine.reset();
    lowPassFilter.reset();
    highPassFilter.reset();
}

void WaveguideResonator::setPitch(float value) {
    // Map 0-1 to 20Hz - 2000Hz logarithmic
    currentPitchHz = 20.0f * std::pow(100.0f, value);
    auto delaySamples = sampleRate / currentPitchHz;
    delayLine.setDelay(delaySamples);
}

void WaveguideResonator::setDecay(float value) {
    currentDecay = value;
}

void WaveguideResonator::setFeedback(float value) {
    currentFeedback = jlimit(0.0f, 0.99f, value);
}

void WaveguideResonator::setMaterial(int index) {
    currentMaterialIndex = jlimit(0, 3, index);
    auto& mat = materials[currentMaterialIndex];
    
    juce::dsp::IIR::Coefficients<float>::Ptr lpCoeffs = 
        juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, mat.lpFreq);
    lowPassFilter.coefficients = lpCoeffs;
    
    juce::dsp::IIR::Coefficients<float>::Ptr hpCoeffs = 
        juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, mat.hpFreq);
    highPassFilter.coefficients = hpCoeffs;
}

void WaveguideResonator::process(juce::dsp::AudioBlock<float>& block) {
    auto* channelData = block.getChannelPointer(0); // Process mono for simplicity in core, expand for stereo if needed
    
    for (int i = 0; i < block.getNumSamples(); ++i) {
        float input = channelData[i];
        
        // Feedback loop
        float delayed = delayLine.popSample(0);
        float feedbackSignal = delayed * currentFeedback;
        
        // Apply damping/decay
        feedbackSignal *= (1.0f - (currentDecay * 0.5f)); 
        
        // Mix input with feedback
        float wet = input + feedbackSignal;
        
        // Filter stage (Material simulation)
        wet = highPassFilter.processSample(0, wet);
        wet = lowPassFilter.processSample(0, wet);
        
        // Push back into delay line
        delayLine.pushSample(0, wet);
        
        // Output is the delayed signal (classic waveguide output)
        channelData[i] = delayed;
    }
}
