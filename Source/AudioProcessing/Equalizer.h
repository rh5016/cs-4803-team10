#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class Equalizer {
public:
    Equalizer();
    
    void setSampleRate(double sampleRate);
    void reset();
    
    // EQ Parameters
    void setHighShelf(float frequency, float gainDb);
    void setMidPeak(float frequency, float gainDb, float q);
    void setLowShelf(float frequency, float gainDb);
    
    void processBlock(juce::AudioBuffer<float>& buffer);
    
private:
    juce::dsp::ProcessorChain<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Filter<float>
    > filterChain;
    
    juce::dsp::IIR::Coefficients<float>::Ptr highShelfCoeffs;
    juce::dsp::IIR::Coefficients<float>::Ptr midPeakCoeffs;
    juce::dsp::IIR::Coefficients<float>::Ptr lowShelfCoeffs;
    
    double currentSampleRate = 44100.0;
    
    void updateFilters();
};

