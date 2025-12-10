#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <cmath>

class Bitcrusher {
public:
    Bitcrusher();
    
    void setSampleRate(double sampleRate);
    void reset();
    
    // Bitcrusher Parameters
    void setBitDepth(int bits); // 1-32 bits (lower = more distortion)
    void setDecimationFactor(float factor); // 1.0 = no decimation, higher = more aliasing
    void setEnabled(bool enabled);
    
    void processBlock(juce::AudioBuffer<float>& buffer);
    
private:
    bool enabled = false;
    double sampleRate = 44100.0;
    
    // Bit depth reduction
    int bitDepth = 16; // Default 16-bit
    
    // Sample rate decimation
    float decimationFactor = 1.0f; // 1.0 = no decimation
    std::vector<float> decimationCounters; // Per-channel counters
    std::vector<float> lastSamples; // Per-channel last samples
    
    // Helper functions
    float quantizeSample(float sample, int bits);
    float decimateSample(float sample, float& counter, float factor, float& lastValue);
};

