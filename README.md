# Audiodescriptors
AudioDescriptors is a temporary project. It will be incorporated into a future version of [ControlGris](https://github.com/GRIS-UdeM/ControlGris).

AudioDescriptors is a plug-in that analyzes an audio signal and controls certain ControlGris parameters via OSC. The [Fluid Corpus Manipulation Library](https://github.com/flucoma/flucoma-core) is used for audio analysis.

Audio descriptors available:
- Loudness
- Pitch
- Centroid
- Spread
- Flatness (Noise)
- Onset Detection (Iterations Speed)

ControlGris parameters:
- Azimuth (Dome mode)
- Elevation (Dome mode)
- X position (Cube mode)
- Y position (Cube mode)
- Z position (Cube mode)
- Horizontal Span
- Vertical Span

Note that it is possible to use AudioDescriptors' Dome mode to control ControlGris in Cube mode.

## Building
#### 1. Install dependencies

Download and extract [Juce 7.0.12](https://github.com/juce-framework/JUCE/releases/tag/7.0.12).

All necessary libraries are included in the project. See the Distribution_licenses.md file at the root of this project for details.

#### 2. Generating project files

```bash
cd <AudioDescriptors-path>
<path-to-projucer> --resave AudioDescriptors.jucer
```

#### 3. Compiling

Go to the generated `Builds/` folder.

On Windows, use the Visual Studio 2022 solution file.

On MacOS, use the Xcode project. You will have to supply your own developer ID to XCode.

On Linux :

```bash
cd Builds/LinuxMakeFile
make CONFIG=Release CXX=clang++ -j 8
```
