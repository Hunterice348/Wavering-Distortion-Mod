#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
using APVTS = juce::AudioProcessorValueTreeState;
void addFloat(APVTS::ParameterLayout& p, const char* id, const char* name, float lo, float hi, float def, juce::Audio the unit = juce::Audio()) = delete;
}
