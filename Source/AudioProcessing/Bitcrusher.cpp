#include "Bitcrusher.h"
#include <algorithm>

Bitcrusher::Bitcrusher() {
    reset();
}

void Bitcrusher::setSampleRate(double sr) {
    sampleRate = sr;
    reset();
}

void Bitcrusher::reset() {
    decimationCounters.clear();
    lastSamples.clear();
}

void Bitcrusher::setBitDepth(int bits) {
    bitDepth = juce::jlimit(1, 32, bits);
}

void Bitcrusher::setDecimationFactor(float factor) {
    decimationFactor = juce::jlimit(1.0f, 16.0f, factor);
}

void Bitcrusher::setEnabled(bool en) {
    enabled = en;
    if (!enabled) {
        reset();
    }
}

float Bitcrusher::quantizeSample(float sample, int bits) {
    if (bits >= 32) return sample; // No quantization
    
    // Clamp sample to [-1, 1]
    sample = juce::jlimit(-1.0f, 1.0f, sample);
    
    // Calculate number of quantization levels
    int levels = 1 << bits; // 2^bits
    float maxValue = static_cast<float>(levels - 1);
    
    // Quantize: map to integer, then back to float
    float quantized = std::round((sample + 1.0f) * 0.5f * maxValue);
    quantized = (quantized / maxValue) * 2.0f - 1.0f;
    
    return quantized;
}

float Bitcrusher::decimateSample(float sample, float& counter, float factor, float& lastValue) {
    counter += 1.0f;
    
    if (counter >= factor) {
        // Take this sample
        lastValue = sample;
        counter = 0.0f;
        return sample;
    } else {
        // Hold last sample (zero-order hold)
        return lastValue;
    }
}

void Bitcrusher::processBlock(juce::AudioBuffer<float>& buffer) {
    if (!enabled) return;
    
    int numChannels = buffer.getNumChannels();
    int numSamples = buffer.getNumSamples();
    
    // Initialize per-channel buffers if needed
    if (decimationCounters.size() != static_cast<size_t>(numChannels)) {
        decimationCounters.resize(numChannels, 0.0f);
        lastSamples.resize(numChannels, 0.0f);
    }
    
    for (int channel = 0; channel < numChannels; ++channel) {
        float* channelData = buffer.getWritePointer(channel);
        float& counter = decimationCounters[channel];
        float& lastVal = lastSamples[channel];
        
        for (int sample = 0; sample < numSamples; ++sample) {
            float input = channelData[sample];
            
            // Apply decimation first (sample rate reduction)
            float decimated = decimateSample(input, counter, decimationFactor, lastVal);
            
            // Then apply bit depth reduction (quantization)
            float quantized = quantizeSample(decimated, bitDepth);
            
            channelData[sample] = quantized;
        }
    }
}

