# Quick Start: Gemini API Integration

## Setup (5 minutes)

### 1. Get Your Free API Key

1. Go to https://aistudio.google.com/api-keys
2. Sign in with Google
3. Click "Create API Key"
4. Copy the key (starts with `AIza...`)

### 2. Configure in Code

```cpp
#include "PluginProcessor.h"

// In your initialization code:
processor.setGeminiApiKey("YOUR_API_KEY_HERE");
```

### 3. Use Gemini-Enhanced Processing

```cpp
// Async processing (recommended)
processor.processTextInputWithGemini(
    "make the vocals airy and spacious",
    [this]() {
        // Done! Update UI
        updateChangesDisplay();
    }
);
```

## How It Works

**Without Gemini:**
```
User: "make it brighter"
→ Direct keyword matching
→ Audio parameters applied
```

**With Gemini:**
```
User: "I want the track to sparkle more with extra clarity"
→ Gemini: "bright, sparkle, clarity, presence"
→ Keyword mapping
→ Audio parameters applied
```

## Fallback

If Gemini fails or isn't configured, the plugin automatically uses direct keyword matching. Your plugin always works!

## Example

```cpp
// Set API key (do this once, maybe on plugin load)
void MyPlugin::initialize() {
    // Load from settings or get from user
    auto apiKey = loadApiKeyFromSettings();
    processor.setGeminiApiKey(apiKey);
}

// Process user input
void MyPlugin::onUserInput(const juce::String& text) {
    if (processor.isGeminiEnabled()) {
        // Use Gemini for better understanding
        processor.processTextInputWithGemini(text, [this]() {
            refreshUI();
        });
    } else {
        // Fallback to direct processing
        processor.processTextInput(text);
        refreshUI();
    }
}
```

## Free Tier Limits

- ✅ 1,500 requests/day
- ✅ 1,000,000 tokens/minute
- ✅ Perfect for development

## Need Help?

- Check `GEMINI_INTEGRATION.md` for detailed docs
- Verify API key at https://aistudio.google.com/api-keys
- Check the changes log for error messages

