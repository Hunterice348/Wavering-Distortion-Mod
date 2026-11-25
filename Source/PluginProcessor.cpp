#include "PluginProcessor.h"
#include "PluginEditor.h"

WaveringDistortionModAudioProcessor::WaveringDistortionModAudioProcessor()
#ifndef JucePluginCode
 : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                   )
#endif
{
    // Parameter setup would go here using apvts
    // For brevity, assuming default construction
}

WaveringDistortionModAudioProcessor::~WaveringDistortionModAudioProcessor()
{
}

void WaveringDistortionModAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());
    
    router.prepare(spec);
}

void WaveringDistortionModAudioProcessor::releaseResources()
{
}

void WaveringDistortionModAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::dsp::AudioBlock<float> block (buffer);
    router.process(block);
}

juce::AudioProcessorEditor* WaveringDistortionModAudioProcessor::createEditor()
{
    return new WaveringDistortionModAudioProcessorEditor (*this);
}

void WaveringDistortionModAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Serialize parameters
}

void WaveringDistortionModAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Deserialize parameters
}