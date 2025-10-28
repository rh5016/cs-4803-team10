#include "Compressor.h"

Compressor::Compressor() {
    reset();
}

void Compressor::setSampleRate(double sampleRate) {
    juce::ignoreUnused(sampleRate);
}

void Compressor::reset() {
    updateCompressorSettings();
}

void Compressor::setThreshold(float thresholdDb) {
    threshold = thresholdDb;
    updateCompressorSettings();
}

void Compressor::setRatio(float r) {
    ratio = r;
    updateCompressorSettings();
}

void Compressor::setAttack(float attackMs) {
    attack = attackMs;
    updateCompressorSettings();
}

void Compressor::setRelease(float releaseMs) {
    release = releaseMs;
    updateCompressorSettings();
}

void Compressor::setMakeupGain(float gainDb) {
    makeupGain = gainDb;
}

void Compressor::setEnabled(bool en) {
    enabled = en;
}

void Compressor::processBlock(juce::AudioBuffer<float>& buffer) {
    if (!enabled) return;
    
    // Simplified compressor - apply threshold-based gain reduction
    float thresholdGain = juce::Decibels::decibelsToGain(threshold);
    float gainReduction = 1.0f / ratio;
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            float sampleValue = std::abs(channelData[sample]);
            if (sampleValue > thresholdGain) {
                channelData[sample] *= gainReduction;
            }
        }
    }
    
    // Apply makeup gain
    if (makeupGain != 0.0f) {
        float gain = juce::Decibels::decibelsToGain(makeupGain);
        buffer.applyGain(gain);
    }
}

void Compressor::updateCompressorSettings() {
    // Simplified compressor - just apply a static gain reduction when signal exceeds threshold
    // TODO: Implement full compressor with attack/release/ratio
    juce::ignoreUnused(threshold, ratio, attack, release);
}
