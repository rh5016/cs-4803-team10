# Sonara Plugin Setup Instructions

## Prerequisites

- **macOS**: macOS 10.13 or later for Logic Pro X compatibility
- **CMake**: Version 3.22 or later
- **Xcode**: Latest version with Command Line Tools
- **Git**: To clone JUCE submodule

## Installation Steps

### 1. Clone the Repository

```bash
git clone --recursive https://github.com/yourusername/Sonara.git
cd Sonara
```

If you already cloned without the `--recursive` extraction, initialize the JUCE submodule:

```bash
git submodule update --init --recursive
```

### 2. Build the Plugin

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### 3. Install the Plugin

#### For Logic Pro X (AU format):
```bash
cp Sonara_artefacts/Release/Standalone/Sonara.app /Applications/
cp Sonara_artefacts/Release/AU/Sonara.component ~/Library/Audio/Plug-Ins/Components/
```

#### For VST3 compatibility (Ableton, Pro Tools, etc.):
```bash
cp Sonara_artefacts/Release/VST3/Sonara.vst3 ~/Library/Audio/Plug-Ins/VST3/
```

### 4. Verify Installation

1. Open Logic Pro X
2. Create a new audio track
3. Click on an empty plugin slot
4. Navigate to: **AU > Sonara**
5. You should see "Sonara" in the menu

If the plugin doesn't appear, try rescanning plugins in your DAW's preferences.

## Building for Development

For debug builds with additional logging:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

## Troubleshooting

### Plugin Not Showing Up

1. **Reset AU Validation Cache** (for macOS):
   ```bash
   sudo killall -9 audioOSD
   ```
   
2. **Check if plugin is recognized by system**:
   ```bash
   auval -a
   ```
   Look for "Snar" (Sonara) in the list.

### Build Errors

1. **CMake errors**: Make sure you have the latest CMake installed:
   ```bash
   brew install cmake
   ```

2. **JUCE not found**: Reinitialize the submodule:
   ```bash
   git submodule update --init --recursive
   ```

3. **Xcode errors**: Update Xcode Command Line Tools:
   ```bash
   xcode-select --install
   ```

### Runtime Issues

If the plugin crashes or doesn't process audio:

1. Check the system console for error messages
2. Try a clean rebuild:
   ```bash
   rm -rf build && mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . --config Release
   ```

## Development

### Project Structure

```
Sonara/
├── Source/
│   ├── PluginProcessor.h/cpp      # Main audio processing
│   ├── PluginEditor.h/cpp          # UI implementation
│   ├── KeywordMapper.h/cpp         # Text to parameter mapping
│   ├── ChangesLogger.h/cpp         # Change tracking
│   └── AudioProcessing/            # Audio modules
│       ├── Equalizer.h/cpp
│       ├── Compressor.h/cpp
│       └── ReverbProcessor.h/cpp
├── CMakeLists.txt                  # Build configuration
└── JUCE/                           # JUCE framework (submodule)
```

### Adding New Keywords

Edit `Source/KeywordMapper.cpp` to add new keyword patterns in the appropriate function:
- `processBrightnessKeywords()` - for air/sparkle/bright
- `processWarmthKeywords()` - for warm/smooth
- `processReverbKeywords()` - for room/space
- `processCompressorKeywords()` - for punch/tight
- etc.

## Testing

### Quick Test Phrases

Try these in the plugin:

1. **Brightness**: "make it brighter", "add more air", "sparkle"
2. **Warmth**: "warmer", "add body", "smoother"
3. **Reverb**: "add room", "more space", "hall reverb"
4. **Compression**: "more punch", "glue it", "tighter"
5. **Combinations**: "bright and airy", "warm with room", "punchy and tight"

### Expected Behavior

- Text input should accept natural language
- Intensity slider controls effect strength (0-2x)
- Changes log shows exactly what parameters were modified
- Audio processes in real-time with low latency

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

MIT License - See LICENSE file for details

