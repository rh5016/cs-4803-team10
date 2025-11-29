#!/bin/bash

# Sonara Plugin Installation Script
# This script installs the Sonara audio plugin for Logic Pro X and other AU-compatible DAWs

set -e

echo "🎵 Installing Sonara Plugin..."
echo ""

# Check if we're in the right directory
if [ ! -d "Sonara.component" ]; then
    echo "❌ Error: Sonara.component not found!"
    echo "Please run this script from the directory containing Sonara.component"
    exit 1
fi

# Create plugin directory if it doesn't exist
mkdir -p ~/Library/Audio/Plug-Ins/Components

# Remove old version if it exists
if [ -d ~/Library/Audio/Plug-Ins/Components/Sonara.component ]; then
    echo "Removing old version..."
    rm -rf ~/Library/Audio/Plug-Ins/Components/Sonara.component
fi

# Copy new version
echo "Installing plugin..."
cp -r Sonara.component ~/Library/Audio/Plug-Ins/Components/

# Fix PkgInfo
echo "Fixing plugin metadata..."
echo "BNDL Sonr" > ~/Library/Audio/Plug-Ins/Components/Sonara.component/Contents/PkgInfo

# Touch to refresh
touch ~/Library/Audio/Plug-Ins/Components/Sonara.component

# Reset AudioComponentRegistrar (forces macOS to recognize the plugin)
killall AudioComponentRegistrar 2>/dev/null || true

echo ""
echo "✅ Installation complete!"
echo ""
echo "📝 Next steps:"
echo "1. Quit Logic Pro X (or your DAW) if it's running"
echo "2. Open Logic Pro X (or your DAW)"
echo "3. Go to Logic Pro > Settings > Plug-in Manager"
echo "4. Click 'Rescan Plug-ins' and wait for scan to complete"
echo "5. Look for 'Sonara: Sonara' in the plugin list"
echo ""
echo "💡 IMPORTANT: Gemini API Key Setup"
echo "   The plugin uses Google's Gemini AI for enhanced text processing."
echo "   You'll need to set up your API key. See README.txt for instructions."
echo ""

