cmake_minimum_required(VERSION 3.22)
project(WaveringDistortionMod VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.10
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(juce)

juce_add_plugin(WaveringDistortionMod
    COMPANY_NAME "Hunterice348"
    BUNDLE_ID "com.hunterice348.waveringdistortionmod"
    PLUGIN_MANUFACTURER_CODE H348
    PLUGIN_CODE WdM1
    FORMATS VST3
    PRODUCT_NAME "WAVERING DISTORTION MOD"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)

target_sources(WaveringDistortionMod PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/WaveguideResonator.cpp
    Source/DistortionEngine.cpp
    Source/RingModulator.cpp
    Source/ModulationEngine.cpp
    Source/SignalRouter.cpp
    Source/PresetManager.cpp
    Source/DSPUtilities.cpp)

target_include_directories(WaveringDistortionMod PRIVATE Source)
target_compile_definitions(WaveringDistortionMod PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(WaveringDistortionMod PRIVATE
    juce::juce_audio_utils juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags)

juce_generate_juce_header(WaveringDistortionMod)

enable_testing()
add_executable(DSPBasicTests tests/DSPBasicTests.cpp
    Source/WaveguideResonator.cpp Source/DistortionEngine.cpp
    Source/RingModulator.cpp Source/ModulationEngine.cpp Source/DSPUtilities.cpp)
target_include_directories(DSPBasicTests PRIVATE Source)
target_link_libraries(DSPBasicTests PRIVATE juce::juce_dsp juce::juce_core)
add_test(NAME DSPBasicTests COMMAND DSPBasicTests)
