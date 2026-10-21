#include "Bitcrusher.h"
#include <cmath>

void Bitcrusher::setEnabled(bool shouldBeEnabled)
{
    enabled.store(shouldBeEnabled, std::memory_order_relaxed);
}

void Bitcrusher::setBitDepth(int newBitDepth)
{
    bitDepth.store(juce::jlimit(1, 16, newBitDepth), std::memory_order_relaxed);
}

int Bitcrusher::getBitDepth() const
{
    return bitDepth.load(std::memory_order_relaxed);
}

void Bitcrusher::processBlock(juce::AudioBuffer<float>& buffer) const
{
    if (!enabled.load(std::memory_order_relaxed))
        return;

    const int depth = bitDepth.load(std::memory_order_relaxed);
    const float quantizationLevels = static_cast<float>((1u << depth) - 1u);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* samples = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float input = juce::jlimit(-1.0f, 1.0f, samples[sample]);
            const float normalized = (input + 1.0f) * 0.5f;
            const float quantized = std::round(normalized * quantizationLevels) / quantizationLevels;
            samples[sample] = quantized * 2.0f - 1.0f;
        }
    }
}
