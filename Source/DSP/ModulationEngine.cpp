#include "ModulationEngine.h"

ModulationEngine::ModulationEngine() {
    lfo.initialise([](float x) { return std::sin(x); }, 128);
}

void ModulationEngine::prepare(const juce::dsp::ProcessSpec& spec) {
    lfo.prepare(spec);
    envelopeFollower.prepare(spec);
}

void ModulationEngine::process(int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        currentLfoValue = lfo.processSample(0);
        // Envelope follower would process audio input here, simplified for now
        currentEnvValue = std::abs(currentLfoValue); 
    }
}

float ModulationEngine::getLFOValue() {
    return currentLfoValue * lfoDepth;
}

float ModulationEngine::getEnvelopeValue() {
    return currentEnvValue;
}

void ModulationEngine::setLfoRate(float rate) {
    lfo.setFrequency(rate);
}

void ModulationEngine::setLfoDepth(float depth) {
    lfoDepth = depth;
}

void ModulationEngine::setSource(int src) {
    source = src;
}
