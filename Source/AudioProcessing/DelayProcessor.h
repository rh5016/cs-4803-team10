#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class DelayProcessor {
public:
    DelayProcessor();

    void setSampleRate(double sampleRate);
    void reset();

    void setDelayTimeMs(float delayTimeMs);
    void setFeedback(float feedback);
    void setMix(float mix);
    void setEnabled(bool enabled);

    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    void ensureBufferSize(int numChannels);
    int getDelaySamples() const;

    juce::AudioBuffer<float> delayBuffer;
    double currentSampleRate = 44100.0;
    float delayTimeMs = 500.0f;
    float feedback = 0.35f;
    float mix = 0.25f;
    bool enabled = false;
    int writePosition = 0;
    int maxDelaySamples = 0;
};