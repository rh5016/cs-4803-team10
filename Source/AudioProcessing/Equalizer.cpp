#include "Equalizer.h"

Equalizer::Equalizer() {
    reset();
}

void Equalizer::setSampleRate(double sampleRate) {
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    isPrepared = false;
    updateFilters();
}

void Equalizer::reset() {
    filterChain.reset();
    
    // Initialize with flat response
    double sampleRate = spec.sampleRate > 0 ? spec.sampleRate : 44100.0;
    highShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 10000.0f, 1.0f, 1.0f);
    midPeakCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 1000.0f, 1.0f, 1.0f);
    lowShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 100.0f, 1.0f, 1.0f);
    
    updateFilters();
}

void Equalizer::setHighShelf(float frequency, float gainDb) {
    float gain = juce::Decibels::decibelsToGain(gainDb);
    double sampleRate = spec.sampleRate > 0 ? spec.sampleRate : 44100.0;
    highShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, frequency, 1.0f, gain);
    updateFilters();
}

void Equalizer::setMidPeak(float frequency, float gainDb, float q) {
    float gain = juce::Decibels::decibelsToGain(gainDb);
    double sampleRate = spec.sampleRate > 0 ? spec.sampleRate : 44100.0;
    midPeakCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, frequency, q, gain);
    updateFilters();
}

void Equalizer::setLowShelf(float frequency, float gainDb) {
    float gain = juce::Decibels::decibelsToGain(gainDb);
    double sampleRate = spec.sampleRate > 0 ? spec.sampleRate : 44100.0;
    lowShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, frequency, 1.0f, gain);
    updateFilters();
}

void Equalizer::processBlock(juce::AudioBuffer<float>& buffer) {
    juce::dsp::ProcessSpec processSpec;
    processSpec.sampleRate = spec.sampleRate > 0 ? spec.sampleRate : 44100.0;
    processSpec.maximumBlockSize = (juce::uint32)buffer.getNumSamples();
    processSpec.numChannels = (juce::uint32)buffer.getNumChannels();
    
    prepareIfNeeded(processSpec);
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    filterChain.process(context);
}

void Equalizer::prepareIfNeeded(const juce::dsp::ProcessSpec& processSpec) {
    if (!isPrepared || spec.sampleRate != processSpec.sampleRate || 
        spec.maximumBlockSize != processSpec.maximumBlockSize || 
        spec.numChannels != processSpec.numChannels) {
        spec = processSpec;
        filterChain.prepare(spec);
        isPrepared = true;
        updateFilters();
    }
}

void Equalizer::updateFilters() {
    if (isPrepared && highShelfCoeffs && midPeakCoeffs && lowShelfCoeffs) {
        *filterChain.get<0>().coefficients = *highShelfCoeffs;
        *filterChain.get<1>().coefficients = *midPeakCoeffs;
        *filterChain.get<2>().coefficients = *lowShelfCoeffs;
    }
}

