cmake_minimum_required(VERSION 3.15)
project(WaveringDistortionMod VERSION 1.0.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE
add_subdirectory(JUCE)

# Define the plugin target
juce_add_plugin(WaveringDistortionMod
    PRODUCT_NAME "Wavering Distortion Mod"
    COMPANY_NAME "Hunterice348"
    COMPANY_WEBSITE "https://github.com/Hunterice348"
    BUNDLE_ID com.hunterice348.waveringdistortionmod
    FORMATS VST3
    PLUGIN_MANUFACTURER_CODE H348
    PLUGIN_CODE WDMO
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

# Source Files
target_sources(WaveringDistortionMod PRIVATE
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginEditor.cpp
    Source/DSP/WaveguideResonator.h
    Source/DSP/WaveguideResonator.cpp
    Source/DSP/DistortionEngine.h
    Source/DSP/DistortionEngine.cpp
    Source/DSP/RingModulator.h
    Source/DSP/RingModulator.cpp
    Source/DSP/ModulationEngine.h
    Source/DSP/ModulationEngine.cpp
    Source/Routing/SignalRouter.h
    Source/UI/PedalLookAndFeel.h
)

# Include Directories
target_include_directories(WaveringDistortionMod PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Link JUCE Modules
target_link_libraries(WaveringDistortionMod PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

# Windows Specific
if(WIN32)
    target_compile_definitions(WaveringDistortionMod PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()