#include "Compressor.h"

Compressor::Compressor() {
    reset();
}

void Compressor::setSampleRate(double sampleRate) {
    currentSampleRate = sampleRate;
    envelope = 0.0f;
}

void Compressor::reset() {
    envelope = 0.0f;
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
    
    float thresholdGain = juce::Decibels::decibelsToGain(threshold);
    float attackCoeff = std::exp(-1.0f / (attack * 0.001f * (float)currentSampleRate));
    float releaseCoeff = std::exp(-1.0f / (release * 0.001f * (float)currentSampleRate));
    float makeupGainLinear = juce::Decibels::decibelsToGain(makeupGain);
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            float input = channelData[sample];
            float inputLevel = std::abs(input);
            
            // Envelope follower with attack/release
            if (inputLevel > envelope) {
                envelope = inputLevel + (envelope - inputLevel) * attackCoeff;
            } else {
                envelope = inputLevel + (envelope - inputLevel) * releaseCoeff;
            }
            
            // Calculate gain reduction
            float gainReduction = 1.0f;
            if (envelope > thresholdGain) {
                float overThreshold = envelope - thresholdGain;
                float compressedLevel = thresholdGain + (overThreshold / ratio);
                gainReduction = compressedLevel / envelope;
            }
            
            // Apply gain reduction and makeup gain
            channelData[sample] = input * gainReduction * makeupGainLinear;
        }
    }
}

void Compressor::updateCompressorSettings() {
    // Settings are applied in processBlock
}
