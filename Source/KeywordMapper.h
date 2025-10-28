#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include "ChangesLogger.h"
#include <map>
#include <vector>

struct AudioParameters {
    // EQ Parameters
    struct EQ {
        float highShelfFreq = 10000.0f;
        float highShelfGain = 0.0f;
        float midFreq = 2000.0f;
        float midGain = 0.0f;
        float midQ = 1.0f;
        float lowShelfFreq = 100.0f;
        float lowShelfGain = 0.0f;
    } eq;
    
    // Compressor Parameters
    struct Compressor {
        float threshold = 0.0f;
        float ratio = 1.0f;
        float attack = 10.0f;
        float release = 100.0f;
        float makeupGain = 0.0f;
        bool enabled = false;
    } compressor;
    
    // Reverb Parameters
    struct Reverb {
        float roomSize = 0.0f;
        float damping = 0.0f;
        float width = 1.0f;
        float wetLevel = 0.0f;
        float dryLevel = 1.0f;
        bool enabled = false;
    } reverb;
    
    float intensity = 1.0f; // Global intensity multiplier
};


class KeywordMapper {
public:
    KeywordMapper();
    
    // Process text input and return audio parameters
    AudioParameters processText(const juce::String& text, float baseIntensity = 1.0f);
    
    // Get list of changes that were applied
    std::vector<ChangeLog> getRecentChanges() const;
    
    // Reset all parameters
    void reset();
    
private:
    std::vector<ChangeLog> recentChanges;
    
    // Keyword detection functions
    bool containsKeyword(const juce::String& text, const std::vector<juce::String>& keywords);
    float extractIntensity(const juce::String& text);
    
    // Processing functions for each effect type
    void processBrightnessKeywords(const juce::String& text, AudioParameters& params);
    void processWarmthKeywords(const juce::String& text, AudioParameters& params);
    void processReverbKeywords(const juce::String& text, AudioParameters& params);
    void processCompressorKeywords(const juce::String& text, AudioParameters& params);
    void processBassKeywords(const juce::String& text, AudioParameters& params);
    void processPresenceKeywords(const juce::String& text, AudioParameters& params);
    
    // Keyword lists
    std::vector<juce::String> brightnessKeywords;
    std::vector<juce::String> warmthKeywords;
    std::vector<juce::String> reverbKeywords;
    std::vector<juce::String> compressorKeywords;
    std::vector<juce::String> bassKeywords;
    std::vector<juce::String> presenceKeywords;
    
    // Helper to add change log
    void addChange(const juce::String& description, const juce::Colour& color = juce::Colours::white);
};

