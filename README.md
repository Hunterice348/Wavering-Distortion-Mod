# WAVERING DISTORTION MOD

**Experimental JUCE VST3 distortion effect**

A small JUCE-based VST3 audio effect for Windows x64 and other platforms supported by JUCE. The current bootstrap provides a smoothed soft-clipping drive stage, dry/wet blend, output level, three factory presets, host automation/state persistence, and a generic parameter editor. The original modular DSP/editor files described in earlier plans are not present yet and are not included in this build.

## Build (Windows x64)

Requirements: Visual Studio 2022 with the Desktop C++ workload, CMake 3.22+, Git, and an internet connection for the first configure (JUCE is fetched by CMake).

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

The VST3 bundle is normally at `build/WaveringDistortionMod_artefacts/Release/VST3/WAVERING DISTORTION MOD.vst3`. Install it by copying the bundle to `C:\Program Files\Common Files\VST3\`, then rescan plugins in your DAW.

Other generators and operating systems can build the same VST3 target if the corresponding C++ toolchain is installed. A build has not been run in this repository-hosted environment; please run the commands above in a configured C++ environment.

## Parameters

- **Drive:** input gain before the soft clipper (0–30 dB).
- **Mix:** dry/wet blend (0–100%).
- **Output:** post-effect level (-24–+12 dB).

All three parameters are exposed to host automation and saved in the plugin state. The generic JUCE editor displays the parameter controls; dedicated metering and custom pedal UI are not implemented yet.

## License

MIT. See [LICENSE](LICENSE).
