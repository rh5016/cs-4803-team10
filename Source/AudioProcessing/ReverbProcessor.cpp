#include "ReverbProcessor.h"

ReverbProcessor::ReverbProcessor() {
    reset();
}

void ReverbProcessor::setSampleRate(double sampleRate) {
    juce::ignoreUnused(sampleRate);
    // Reverb sample rate is set automatically by JUCE's DSP module
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
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    reverb.process(context);
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

