#include "KeywordMapper.h"

KeywordMapper::KeywordMapper() {
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
    
    // Apply intensity to all parameters
    params.eq.highShelfGain *= intensity;
    params.eq.midGain *= intensity;
    params.eq.lowShelfGain *= intensity;
    params.reverb.wetLevel *= intensity;
    params.reverb.roomSize *= intensity;
    params.compressor.ratio *= intensity;
    
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
    if (text.contains("slight") || text.contains("little") || text.contains("bit")) return 0.5f;
    if (text.contains("more") || text.contains("much")) return 1.5f;
    if (text.contains("very") || text.contains("super") || text.contains("extreme")) return 2.0f;
    if (text.contains("less") || text.contains("reduce")) return -1.0f;
    return 1.0f;
}

void KeywordMapper::processBrightnessKeywords(const juce::String& text, AudioParameters& params) {
    if (containsKeyword(text, brightnessKeywords)) {
        if (text.contains("more air") || text.contains("airy") || text.contains("airiness")) {
            params.eq.highShelfFreq = 10000.0f;
            params.eq.highShelfGain = 4.0f;
            addChange("High Shelf 10kHz +4.0dB", juce::Colour(0xff8affb4));
        } else if (text.contains("sparkle") || text.contains("sparkly")) {
            params.eq.highShelfFreq = 12000.0f;
            params.eq.highShelfGain = 3.0f;
            addChange("High Shelf 12kHz +3.0dB", juce::Colour(0xff8affb4));
        } else if (text.contains("bright") || text.contains("brightness")) {
            params.eq.highShelfFreq = 8000.0f;
            params.eq.highShelfGain = 2.5f;
            addChange("High Shelf 8kHz +2.5dB", juce::Colour(0xff8affb4));
        }
        
        if (text.contains("clarity") || text.contains("clear")) {
            params.eq.midFreq = 2500.0f;
            params.eq.midGain = 2.0f;
            params.eq.midQ = 1.5f;
            addChange("Peak 2.5kHz +2.0dB Q:1.5", juce::Colour(0xff8affb4));
        }
    }
    
    if (text.contains("dull") || text.contains("dark")) {
        params.eq.highShelfFreq = 8000.0f;
        params.eq.highShelfGain = -3.0f;
        addChange("High Shelf 8kHz -3.0dB", juce::Colour(0xff8affb4));
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
    if (containsKeyword(text, reverbKeywords)) {
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
        
        if (text.contains("punch") || text.contains("punchy")) {
            params.compressor.threshold = -12.0f;
            params.compressor.ratio = 4.0f;
            params.compressor.attack = 3.0f;
            params.compressor.release = 60.0f;
            params.compressor.makeupGain = 2.0f;
            addChange("Compressor: Ratio 4:1, Attack 3ms, Release 60ms, +2dB makeup", juce::Colour(0xffff6b35));
        } else if (text.contains("glue") || text.contains("tight")) {
            params.compressor.threshold = -8.0f;
            params.compressor.ratio = 2.5f;
            params.compressor.attack = 10.0f;
            params.compressor.release = 100.0f;
            params.compressor.makeupGain = 1.0f;
            addChange("Compressor: Ratio 2.5:1, Attack 10ms, Release 100ms", juce::Colour(0xffff6b35));
        } else if (text.contains("level") || text.contains("even")) {
            params.compressor.threshold = -10.0f;
            params.compressor.ratio = 3.0f;
            params.compressor.attack = 20.0f;
            params.compressor.release = 150.0f;
            params.compressor.makeupGain = 1.5f;
            addChange("Compressor: Ratio 3:1, Attack 20ms, Release 150ms", juce::Colour(0xffff6b35));
        }
    }
}

void KeywordMapper::processBassKeywords(const juce::String& text, AudioParameters& params) {
    if (containsKeyword(text, bassKeywords)) {
        if (text.contains("more bass") || text.contains("low end") || text.contains("lows")) {
            params.eq.lowShelfFreq = 100.0f;
            params.eq.lowShelfGain = 4.0f;
            addChange("Low Shelf 100Hz +4.0dB", juce::Colour(0xff4fc3f7));
        }
        
        if (text.contains("deeper") || text.contains("deep")) {
            params.eq.lowShelfFreq = 60.0f;
            params.eq.lowShelfGain = 3.0f;
            addChange("Low Shelf 60Hz +3.0dB", juce::Colour(0xff4fc3f7));
        }
        
        if (text.contains("boom") || text.contains("boomy")) {
            params.eq.lowShelfFreq = 150.0f;
            params.eq.lowShelfGain = -3.0f;
            addChange("Low Shelf 150Hz -3.0dB", juce::Colour(0xff4fc3f7));
        }
    }
}

void KeywordMapper::processPresenceKeywords(const juce::String& text, AudioParameters& params) {
    if (containsKeyword(text, presenceKeywords)) {
        if (text.contains("presence") || text.contains("forward") || text.contains("vocal")) {
            params.eq.midFreq = 3000.0f;
            params.eq.midGain = 3.0f;
            params.eq.midQ = 2.0f;
            addChange("Peak 3kHz +3.0dB Q:2.0", juce::Colour(0xffffb74d));
        }
    }
}

void KeywordMapper::addChange(const juce::String& description, const juce::Colour& color) {
    recentChanges.push_back({description, color});
}

std::vector<ChangeLog> KeywordMapper::getRecentChanges() const {
    return recentChanges;
}

void KeywordMapper::reset() {
    recentChanges.clear();
}

