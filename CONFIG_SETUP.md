# API Key Configuration Setup

## ✅ Secure API Key Storage

The API key is now stored in a **gitignored** `config.h` file, so it won't be committed to version control.

## How It Works

The plugin loads the API key in this priority order:

1. **Environment Variable** (`GEMINI_API_KEY`) - Most secure, recommended for production
2. **config.h file** - Good for development, gitignored
3. **Hardcoded fallback** - Only used if neither of the above are set (remove before public release!)

## Setup Instructions

### Option 1: Use config.h (Recommended for Development)

1. **Copy the template:**
   ```bash
   cp config.h.example config.h
   ```

2. **Edit `config.h` and add your API key:**
   ```cpp
   #define GEMINI_API_KEY "your-actual-api-key-here"
   ```

3. **The `config.h` file is already gitignored** - it won't be committed!

### Option 2: Use Environment Variable (Recommended for Production)

Set the environment variable before running Logic Pro:

```bash
# macOS/Linux
export GEMINI_API_KEY="your-api-key-here"

# To make it permanent, add to ~/.zshrc or ~/.bashrc:
echo 'export GEMINI_API_KEY="your-api-key-here"' >> ~/.zshrc
```

### Option 3: Remove Hardcoded Key (Before Public Release)

Before sharing the code publicly:

1. Remove the hardcoded API key from `Source/PluginProcessor.cpp` (line ~50)
2. Users will need to either:
   - Set `GEMINI_API_KEY` environment variable, OR
   - Create their own `config.h` file from `config.h.example`

## Current Status

✅ `config.h` is created with your API key  
✅ `config.h` is in `.gitignore` (won't be committed)  
✅ `config.h.example` is tracked (template for others)  
✅ Plugin reads from config.h automatically  

## For Distribution

When sharing the plugin:

1. **Remove the hardcoded API key** from `Source/PluginProcessor.cpp`
2. Include `config.h.example` in the distribution
3. Users create their own `config.h` from the example
4. Or users set the `GEMINI_API_KEY` environment variable

## Verification

To verify `config.h` is gitignored:

```bash
git check-ignore config.h
# Should output: config.h
```

To verify it's not tracked:

```bash
git status
# config.h should NOT appear in the list
```

