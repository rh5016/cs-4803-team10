#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class Compressor {
public:
    Compressor();
    
    void setSampleRate(double sampleRate);
    void reset();
    
    // Compressor Parameters
    void setThreshold(float thresholdDb);
    void setRatio(float ratio);
    void setAttack(float attackMs);
    void setRelease(float releaseMs);
    void setMakeupGain(float gainDb);
    void setEnabled(bool enabled);
    
    void processBlock(juce::AudioBuffer<float>& buffer);
    
private:
    // juce::dsp::Compressor<float> compressor;
    
    float threshold = 0.0f;
    float ratio = 1.0f;
    float attack = 10.0f;
    float release = 100.0f;
    float makeupGain = 0.0f;
    bool enabled = false;
    
    void updateCompressorSettings();
};

