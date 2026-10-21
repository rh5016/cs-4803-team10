#include "PitchShifter.h"
#include <algorithm>
#include <cmath>

void PitchShifter::prepare(double sampleRate, int maximumChannels)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    delayLength = juce::jmax(2, static_cast<int>(std::ceil((minimumDelaySeconds + delayRangeSeconds) * currentSampleRate)) + 2);
    delayBuffers.assign(static_cast<size_t>(juce::jmax(1, maximumChannels)),
                        std::vector<float>(static_cast<size_t>(delayLength), 0.0f));
    reset();
}

void PitchShifter::reset()
{
    for (auto& channelBuffer : delayBuffers)
        std::fill(channelBuffer.begin(), channelBuffer.end(), 0.0f);

    writePosition = 0;
    phase = 0.0;
}

void PitchShifter::setEnabled(bool shouldBeEnabled)
{
    enabled.store(shouldBeEnabled, std::memory_order_relaxed);
}

void PitchShifter::setSemitones(float newSemitones)
{
    semitones.store(juce::jlimit(-12.0f, 12.0f, newSemitones), std::memory_order_relaxed);
}

float PitchShifter::getSemitones() const
{
    return semitones.load(std::memory_order_relaxed);
}

void PitchShifter::processBlock(juce::AudioBuffer<float>& buffer)
{
    if (!enabled.load(std::memory_order_relaxed) || delayLength <= 0)
        return;

    const float shift = semitones.load(std::memory_order_relaxed);
    if (std::abs(shift) < 0.001f)
        return;

    const double ratio = std::pow(2.0, static_cast<double>(shift) / 12.0);
    const double delayRangeSamples = delayRangeSeconds * currentSampleRate;
    const double minimumDelaySamples = minimumDelaySeconds * currentSampleRate;
    const double phaseIncrement = (1.0 - ratio) / delayRangeSamples;
    const int channelsToProcess = juce::jmin(buffer.getNumChannels(), static_cast<int>(delayBuffers.size()));

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        double headPhaseA = phase;
        double headPhaseB = phase + 0.5;
        if (headPhaseB >= 1.0)
            headPhaseB -= 1.0;

        const double weightA = 1.0 - std::abs(2.0 * headPhaseA - 1.0);
        const double weightB = 1.0 - std::abs(2.0 * headPhaseB - 1.0);
        const double delayA = minimumDelaySamples + headPhaseA * delayRangeSamples;
        const double delayB = minimumDelaySamples + headPhaseB * delayRangeSamples;

        for (int channel = 0; channel < channelsToProcess; ++channel)
        {
            auto& delayBuffer = delayBuffers[static_cast<size_t>(channel)];
            const float input = buffer.getSample(channel, sample);
            delayBuffer[static_cast<size_t>(writePosition)] = input;

            const auto readDelayedSample = [&delayBuffer, this](double delaySamples) {
                double readPosition = static_cast<double>(writePosition) - delaySamples;
                while (readPosition < 0.0)
                    readPosition += delayLength;

                const int indexA = static_cast<int>(readPosition) % delayLength;
                const int indexB = (indexA + 1) % delayLength;
                const float fraction = static_cast<float>(readPosition - std::floor(readPosition));
                return delayBuffer[static_cast<size_t>(indexA)] * (1.0f - fraction)
                     + delayBuffer[static_cast<size_t>(indexB)] * fraction;
            };

            const float outputA = readDelayedSample(delayA);
            const float outputB = readDelayedSample(delayB);
            buffer.setSample(channel, sample,
                             static_cast<float>(outputA * weightA + outputB * weightB));
        }

        writePosition = (writePosition + 1) % delayLength;
        phase += phaseIncrement;
        phase -= std::floor(phase);
    }
}
