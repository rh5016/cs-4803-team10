========================================
SONARA AUDIO PLUGIN - INSTALLATION GUIDE
========================================

Sonara is an intelligent plugin that transforms your sound 
based on text input. Simply describe what you want, and 
Sonara applies professional audio processing automatically.

========================================
QUICK INSTALL (currently only for macOS)
========================================

1. Double-click "Install_Sonara.sh"
2. Enter your password when prompted (if asked)
3. Open your DAW
4. Rescan plugins:
   - Logic Pro: Logic Pro > Settings > Plug-in Manager > Rescan Plug-ins
   - Other DAWs: Check your DAW's plugin preferences
(Be patient with this since it sometimes takes a minute to show up)

========================================
MANUAL INSTALL
========================================

If the installer doesn't work:

1. Copy "Sonara.component" to:
   ~/Library/Audio/Plug-Ins/Components/
   
2. Run this command in Terminal:
   echo "BNDL Sonr" > ~/Library/Audio/Plug-Ins/Components/Sonara.component/Contents/PkgInfo
   
3. Restart your DAW

========================================
GEMINI API KEY SETUP (REQUIRED)
========================================

Sonara uses Google's Gemini AI for enhanced natural language processing.
You need your own free Gemini API key to use this feature.

Getting Your API Key:
---------------------
1. Visit: https://aistudio.google.com/api-keys
2. Sign in with your Google account
3. Click "Create API Key"
4. Copy your API key (starts with "AIza...")

Setting Up the API Key:
-------------------------------------------
1. Open Terminal
2. Run this command (replace YOUR_KEY_HERE with your actual key):
   
   export GEMINI_API_KEY="YOUR_KEY_HERE"
   
3. To make it permanent, add to ~/.zshrc:
   
   echo 'export GEMINI_API_KEY="YOUR_KEY_HERE"' >> ~/.zshrc
   source ~/.zshrc

4. Restart your DAW after setting the environment variable

========================================
USAGE
========================================

1. Insert Sonara on an audio track in your DAW
2. Type your desired effect in plain English:
   - "make vocals bright and airy"
   - "add reverb"
   - "more bass"
   - "remove reverb"
   - "punchy compression"
3. Adjust the intensity slider to control effect strength
4. See the "Changes Applied" log to understand what was changed

========================================
SUPPORTED COMMANDS
========================================

• Brightness: "bright", "airy", "sparkle", "crisp", "remove bright"
• Warmth: "warm", "smooth", "body", "full"
• Reverb: "add reverb", "room", "hall", "ambience", "remove reverb"
• Compression: "punch", "tight", "glue", "level"
• Bass: "more bass", "deeper", "remove bass", "less bass"
• Presence: "forward", "vocal", "upfront", "presence"

The plugin understands natural language, so you can combine commands:
- "make vocals bright with a bit of room reverb"
- "add bass and make it punchy"
- "remove reverb and add more presence"

========================================
REQUIREMENTS
========================================

• macOS 10.13 or later
• Logic Pro X, Ableton Live, or other AU-compatible DAW
• Internet connection (for Gemini AI features)
• Free Gemini API key (for AI-enhanced processing)

========================================
TROUBLESHOOTING
========================================

Plugin doesn't appear in DAW:
------------------------------
1. Make sure Logic Pro X (or your DAW) is completely quit
2. Try running: killall AudioComponentRegistrar
3. Reopen your DAW
4. Rescan plugins in Plug-in Manager
5. Verify the plugin is installed:
   ls -la ~/Library/Audio/Plug-Ins/Components/Sonara.component

Plugin appears but shows errors:
---------------------------------
1. Check that you're running macOS 10.13 or later
2. Verify the plugin binary:
   file ~/Library/Audio/Plug-Ins/Components/Sonara.component/Contents/MacOS/Sonara
   Should show: "Mach-O 64-bit bundle arm64" (or similar)

Gemini AI not working:
----------------------
1. Make sure you've set up your API key (see above)
2. Check your internet connection
3. Verify the API key is correct
4. Check Gemini Studio dashboard: https://aistudio.google.com
5. The plugin will still work with basic keyword matching if Gemini fails

Text processing not working:
-----------------------------
1. Make sure you've typed text in the text input field
2. Press Enter or click outside the text field to process
3. Check the "Changes Applied" log for any error messages

========================================
SUPPORT
========================================

For more information, documentation, or to report issues, email:
rhegde8@gatech.edu

For questions about Gemini API:
https://ai.google.dev/docs

Built with JUCE.

