# Gemini API Integration Guide

This document explains how to integrate Google's Gemini API with the Sonara plugin for enhanced natural language processing.

## Overview

The Gemini integration allows the plugin to use Google's LLM (Large Language Model) to better understand and interpret user requests before mapping them to audio parameters. This enables more natural language understanding and better handling of complex requests.

## Getting Started

### 1. Get a Gemini API Key (Free Tier)

1. Visit [Google AI Studio](https://aistudio.google.com/api-keys)
2. Sign in with your Google account
3. Click "Create API Key"
4. Select "Create API key in new project" or use an existing project
5. Copy your API key

**Free Tier Limits:**
- Up to 1,500 requests per day
- 1,000,000 tokens per minute
- Perfect for development and personal use

### 2. Configure the API Key

You can set the Gemini API key programmatically:

```cpp
// In your plugin code
processor.setGeminiApiKey("YOUR_API_KEY_HERE");
```

To check if Gemini is enabled:

```cpp
if (processor.isGeminiEnabled()) {
    // Gemini is configured and ready to use
}
```

### 3. Using Gemini-Enhanced Text Processing

#### Option A: Async Processing (Recommended)

Use `processTextInputWithGemini()` for non-blocking processing:

```cpp
processor.processTextInputWithGemini("make the vocals more airy and spacious", [this]() {
    // Called when processing is complete
    // Update UI or perform other actions
    updateChangesDisplay();
});
```

#### Option B: Direct Keyword Mapping (Fallback)

If Gemini is not configured or fails, the plugin automatically falls back to direct keyword mapping:

```cpp
// This always works, with or without Gemini
processor.processTextInput("make it bright and punchy");
```

## How It Works

1. **User Input**: User types a natural language request (e.g., "I want my vocals to sound more spacious and professional")

2. **Gemini Processing** (if enabled): 
   - The text is sent to Gemini API
   - Gemini normalizes and enhances the request into standardized audio engineering keywords
   - Example: "airy, spacious, reverb, hall" 

3. **Keyword Mapping**: 
   - The processed text goes through the existing KeywordMapper
   - Keywords are mapped to specific audio parameters
   - Parameters are applied to EQ, Compressor, and Reverb processors

4. **Fallback**: 
   - If Gemini fails or isn't configured, the original text is processed directly
   - The plugin continues to work normally without LLM enhancement

## Architecture

### Components

- **`GeminiClient`**: Handles HTTP communication with Gemini API
  - Location: `Source/GeminiClient.h` and `.cpp`
  - Uses JUCE's networking capabilities for HTTP requests
  - Runs async requests on a background thread

- **`KeywordMapper`**: Enhanced with optional Gemini integration
  - Location: `Source/KeywordMapper.h` and `.cpp`
  - Can process text with or without Gemini
  - Automatically falls back if Gemini fails

- **`SonaraAudioProcessor`**: Exposes Gemini functionality
  - Location: `Source/PluginProcessor.h` and `.cpp`
  - Provides methods to configure and use Gemini

### Request Flow

```
User Input
    ↓
[GeminiClient] (if enabled)
    ↓
Enhanced Keywords
    ↓
[KeywordMapper]
    ↓
Audio Parameters
    ↓
[Audio Processors]
```

## Example Usage

### Basic Setup

```cpp
#include "PluginProcessor.h"

// Set API key
processor.setGeminiApiKey("AIzaSy...");  // Your actual API key

// Process text with Gemini
processor.processTextInputWithGemini(
    "I want my guitar to sound warm and full with a bit of room reverb",
    [this]() {
        // Processing complete
        auto changes = processor.getChangeLog();
        // Display changes to user
    }
);
```

### Error Handling

The integration includes automatic fallback:

```cpp
// This will always work, even if:
// - API key is not set
// - Network is down
// - API rate limit exceeded
// - Invalid API key

processor.processTextInputWithGemini(text, []() {
    // Called after processing (with or without Gemini)
});
```

If Gemini fails, you'll see a message in the changes log: "LLM processing failed, using direct keyword mapping"

## API Key Security

**Important Security Notes:**

1. **Never commit API keys to version control**
   - Add `*api_key*` to your `.gitignore`
   - Use environment variables or secure storage

2. **Store API keys securely**
   - Consider using JUCE's `ValueTree` with encryption
   - Or store in system keychain/credential store

3. **Limit API key scope**
   - In Google Cloud Console, restrict API key to specific APIs
   - Set usage quotas and restrictions

## Troubleshooting

### Gemini not processing requests

1. Check if API key is set:
   ```cpp
   if (!processor.isGeminiEnabled()) {
       // API key not configured
   }
   ```

2. Check network connectivity

3. Verify API key is valid and has quota remaining

4. Check the changes log for error messages

### Slow responses

- Gemini API typically responds in 1-3 seconds
- If slow, check your internet connection
- Consider using direct keyword mapping for faster response

### Rate limiting

- Free tier: 1,500 requests/day
- If exceeded, the plugin automatically falls back to keyword mapping
- Wait 24 hours or upgrade to paid tier

## Building

The Gemini integration requires:

- JUCE networking module (already included)
- C++17 or later
- Internet connection at runtime

Build commands:

```bash
cd build
cmake ..
make
```

## Future Enhancements

Potential improvements:

- Persistent API key storage (encrypted)
- UI for API key configuration
- Request caching to reduce API calls
- Support for other LLM providers (OpenAI, etc.)
- Batch processing multiple requests

## Support

For issues or questions:
- Check the changes log for error messages
- Verify API key at [Google AI Studio](https://aistudio.google.com/api-keys)
- Review Gemini API documentation: https://ai.google.dev/docs

## License

This integration uses Google's Gemini API, which is subject to Google's Terms of Service and API usage policies.

