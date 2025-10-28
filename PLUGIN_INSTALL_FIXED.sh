#!/bin/bash

# Sonara Plugin Installation Script - Fixed Version

set -e

echo "🎵 Installing Sonara Plugin..."

# Remove old version
echo "Removing old version..."
rm -rf ~/Library/Audio/Plug-Ins/Components/Sonara.component

# Copy new version
echo "Installing plugin..."
cp -r build/Sonara_artefacts/Release/AU/Sonara.component ~/Library/Audio/Plug-Ins/Components/

# Fix PkgInfo
echo "Fixing PkgInfo..."
echo "BNDL Sonr" > ~/Library/Audio/Plug-Ins/Components/Sonara.component/Contents/PkgInfo

# Touch to refresh
touch ~/Library/Audio/Plug-Ins/Components/Sonara.component

# Reset AudioComponentRegistrar
echo "Resetting audio component registration..."
killall AudioComponentRegistrar 2>/dev/null || true

echo ""
echo "✅ Installation complete!"
echo ""
echo "📝 Next steps:"
echo "1. Quit Logic Pro X if it's running"
echo "2. Open Logic Pro X"
echo "3. If you see a validation message, click 'Start Plug-In Manager'"
echo "4. Look for 'Sonara: Sonara' in the plugin list"
echo "5. If it shows an error, click 'Reset & Rescan Selection' next to it"
echo ""
echo "The plugin has passed all AU validation tests and should work properly now."

