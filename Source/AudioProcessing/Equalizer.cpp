#include "Equalizer.h"

Equalizer::Equalizer() {
    reset();
}

void Equalizer::setSampleRate(double sampleRate) {
    currentSampleRate = sampleRate;
    updateFilters();
}

void Equalizer::reset() {
    filterChain.reset();
    
    // Initialize with flat response
    highShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(currentSampleRate, 10000.0f, 1.0f, 1.0f);
    midPeakCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, 1000.0f, 1.0f, 1.0f);
    lowShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(currentSampleRate, 100.0f, 1.0f, 1.0f);
    
    updateFilters();
}

void Equalizer::setHighShelf(float frequency, float gainDb) {
    float gain = juce::Decibels::decibelsToGain(gainDb);
    highShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(currentSampleRate, frequency, 1.0f, gain);
    updateFilters();
}

void Equalizer::setMidPeak(float frequency, float gainDb, float q) {
    float gain = juce::Decibels::decibelsToGain(gainDb);
    midPeakCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, frequency, q, gain);
    updateFilters();
}

void Equalizer::setLowShelf(float frequency, float gainDb) {
    float gain = juce::Decibels::decibelsToGain(gainDb);
    lowShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(currentSampleRate, frequency, 1.0f, gain);
    updateFilters();
}

void Equalizer::processBlock(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    filterChain.process(context);
}

void Equalizer::updateFilters() {
    *filterChain.get<0>().coefficients = *highShelfCoeffs;
    *filterChain.get<1>().coefficients = *midPeakCoeffs;
    *filterChain.get<2>().coefficients = *lowShelfCoeffs;
}

