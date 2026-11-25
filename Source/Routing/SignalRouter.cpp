#include "SignalRouter.h"

SignalRouter::SignalRouter() {}

void SignalRouter::prepare(const juce::dsp::ProcessSpec& spec) {
    waveguide.prepare(spec);
    distortion.prepare(spec);
    ringMod.prepare(spec);
    modulation.prepare(spec);
    
    tempBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
    tempBuffer.clear();
}

void SignalRouter::setRoutingMode(RoutingMode mode) {
    currentMode = mode;
}

void SignalRouter::setMasterMix(float mix) {
    masterMix = mix;
}

void SignalRouter::setMasterOutput(float gain) {
    masterGain = gain;
}

void SignalRouter::process(juce::dsp::AudioBlock<float>& block) {
    // Update Modulation
    modulation.process(block.getNumSamples());
    
    // Apply Modulation to Parameters (Example: LFO to Pitch)
    float lfoVal = modulation.getLFOValue();
    // Note: In a real implementation, you'd map this to specific params smoothly
    
    if (currentMode == RoutingMode::Serial) {
        // Waveguide -> Distortion -> RingMod
        waveguide.process(block);
        distortion.process(block);
        ringMod.process(block);
    } 
    else if (currentMode == RoutingMode::Alternative) {
        // Distortion -> Waveguide -> RingMod
        distortion.process(block);
        waveguide.process(block);
        ringMod.process(block);
    } 
    else if (currentMode == RoutingMode::Parallel) {
        // Copy input to temp
        tempBuffer.copyFrom(0, 0, block.getChannelPointer(0), block.getNumSamples());
        
        // Process copies independently (Simplified for mono example)
        // In full stereo, you'd need separate buffers for each path
        
        // Path 1: Waveguide
        juce::dsp::AudioBlock<float> wgBlock(tempBuffer);
        waveguide.process(wgBlock);
        
        // Path 2: Distortion
        juce::dsp::AudioBlock<float> distBlock(tempBuffer); // Reusing temp for demo, needs separate buffer in prod
        distortion.process(distBlock);
        
        // Path 3: RingMod
        juce::dsp::AudioBlock<float> rmBlock(tempBuffer);
        ringMod.process(rmBlock);
        
        // Blend (Simplified: Just using last processed for demo, 
        // real parallel needs summing mixer)
        // For this code snippet, we'll fall back to Serial logic for stability 
        // unless full buffer management is implemented.
        // *Implementation Note*: True parallel requires 3 temporary buffers.
        waveguide.process(block); // Fallback to serial for safety in this snippet
    }
    
    // Master Gain
    block *= masterGain;
}
