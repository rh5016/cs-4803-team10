# Sonara Plugin Installation Instructions

## Quick Install

The plugin has been installed at:
```
~/Library/Audio/Plug-Ins/Components/Sonara.component
```

## Steps to Use in Logic Pro X

### 1. Rescan Plugins in Logic Pro X

1. Open **Logic Pro X**
2. Go to **Logic Pro > Settings > Plug-in Manager** (or press `Cmd+,` then click Plug-ins)
3. Click the **"Rescan Plug-ins"** button at the bottom
4. Wait for the scan to complete
5. Look for "Sonara" in the list - it should show as **"Sonara: Sonara"** under manufacturer **"Sonr"**

### 2. Insert the Plugin

1. Create or select an audio track
2. Click on an empty insert slot
3. Navigate to: **AU > Sonara > Sonara**

### 3. If Plugin Doesn't Appear

Try these steps in order:

**Option A: Restart Logic Pro X**
- Quit Logic Pro X completely (`Cmd+Q`)
- Reopen Logic Pro X
- The plugin should appear automatically

**Option B: Manual Rescan**
```bash
# In Terminal, run:
killall AudioComponentRegistrar
killall -9 audioOSD
# Then reopen Logic Pro X
```

**Option C: Verify Installation**
```bash
ls -la ~/Library/Audio/Plug-Ins/Components/Sonara.component
```

The plugin bundle should exist and show as a directory.

**Option D: Reinstall Plugin**
```bash
# Remove old version
rm -rf ~/Library/Audio/Plug-Ins/Components/Sonara.component

# Copy fresh from build
cp -r ~/Desktop/cs-4803-team10/build/Sonara_artefacts/Release/AU/Sonara.component \
      ~/Library/Audio/Plug-Ins/Components/

# Update PkgInfo
echo "BNDL Sonr" > ~/Library/Audio/Plug-Ins/Components/Sonara.component/Contents/PkgInfo

# Touch to refresh
touch ~/Library/Audio/Plug-Ins/Components/Sonara.component
```

### 4. Plugin Validation Status

The plugin passed macOS AudioUnit validation. If you still don't see it:

1. **Check Logic's minimum supported version**: The plugin was built for macOS 10.13+
2. **Check plugin list in Plug-in Manager**: Look for any "Sonara" entries
3. **Check for error messages**: In Plug-in Manager, look for red/yellow indicators

## Current Status

✅ Plugin validated successfully with auvaltool  
✅ PkgInfo file corrected (BNDL Sonr)  
✅ Info.plist properly configured  
✅ Binary exists and is properly built  

The plugin should appear in Logic Pro X after rescanning.

## Troubleshooting

If the plugin still doesn't work:

1. Check system logs:
   ```bash
   log show --predicate 'process == "AudioComponentRegistrar"' --last 5m
   ```

2. Verify the binary:
   ```bash
   file ~/Library/Audio/Plug-Ins/Components/Sonara.component/Contents/MacOS/Sonara
   ```
   Should show: `Mach-O 64-bit bundle arm64`

3. Rebuild from scratch:
   ```bash
   cd ~/Desktop/cs-4803-team10
   rm -rf build
   ./build.sh
   ```

## Alternative: Use Standalone App

If Logic Pro continues to have issues, you can use the standalone app:

```bash
open build/Sonara_artefacts/Release/Standalone/Sonara.app
```

This doesn't require Logic Pro and can test the plugin's functionality.

