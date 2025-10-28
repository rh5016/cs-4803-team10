#!/bin/bash

# Sonara Plugin Build Script

set -e

echo "🎵 Building Sonara Audio Plugin..."

# Check if JUCE submodule is initialized
if [ ! -d "JUCE/modules" ]; then
    echo "⚠️  JUCE submodule not found. Initializing..."
    git submodule update --init --recursive
fi

# Create build directory
mkdir -p build
cd build

# Configure CMake
echo "📐 Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "🔨 Building plugin..."
cmake --build . --config Release

# Check if build was successful
if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Build successful!"
    echo ""
    echo "📦 Plugin files created:"
    echo "   - Standalone: Sonara_artefacts/Release/Standalone/Sonara.app"
    echo "   - AU:         Sonara_artefacts/Release/AU/Sonara.component"
    echo "   - VST3:       Sonara_artefacts/Release/VST3/Sonara.vst3"
    echo ""
    echo "📝 To install for Logic Pro X:"
    echo "   cp Sonara_artefacts/Release/AU/Sonara.component ~/Library/Audio/Plug-Ins/Components/"
    echo ""
else
    echo ""
    echo "❌ Build failed!"
    exit 1
fi

