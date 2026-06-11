#include "DelayProcessor.h"

#include <cmath>

DelayProcessor::DelayProcessor()
{
    reset();
}

void DelayProcessor::setSampleRate(double sampleRate)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    maxDelaySamples = (int)(currentSampleRate * 2.0);
    delayBuffer.setSize(2, maxDelaySamples + 1, false, true, true);
    reset();
}

void DelayProcessor::reset()
{
    delayBuffer.clear();
    writePosition = 0;
}

void DelayProcessor::setDelayTimeMs(float delayMs)
{
    delayTimeMs = juce::jlimit(1.0f, 2000.0f, delayMs);
}

void DelayProcessor::setFeedback(float newFeedback)
{
    feedback = juce::jlimit(0.0f, 0.95f, newFeedback);
}

void DelayProcessor::setMix(float newMix)
{
    mix = juce::jlimit(0.0f, 1.0f, newMix);
}

void DelayProcessor::setEnabled(bool en)
{
    enabled = en;
}

void DelayProcessor::ensureBufferSize(int numChannels)
{
    if (numChannels <= 0)
        numChannels = 1;

    if (delayBuffer.getNumChannels() != numChannels || delayBuffer.getNumSamples() != maxDelaySamples + 1)
    {
        delayBuffer.setSize(numChannels, maxDelaySamples + 1, false, true, true);
        delayBuffer.clear();
        writePosition = 0;
    }
}

int DelayProcessor::getDelaySamples() const
{
    const int delaySamples = (int)std::round(delayTimeMs * 0.001 * currentSampleRate);
    return juce::jlimit(1, juce::jmax(1, maxDelaySamples), delaySamples);
}

void DelayProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    if (!enabled || buffer.getNumSamples() == 0)
        return;

    ensureBufferSize(buffer.getNumChannels());

    const int delaySamples = getDelaySamples();
    const int bufferLength = delayBuffer.getNumSamples();
    const float dryMix = 1.0f - mix;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        const int readPosition = (writePosition - delaySamples + bufferLength) % bufferLength;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
            float* delayData = delayBuffer.getWritePointer(channel);

            const float input = channelData[sample];
            const float delayed = delayData[readPosition];
            const float output = input * dryMix + delayed * mix;

            delayData[writePosition] = input + delayed * feedback;
            channelData[sample] = output;
        }

        writePosition = (writePosition + 1) % bufferLength;
    }
}