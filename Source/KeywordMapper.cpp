#include "KeywordMapper.h"
#include "GeminiClient.h"

KeywordMapper::KeywordMapper() {
    // Gemini client will be created when API key is set
    geminiClient = nullptr;
    
    // Initialize keyword lists
    brightnessKeywords = {
        "bright", "brighter", "brightness", "sparkle", "sparkly", "air", "airy", "airiness",
        "crisp", "crispy", "highs", "high end", "treble", "presence", "shine", "shiny",
        "clear", "clearer", "clarity", "detail", "detailed"
    };
    
    warmthKeywords = {
        "warm", "warmer", "warmth", "smooth", "smoother", "body", "full", "fuller",
        "thick", "thicker", "round", "rounder", "mellow", "soft", "softer", "sweet"
    };
    
    reverbKeywords = {
        "reverb", "verb", "room", "space", "spacious", "hall", "ambience", "ambient",
        "distance", "distant", "echo", "echoes", "wet", "wetness", "atmosphere", "atmospheric"
    };
    
    compressorKeywords = {
        "punch", "punchy", "tight", "tighter", "glue", "glued", "glue the mix", "cohesion",
        "consistent", "consistent dynamics", "control", "controlled", "compression", "compress",
        "even", "even dynamics", "level", "leveled"
    };
    
    bassKeywords = {
        "bass", "low end", "lows", "low", "deeper", "deep", "boom", "boomy", "thump",
        "thumpy", "kick", "punch", "weight", "heavy", "thick"
    };
    
    presenceKeywords = {
        "presence", "forward", "upfront", "cut", "cut through", "vocal", "vocals",
        "mid", "mids", "midrange", "snap", "snappy"
    };
}

AudioParameters KeywordMapper::processText(const juce::String& text, float baseIntensity) {
    recentChanges.clear();
    
    // Convert to lowercase for matching
    juce::String lowerText = text.toLowerCase();
    
    // Extract intensity modifiers
    float intensity = extractIntensity(lowerText) * baseIntensity;
    
    // Initialize parameters
    AudioParameters params;
    params.intensity = intensity;
    
    // Process each effect category
    processBrightnessKeywords(lowerText, params);
    processWarmthKeywords(lowerText, params);
    processReverbKeywords(lowerText, params);
    processCompressorKeywords(lowerText, params);
    processBassKeywords(lowerText, params);
    processPresenceKeywords(lowerText, params);
    
    // Apply intensity to all parameters (only if intensity is positive)
    // For negative values (like reductions), we still want them to work
    if (intensity > 0) {
        params.eq.highShelfGain *= intensity;
        params.eq.midGain *= intensity;
        params.eq.lowShelfGain *= intensity;
        params.reverb.wetLevel *= intensity;
        params.reverb.roomSize *= intensity;
        
        // Compressor ratio shouldn't go below 1.0
        float newRatio = params.compressor.ratio * intensity;
        params.compressor.ratio = (newRatio < 1.0f) ? params.compressor.ratio : newRatio;
    }
    
    return params;
}

bool KeywordMapper::containsKeyword(const juce::String& text, const std::vector<juce::String>& keywords) {
    for (const auto& keyword : keywords) {
        if (text.contains(keyword)) {
            return true;
        }
    }
    return false;
}

float KeywordMapper::extractIntensity(const juce::String& text) {
    // Check for removal/reduction keywords first
    if (text.contains("remove") || text.contains("no ") || text.contains("without") || 
        text.contains("take away") || text.contains("eliminate") || text.contains("cut")) {
        return -2.0f; // Strong negative for removal
    }
    if (text.contains("slight") || text.contains("little") || text.contains("bit")) return 0.5f;
    if (text.contains("more") || text.contains("much") || text.contains("add")) return 1.5f;
    if (text.contains("very") || text.contains("super") || text.contains("extreme")) return 2.0f;
    if (text.contains("less") || text.contains("reduce") || text.contains("lower") || text.contains("decrease")) return -1.0f;
    return 1.0f;
}

void KeywordMapper::processBrightnessKeywords(const juce::String& text, AudioParameters& params) {
    // Check for removal first
    if (text.contains("remove bright") || text.contains("no bright") || text.contains("without bright") ||
        text.contains("remove highs") || text.contains("cut highs") || text.contains("take away bright")) {
        params.eq.highShelfFreq = 8000.0f;
        params.eq.highShelfGain = -5.0f;
        addChange("High Shelf 8kHz -5.0dB (removed)", juce::Colour(0xff8affb4));
    }
    // Check for dark/dull (reduction)
    else if (text.contains("dull") || text.contains("dark") || text.contains("muddy") ||
             text.contains("less bright") || text.contains("reduce bright")) {
        params.eq.highShelfFreq = 8000.0f;
        params.eq.highShelfGain = -3.0f;
        addChange("High Shelf 8kHz -3.0dB", juce::Colour(0xff8affb4));
    }
    // Then check for brightness boosts
    else if (containsKeyword(text, brightnessKeywords)) {
        if (text.contains("more air") || text.contains("airy") || text.contains("airiness")) {
            params.eq.highShelfFreq = 10000.0f;
            params.eq.highShelfGain = 4.0f;
            addChange("High Shelf 10kHz +4.0dB", juce::Colour(0xff8affb4));
        } 
        else if (text.contains("sparkle") || text.contains("sparkly") || text.contains("shine") || text.contains("shiny")) {
            params.eq.highShelfFreq = 12000.0f;
            params.eq.highShelfGain = 3.0f;
            addChange("High Shelf 12kHz +3.0dB", juce::Colour(0xff8affb4));
        } 
        else if (text.contains("crisp") || text.contains("crispy") || text.contains("highs") || text.contains("treble")) {
            params.eq.highShelfFreq = 9000.0f;
            params.eq.highShelfGain = 3.0f;
            addChange("High Shelf 9kHz +3.0dB", juce::Colour(0xff8affb4));
        }
        else {
            // Default brightness boost
            params.eq.highShelfFreq = 8000.0f;
            params.eq.highShelfGain = 2.5f;
            addChange("High Shelf 8kHz +2.5dB", juce::Colour(0xff8affb4));
        }
        
        // Clarity can be applied in addition to brightness
        if (text.contains("clarity") || text.contains("clear") || text.contains("clearer") || 
            text.contains("detail") || text.contains("detailed")) {
            // Only apply clarity mid boost if presence hasn't already set a higher mid freq
            if (params.eq.midFreq < 2000.0f || params.eq.midFreq == 2000.0f) {
                params.eq.midFreq = 2500.0f;
                params.eq.midGain = 2.0f;
                params.eq.midQ = 1.5f;
                addChange("Peak 2.5kHz +2.0dB Q:1.5", juce::Colour(0xff8affb4));
            }
        }
    }
}

void KeywordMapper::processWarmthKeywords(const juce::String& text, AudioParameters& params) {
    if (containsKeyword(text, warmthKeywords)) {
        if (text.contains("warm") || text.contains("warmth")) {
            params.eq.midFreq = 800.0f;
            params.eq.midGain = 2.0f;
            params.eq.midQ = 1.0f;
            addChange("Peak 800Hz +2.0dB", juce::Colour(0xffa78bfa));
            
            params.eq.highShelfFreq = 10000.0f;
            params.eq.highShelfGain = -1.5f;
            addChange("High Shelf 10kHz -1.5dB", juce::Colour(0xffa78bfa));
        }
        
        if (text.contains("body") || text.contains("full")) {
            params.eq.midFreq = 300.0f;
            params.eq.midGain = 3.0f;
            params.eq.midQ = 1.5f;
            addChange("Peak 300Hz +3.0dB", juce::Colour(0xffa78bfa));
        }
        
        if (text.contains("smooth")) {
            params.eq.highShelfFreq = 5000.0f;
            params.eq.highShelfGain = -2.0f;
            addChange("High Shelf 5kHz -2.0dB", juce::Colour(0xffa78bfa));
        }
    }
}

void KeywordMapper::processReverbKeywords(const juce::String& text, AudioParameters& params) {
    // Check for removal first
    bool removeReverb = text.contains("remove reverb") || text.contains("no reverb") || 
                        text.contains("without reverb") || text.contains("take away reverb");
    
    if (removeReverb) {
        params.reverb.enabled = false;
        params.reverb.wetLevel = 0.0f;
        addChange("Reverb: Disabled", juce::Colour(0xff10b981));
        return;
    }
    
    // Check if reverb keywords are present (including "add reverb")
    if (containsKeyword(text, reverbKeywords) || text.contains("add reverb") || 
        text.contains("with reverb") || text.contains("put reverb")) {
        params.reverb.enabled = true;
        
        if (text.contains("room") || text.contains("space") || text.contains("spacious")) {
            params.reverb.roomSize = 0.4f;
            params.reverb.damping = 0.3f;
            params.reverb.wetLevel = 0.15f;
            params.reverb.width = 0.8f;
            addChange("Room Reverb: Wet 15%, Room 40%, Damping 30%", juce::Colour(0xff10b981));
        } else if (text.contains("hall")) {
            params.reverb.roomSize = 0.8f;
            params.reverb.damping = 0.5f;
            params.reverb.wetLevel = 0.25f;
            params.reverb.width = 1.0f;
            addChange("Hall Reverb: Wet 25%, Room 80%, Damping 50%", juce::Colour(0xff10b981));
        } else if (text.contains("ambience") || text.contains("ambient")) {
            params.reverb.roomSize = 0.3f;
            params.reverb.damping = 0.4f;
            params.reverb.wetLevel = 0.1f;
            params.reverb.width = 0.9f;
            addChange("Ambience: Wet 10%, Room 30%, Damping 40%", juce::Colour(0xff10b981));
        } else {
            // Default reverb when just "add reverb" or "reverb" is mentioned
            params.reverb.roomSize = 0.4f;
            params.reverb.damping = 0.3f;
            params.reverb.wetLevel = 0.2f;
            params.reverb.width = 0.9f;
            addChange("Reverb: Wet 20%, Room 40%, Damping 30%", juce::Colour(0xff10b981));
        }
    }
    
    if (text.contains("dry") || text.contains("close") || text.contains("upfront")) {
        params.reverb.enabled = true;
        params.reverb.wetLevel = 0.05f;
        params.reverb.dryLevel = 0.95f;
        addChange("Dry Mix: Wet 5%", juce::Colour(0xff10b981));
    }
}

void KeywordMapper::processCompressorKeywords(const juce::String& text, AudioParameters& params) {
    if (containsKeyword(text, compressorKeywords)) {
        params.compressor.enabled = true;
        
        // Check for punch - but only if it's not in the context of bass (bass punch = different meaning)
        if ((text.contains("punch") || text.contains("punchy")) && !text.contains("bass") && !text.contains("kick")) {
            params.compressor.threshold = -12.0f;
            params.compressor.ratio = 4.0f;
            params.compressor.attack = 3.0f;
            params.compressor.release = 60.0f;
            params.compressor.makeupGain = 2.0f;
            addChange("Compressor: Ratio 4:1, Attack 3ms, Release 60ms, +2dB makeup", juce::Colour(0xffff6b35));
        } 
        else if (text.contains("glue") || text.contains("tight") || text.contains("tighter")) {
            params.compressor.threshold = -8.0f;
            params.compressor.ratio = 2.5f;
            params.compressor.attack = 10.0f;
            params.compressor.release = 100.0f;
            params.compressor.makeupGain = 1.0f;
            addChange("Compressor: Ratio 2.5:1, Attack 10ms, Release 100ms", juce::Colour(0xffff6b35));
        } 
        else if (text.contains("level") || text.contains("leveled") || text.contains("even") || 
                 text.contains("consistent") || text.contains("control") || text.contains("controlled")) {
            params.compressor.threshold = -10.0f;
            params.compressor.ratio = 3.0f;
            params.compressor.attack = 20.0f;
            params.compressor.release = 150.0f;
            params.compressor.makeupGain = 1.5f;
            addChange("Compressor: Ratio 3:1, Attack 20ms, Release 150ms", juce::Colour(0xffff6b35));
        }
        else {
            // Default compression if keyword is present but no specific type matched
            params.compressor.threshold = -10.0f;
            params.compressor.ratio = 3.0f;
            params.compressor.attack = 15.0f;
            params.compressor.release = 100.0f;
            params.compressor.makeupGain = 1.0f;
            addChange("Compressor: Ratio 3:1 (default)", juce::Colour(0xffff6b35));
        }
    }
}

void KeywordMapper::processBassKeywords(const juce::String& text, AudioParameters& params) {
    bool bassApplied = false;
    
    // Check for removal first
    if (text.contains("remove bass") || text.contains("no bass") || text.contains("without bass") ||
        text.contains("take away bass") || text.contains("cut bass")) {
        params.eq.lowShelfFreq = 100.0f;
        params.eq.lowShelfGain = -5.0f;
        addChange("Low Shelf 100Hz -5.0dB (removed)", juce::Colour(0xff4fc3f7));
        bassApplied = true;
    }
    // Check for specific bass reduction keywords
    else if (text.contains("boom") || text.contains("boomy") || text.contains("reduce bass") || 
             text.contains("less bass") || text.contains("lower bass")) {
        params.eq.lowShelfFreq = 150.0f;
        params.eq.lowShelfGain = -3.0f;
        addChange("Low Shelf 150Hz -3.0dB", juce::Colour(0xff4fc3f7));
        bassApplied = true;
    }
    // Check for deep bass keywords
    else if (text.contains("deeper") || text.contains("deep bass") || text.contains("sub") || 
             text.contains("add bass") || text.contains("more bass")) {
        params.eq.lowShelfFreq = 60.0f;
        params.eq.lowShelfGain = 3.0f;
        addChange("Low Shelf 60Hz +3.0dB", juce::Colour(0xff4fc3f7));
        bassApplied = true;
    }
    // General bass boost
    else if (containsKeyword(text, bassKeywords)) {
        params.eq.lowShelfFreq = 100.0f;
        params.eq.lowShelfGain = 4.0f;
        addChange("Low Shelf 100Hz +4.0dB", juce::Colour(0xff4fc3f7));
        bassApplied = true;
    }
}

void KeywordMapper::processPresenceKeywords(const juce::String& text, AudioParameters& params) {
    if (containsKeyword(text, presenceKeywords)) {
        // Only apply presence if mid frequency hasn't been set by warmth keywords
        // (warmth uses lower frequencies, presence uses higher)
        if (params.eq.midFreq < 2000.0f) {
            // Warmth was already applied, don't override
            return;
        }
        
        if (text.contains("snap") || text.contains("snappy")) {
            params.eq.midFreq = 4000.0f;
            params.eq.midGain = 2.5f;
            params.eq.midQ = 2.5f;
            addChange("Peak 4kHz +2.5dB Q:2.5", juce::Colour(0xffffb74d));
        }
        else if (text.contains("presence") || text.contains("forward") || text.contains("vocal") || 
                 text.contains("upfront") || text.contains("cut") || text.contains("cut through")) {
            params.eq.midFreq = 3000.0f;
            params.eq.midGain = 3.0f;
            params.eq.midQ = 2.0f;
            addChange("Peak 3kHz +3.0dB Q:2.0", juce::Colour(0xffffb74d));
        }
        else if (text.contains("mid") || text.contains("mids") || text.contains("midrange")) {
            params.eq.midFreq = 2500.0f;
            params.eq.midGain = 2.0f;
            params.eq.midQ = 1.5f;
            addChange("Peak 2.5kHz +2.0dB Q:1.5", juce::Colour(0xffffb74d));
        }
    }
}

void KeywordMapper::addChange(const juce::String& description, const juce::Colour& color) {
    recentChanges.push_back({description, color});
}

std::vector<ChangeLog> KeywordMapper::getRecentChanges() const {
    return recentChanges;
}

KeywordMapper::~KeywordMapper() {
    geminiClient = nullptr; // Will be destroyed automatically via unique_ptr
}

void KeywordMapper::setGeminiApiKey(const juce::String& apiKey) {
    if (apiKey.trim().isNotEmpty()) {
        if (!geminiClient) {
            geminiClient = std::make_unique<GeminiClient>();
        }
        geminiClient->setApiKey(apiKey);
    } else {
        geminiClient = nullptr;
    }
}

bool KeywordMapper::isGeminiEnabled() const {
    return geminiClient != nullptr && geminiClient->isApiKeySet();
}

void KeywordMapper::processTextWithGemini(const juce::String& text, 
                                           float baseIntensity,
                                           std::function<void(const AudioParameters&)> callback) {
    // Capture text by value for the lambda
    juce::String textCopy = text;
    
    // If Gemini is not enabled, fall back to direct processing
    if (!isGeminiEnabled()) {
        addChange("Gemini not enabled, using direct keyword mapping", juce::Colours::orange);
        AudioParameters params = processText(textCopy, baseIntensity);
        if (callback) {
            callback(params);
        }
        return;
    }
    
    // Log that we're using Gemini
    addChange("Processing with Gemini LLM...", juce::Colours::yellow);
    
    // Use Gemini to pre-process the text
    geminiClient->processTextAsync(textCopy, [this, textCopy, baseIntensity, callback](bool success, const juce::String& processedText, const juce::String& error) {
        if (success && processedText.isNotEmpty()) {
            // Process the Gemini-enhanced text through keyword mapper
            addChange("Gemini: " + processedText, juce::Colours::lightgreen);
            AudioParameters params = processText(processedText, baseIntensity);
            if (callback) {
                callback(params);
            }
        } else {
            // Gemini failed, fall back to direct keyword processing
            // Log the error for debugging
            juce::String errorMsg = error.isNotEmpty() ? error : "Unknown error";
            addChange("LLM failed: " + errorMsg + " (using direct mapping)", juce::Colours::orange);
            AudioParameters params = processText(textCopy, baseIntensity);
            if (callback) {
                callback(params);
            }
        }
    });
}

void KeywordMapper::reset() {
    recentChanges.clear();
}

