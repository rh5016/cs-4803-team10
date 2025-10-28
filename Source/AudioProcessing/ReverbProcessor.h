#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class ReverbProcessor {
public:
    ReverbProcessor();
    
    void setSampleRate(double sampleRate);
    void reset();
    
    // Reverb Parameters
    void setRoomSize(float roomSize);
    void setDamping(float damping);
    void setWidth(float width);
    void setWetLevel(float wetLevel);
    void setDryLevel(float dryLevel);
    void setEnabled(bool enabled);
    
    void processBlock(juce::AudioBuffer<float>& buffer);
    
private:
    juce::dsp::Reverb reverb;
    
    float roomSize = 0.0f;
    float damping = 0.0f;
    float width = 1.0f;
    float wetLevel = 0.0f;
    float dryLevel = 1.0f;
    bool enabled = false;
    
    void updateReverbSettings();
};

