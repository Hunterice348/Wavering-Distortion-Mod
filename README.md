# WAVERING DISTORTION MOD

**Experimental hybrid effect pedal — Waveguide Resonator + Distortion + Ring Modulator**

A single JUCE-based VST3 effect plugin, intended for Windows x64 and compatible VST3 hosts. The DSP is split into focused modules and uses JUCE's `AudioProcessorValueTreeState` for automation and state persistence.

## Build (Windows x64)

Requirements: Visual Studio 2022 with the Desktop C++ workload, CMake 3.22+, Git, and an internet connection for the first configure (JUCE is fetched by CMake).

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The VST3 bundle is normally at `build/WaveringDistortionMod_artefacts/Release/VST3/WAVERING DISTORTION MOD.vst3`. Install it by copying the bundle to `C:\Program Files\Common Files\VST3\`, then rescan plugins in your DAW.

Other generators and operating systems can build the same VST3 target if the corresponding C++ toolchain is installed. No build has been run in this repository-hosted environment; please run the commands above to validate on your target machine.

## Current implementation

- Three active DSP modules: feedback-delay waveguide resonator, nonlinear distortion/wavefolding, and oscillator-based ring modulation.
- Serial, reordered serial, and parallel routing; global mix/output and module levels.
- Mono/stereo processing, smoothed host parameters, bounded feedback, modulation sources, and APVTS state save/restore.
- Factory preset selector and an interactive dark pedal-style editor with waveform scope and input/output level meters.
- `tests/DSPBasicTests.cpp` exercises basic finite-output and nonlinear-processing behavior.

The parameter tree exposes the specified module controls for DAW automation. DSP controls that are not yet represented by a full physical model or dedicated filter topology are currently mapped to practical approximations; this is an initial implementation, not a claim of hardware-validated modeling. Oversampling and tempo-synchronized modulation are not implemented yet.

## Repository structure

- `Source/PluginProcessor.*` — host integration, parameters, state, audio processing
- `Source/PluginEditor.*` — functional editor, controls, meters, waveform view
- `Source/WaveguideResonator.*`, `DistortionEngine.*`, `RingModulator.*` — effect engines
- `Source/ModulationEngine.*`, `SignalRouter.*`, `PresetManager.*`, `DSPUtilities.*` — supporting DSP and preset logic
- `tests/` — lightweight DSP checks

## License

MIT. See [LICENSE](LICENSE).
