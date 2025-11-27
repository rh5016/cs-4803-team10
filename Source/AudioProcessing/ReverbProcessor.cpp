#include "ReverbProcessor.h"

ReverbProcessor::ReverbProcessor() {
    reset();
}

void ReverbProcessor::setSampleRate(double sampleRate) {
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    isPrepared = false;
}

void ReverbProcessor::reset() {
    reverb.reset();
    updateReverbSettings();
}

void ReverbProcessor::setRoomSize(float r) {
    roomSize = r;
    updateReverbSettings();
}

void ReverbProcessor::setDamping(float d) {
    damping = d;
    updateReverbSettings();
}

void ReverbProcessor::setWidth(float w) {
    width = w;
    updateReverbSettings();
}

void ReverbProcessor::setWetLevel(float wet) {
    wetLevel = wet;
    updateReverbSettings();
}

void ReverbProcessor::setDryLevel(float dry) {
    dryLevel = dry;
    updateReverbSettings();
}

void ReverbProcessor::setEnabled(bool en) {
    enabled = en;
}

void ReverbProcessor::processBlock(juce::AudioBuffer<float>& buffer) {
    if (!enabled) return;
    
    juce::dsp::ProcessSpec processSpec;
    processSpec.sampleRate = spec.sampleRate > 0 ? spec.sampleRate : 44100.0;
    processSpec.maximumBlockSize = (juce::uint32)buffer.getNumSamples();
    processSpec.numChannels = (juce::uint32)buffer.getNumChannels();
    
    prepareIfNeeded(processSpec);
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    reverb.process(context);
}

void ReverbProcessor::prepareIfNeeded(const juce::dsp::ProcessSpec& processSpec) {
    if (!isPrepared || spec.sampleRate != processSpec.sampleRate || 
        spec.maximumBlockSize != processSpec.maximumBlockSize || 
        spec.numChannels != processSpec.numChannels) {
        spec = processSpec;
        reverb.prepare(spec);
        isPrepared = true;
        updateReverbSettings();
    }
}

void ReverbProcessor::updateReverbSettings() {
    juce::dsp::Reverb::Parameters params;
    params.roomSize = roomSize;
    params.damping = damping;
    params.width = width;
    params.wetLevel = wetLevel;
    params.dryLevel = dryLevel;
    reverb.setParameters(params);
}

