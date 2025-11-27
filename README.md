# Sonara Audio Plugin

Intelligent audio engineering plugin that transforms your sound based on text input.

## Features

- **Text-based audio processing**: Describe your desired sound in plain English
- **Intelligent keyword mapping**: Understands creative language and applies appropriate processing
- **Real-time feedback**: See exactly what changes were made to your audio
- **Professional audio processing**: Built-in EQ, compression, and reverb
- **DAW compatible**: Works with Logic Pro X, Ableton, and more

## Installation

### Quick Build (Recommended)

```bash
git clone --recursive https://github.com/yourusername/Sonara.git
cd Sonara
./build.sh
```

### Manual Build

1. Clone this repository:
```bash
git clone --recursive https://github.com/yourusername/Sonara.git
cd Sonara
```

2. Install JUCE via submodule (automatically done with --recursive):
```bash
git submodule update --init --recursive
```

3. Build the project:
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

4. Install the plugin:
   - **macOS (Logic Pro)**: Copy the `Sonara.component` to `~/Library/Audio/Plug-Ins/Components/`
   - **macOS (Other DAWs)**: Copy the `Sonara.vst3` to `~/Library/Audio/Plug-Ins/VST3/`
   - **Windows**: Copy the `.vst3` file to your VST3 plugins directory
   - **Linux**: Copy the `.so` file to your VST3 plugins directory

### Using Pre-built Binaries

Download the latest release from the releases page and install according to your platform.

## Usage

1. Open your DAW
2. Insert Sonara as a plugin on your track
3. Type your desired effect in plain English (e.g., "make it brighter", "add room", "more punch")
4. The plugin will apply appropriate processing and show you what changed
5. Adjust the intensity slider to control the effect strength

### Example Keywords

- **Brightness**: "brighter", "more air", "sparkle", "crisp"
- **Warmth**: "warmer", "smoother", "more body"
- **Reverb**: "add room", "more space", "hall", "ambience"
- **Compression**: "more punch", "tighter", "glue the mix"
- **Bass**: "more low end", "deeper bass"

## Architecture

- `PluginProcessor`: Main audio processing engine
- `KeywordMapper`: Maps text input to audio processing parameters
- `ChangesLogger`: Tracks and displays what changes were made
- `AudioProcessing/Equalizer`: Parametric EQ implementation
- `AudioProcessing/Compressor`: Compressor implementation
- `AudioProcessing/ReverbProcessor`: Reverb implementation

## Development

Built with JUCE 7.x and CMake. Compatible with modern C++17.


## Acknowledgments

Built with [JUCE](https://juce.com/) framework.

