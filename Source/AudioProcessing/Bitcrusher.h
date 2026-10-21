#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>

class Bitcrusher {
public:
    void setEnabled(bool shouldBeEnabled);
    void setBitDepth(int bitDepth);
    int getBitDepth() const;
    void processBlock(juce::AudioBuffer<float>& buffer) const;

private:
    std::atomic<bool> enabled { false };
    std::atomic<int> bitDepth { 8 };
};
