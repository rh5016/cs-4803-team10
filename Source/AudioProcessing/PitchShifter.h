#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>
#include <vector>

// Delay-line pitch transposer. This is a lightweight effect rather than
// pitch correction: it shifts all incoming material by a selected interval.
class PitchShifter {
public:
    void prepare(double sampleRate, int maximumChannels);
    void reset();
    void setEnabled(bool shouldBeEnabled);
    void setSemitones(float semitones);
    float getSemitones() const;
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    static constexpr float minimumDelaySeconds = 0.01f;
    static constexpr float delayRangeSeconds = 0.06f;

    std::atomic<bool> enabled { false };
    std::atomic<float> semitones { 0.0f };
    double currentSampleRate = 44100.0;
    int delayLength = 0;
    int writePosition = 0;
    double phase = 0.0;
    std::vector<std::vector<float>> delayBuffers;
};
