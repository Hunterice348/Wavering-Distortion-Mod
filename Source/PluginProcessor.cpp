#include "PluginProcessor.h"

namespace
{
using APVTS = juce::AudioProcessorValueTreeState;

constexpr std::array<std::array<float, 3>, 3> presetValues {{
    {{ 6.0f, 0.65f, 0.0f }},
    {{ 18.0f, 0.8f, -2.0f }},
    {{ 28.0f, 1.0f, -6.0f }}
}};

constexpr std::array<const char*, 3> presetNames {{
    "Warm", "Driven", "Saturated"
}};

void setParameter(APVTS& state, const juce::String& id, float value)
{
    if (auto* parameter = state.getParameter(id))
        parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
}
}

WaveringDistortionModAudioProcessor::WaveringDistortionModAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "WaveringDistortionMod", createParameterLayout())
{
}

APVTS::ParameterLayout WaveringDistortionModAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "drive", 1 }, "Drive",
        juce::NormalisableRange<float> { 0.0f, 30.0f, 0.1f }, 6.0f, "dB"));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "mix", 1 }, "Mix",
        juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.65f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "output", 1 }, "Output",
        juce::NormalisableRange<float> { -24.0f, 12.0f, 0.1f }, 0.0f, "dB"));
    return layout;
}

void WaveringDistortionModAudioProcessor::prepareToPlay(double sampleRate, int)
{
    const auto smoothingTime = juce::jmax(1.0, sampleRate) * 0.02;
    driveSmoother.reset(sampleRate, 0.02);
    mixSmoother.reset(sampleRate, 0.02);
    outputSmoother.reset(sampleRate, 0.02);

    driveSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(
        parameters.getRawParameterValue("drive")->load()));
    mixSmoother.setCurrentAndTargetValue(parameters.getRawParameterValue("mix")->load());
    outputSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(
        parameters.getRawParameterValue("output")->load()));
    juce::ignoreUnused(smoothingTime);
}

void WaveringDistortionModAudioProcessor::releaseResources()
{
}

bool WaveringDistortionModAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto output = layouts.getMainOutputChannelSet();
    if (output != juce::AudioChannelSet::mono() && output != juce::AudioChannelSet::stereo())
        return false;

    return output == layouts.getMainInputChannelSet();
}

void WaveringDistortionModAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                        juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused(midiMessages);

    const auto inputChannels = getTotalNumInputChannels();
    const auto outputChannels = getTotalNumOutputChannels();
    for (auto channel = inputChannels; channel < outputChannels; ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());

    driveSmoother.setTargetValue(juce::Decibels::decibelsToGain(
        parameters.getRawParameterValue("drive")->load()));
    mixSmoother.setTargetValue(parameters.getRawParameterValue("mix")->load());
    outputSmoother.setTargetValue(juce::Decibels::decibelsToGain(
        parameters.getRawParameterValue("output")->load()));

    float inputPeak = 0.0f;
    float outputPeak = 0.0f;
    const auto samples = buffer.getNumSamples();

    for (auto channel = 0; channel < inputChannels; ++channel)
    {
        auto* audio = buffer.getWritePointer(channel);
        for (auto sample = 0; sample < samples; ++sample)
        {
            const auto dry = audio[sample];
            inputPeak = juce::jmax(inputPeak, std::abs(dry));

            const auto drive = driveSmoother.getNextValue();
            const auto mix = mixSmoother.getNextValue();
            const auto output = outputSmoother.getNextValue();
            const auto wet = std::tanh(dry * drive);
            audio[sample] = ((dry * (1.0f - mix)) + (wet * mix)) * output;
            outputPeak = juce::jmax(outputPeak, std::abs(audio[sample]));
        }
    }

    meterInput.store(inputPeak);
    meterOutput.store(outputPeak);
}

juce::AudioProcessorEditor* WaveringDistortionModAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

void WaveringDistortionModAudioProcessor::setCurrentProgram(int index)
{
    currentPreset = juce::jlimit(0, getNumPrograms() - 1, index);
    const auto& preset = presetValues[static_cast<std::size_t>(currentPreset)];
    setParameter(parameters, "drive", preset[0]);
    setParameter(parameters, "mix", preset[1]);
    setParameter(parameters, "output", preset[2]);
}

const juce::String WaveringDistortionModAudioProcessor::getProgramName(int index)
{
    return presetNames[static_cast<std::size_t>(juce::jlimit(0, getNumPrograms() - 1, index))];
}

void WaveringDistortionModAudioProcessor::getStateInformation(juce::MemoryBlock& destinationData)
{
    if (const auto state = parameters.copyState(); auto xml = state.createXml())
        copyXmlToBinary(*xml, destinationData);
}

void WaveringDistortionModAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
    {
        if (xml->hasTagName(parameters.state.get რომ()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveringDistortionModAudioProcessor();
}
