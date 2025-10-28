# Sonara Project Structure

Complete overview of the Sonara audio plugin project structure.

## Directory Layout

```
Sonara/
├── CMakeLists.txt              # Build configuration
├── .gitmodules                 # JUCE git submodule reference
├── .gitignore                  # Git ignore rules
├── build.sh                    # Automated build script
│
├── Source/                     # Main source code
│   ├── JuceHeader.h            # JUCE framework includes
│   │
│   ├── PluginProcessor.h       # Main audio processor declaration
│   ├── PluginProcessor.cpp     # Audio processing implementation
│   ├── PluginEditor.h          # UI declaration
│   ├── PluginEditor.cpp        # UI implementation
│   │
│   ├── KeywordMapper.h         # Text-to-parameter mapping
│   ├── KeywordMapper.cpp       # Keyword processing logic
│   │
│   ├── ChangesLogger.h         # Change tracking
│   ├── ChangesLogger.cpp       # Logging implementation
│   │
│   └── AudioProcessing/        # Audio processing modules
│       ├── Equalizer.h         # EQ module
│       ├── Equalizer.cpp
│       ├── Compressor.h        # Compressor module
│       ├── Compressor.cpp
│       ├── ReverbProcessor.h   # Reverb module
│       └── ReverbProcessor.cpp
│
├── JUCE/                       # JUCE framework (submodule)
│
├── Documentation/
│   ├── README.md               # Main documentation
│   ├── SETUP.md                # Setup instructions
│   ├── EXAMPLES.md             # Usage examples
│   ├── CONTRIBUTING.md         # Contribution guidelines
│   └── PROJECT_STRUCTURE.md    # This file
│
├── index.html                  # Marketing website
├── LICENSE                     # MIT License
└── CONTRIBUTING.md             # Contribution guidelines
```

## Core Components

### 1. Audio Processing (`Source/PluginProcessor.*`)

The main audio processing engine:
- Initializes audio modules (EQ, Compressor, Reverb)
- Handles audio processing pipeline
- Manages plugin state and parameters
- Communicates with UI

**Key Methods:**
- `processTextInput()` - Processes user text and applies parameters
- `processBlock()` - Real-time audio processing
- `setIntensity()` - Controls effect strength

### 2. Keyword Mapper (`Source/KeywordMapper.*`)

Maps natural language to audio parameters:

**Keyword Categories:**
- **Brightness**: bright, airy, sparkle, clarity
- **Warmth**: warm, body, smooth
- **Reverb**: room, space, hall, ambience
- **Compression**: punch, glue, tight
- **Bass**: bass, low end, deep
- **Presence**: presence, forward, vocal

**Processing Flow:**
1. Parse user input text
2. Match keywords to effect categories
3. Calculate parameter values
4. Generate change log entries
5. Return AudioParameters structure

### 3. Audio Modules (`Source/AudioProcessing/`)

#### Equalizer
- High shelf filter (air/brightness)
- Mid peak filter (presence/warmth)
- Low shelf filter (bass)
- Process using JUCE's IIR filters

#### Compressor
- Threshold, ratio, attack, release
- Makeup gain compensation
- Runtime enable/disable
- Uses JUCE's DSP compressor

#### ReverbProcessor
- Room size, damping, width
- Wet/dry mix
- Runtime enable/disable
- Uses JUCE's reverb algorithms

### 4. UI (`Source/PluginEditor.*`)

Modern, professional interface with:

**Components:**
- Text input field for natural language commands
- Intensity slider (0-2x)
- Changes log display
- Custom dark theme with accents

**Colors:**
- Primary: `#8affb4` (green)
- Secondary: `#a78bfa` (purple)
- Background: Dark gradient (`#000000` → `#0a1628`)
- Accents: Various colors for different effect types

### 5. Change Logging (`Source/ChangesLogger.*`)

Tracks and displays what changes were applied:
- Stores change descriptions with colors
- Timestamps entries
- Limits to recent 20 changes
- Colored by effect type

## Build System

### CMake Configuration

Builds for multiple platforms:
- **AU**: Audio Units (macOS, Logic Pro X)
- **VST3**: Universal VST3 format
- **Standalone**: Standalone application

### Build Process

1. Configure with CMake
2. Add JUCE as submodule dependency
3. Link required JUCE modules:
   - `juce_audio_basics`
   - `juce_dsp` (DSP algorithms)
   - `juce_audio_processors`
   - `juce_gui_basics`
   - etc.

## Data Flow

```
User Input (Text)
    ↓
KeywordMapper::processText()
    ↓
AudioParameters struct
    ↓
PluginProcessor::processTextInput()
    ↓
Apply to audio modules (EQ/Comp/Rev)
    ↓
PluginProcessor::processBlock()
    ↓
Audio Output
```

## Extension Points

### Adding New Keywords

Edit `Source/KeywordMapper.cpp`:
```cpp
void KeywordMapper::processXxxKeywords(...) {
    if (text.contains("new-keyword")) {
        params.eq.highShelfGain = 3.0f;
        addChange("High Shelf +3dB", colour);
    }
}
```

### Adding New Effects

1. Create new module in `Source/AudioProcessing/`
2. Add to processor initialization
3. Add processing in `processTextInput()`
4. Update keyword mapper

### UI Customization

Edit `Source/PluginEditor.*`:
- Colors in `CustomLookAndFeel`
- Layout in `resized()`
- Components in constructor

## Platform Support

- ✅ macOS (Logic Pro X, Pro Tools, Ableton)
- ✅ Windows (VST3 compatible DAWs)
- ✅ Linux (VST3 compatible DAWs)

## Requirements

- CMake 3.22+
- JUCE 7.x (via git submodule)
- C++17 compiler
- Xcode (macOS) or Visual Studio (Windows)

## License

MIT License - See [LICENSE](LICENSE)

