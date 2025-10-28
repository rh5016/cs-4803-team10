#include "PluginProcessor.h"
#include "PluginEditor.h"

SonaraAudioProcessor::SonaraAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SonaraAudioProcessor::~SonaraAudioProcessor()
{
}

const juce::String SonaraAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SonaraAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SonaraAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SonaraAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SonaraAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SonaraAudioProcessor::getNumPrograms()
{
    return 1;
}

int SonaraAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SonaraAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SonaraAudioProcessor::getProgramName(int index)
{
    return {};
}

void SonaraAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void SonaraAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    equalizer.setSampleRate(sampleRate);
    compressor.setSampleRate(sampleRate);
    reverbProcessor.setSampleRate(sampleRate);
    
    // Reset processors
    equalizer.reset();
    compressor.reset();
    reverbProcessor.reset();
}

void SonaraAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SonaraAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SonaraAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    // Process audio through chain
    equalizer.processBlock(buffer);
    compressor.processBlock(buffer);
    reverbProcessor.processBlock(buffer);
}

bool SonaraAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SonaraAudioProcessor::createEditor()
{
    return new SonaraAudioProcessorEditor (*this);
}

void SonaraAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Save state
    juce::ignoreUnused(destData);
}

void SonaraAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Restore state
    juce::ignoreUnused(data, sizeInBytes);
}

void SonaraAudioProcessor::processTextInput(const juce::String& text)
{
    // Process text and apply parameters
    AudioParameters params = keywordMapper.processText(text, currentIntensity);
    
    // Apply EQ settings
    if (params.eq.highShelfGain != 0.0f) {
        equalizer.setHighShelf(params.eq.highShelfFreq, params.eq.highShelfGain);
    }
    if (params.eq.midGain != 0.0f) {
        equalizer.setMidPeak(params.eq.midFreq, params.eq.midGain, params.eq.midQ);
    }
    if (params.eq.lowShelfGain != 0.0f) {
        equalizer.setLowShelf(params.eq.lowShelfFreq, params.eq.lowShelfGain);
    }
    
    // Apply compressor settings
    if (params.compressor.enabled) {
        compressor.setThreshold(params.compressor.threshold);
        compressor.setRatio(params.compressor.ratio);
        compressor.setAttack(params.compressor.attack);
        compressor.setRelease(params.compressor.release);
        compressor.setMakeupGain(params.compressor.makeupGain);
        compressor.setEnabled(true);
    }
    
    // Apply reverb settings
    if (params.reverb.enabled) {
        reverbProcessor.setRoomSize(params.reverb.roomSize);
        reverbProcessor.setDamping(params.reverb.damping);
        reverbProcessor.setWidth(params.reverb.width);
        reverbProcessor.setWetLevel(params.reverb.wetLevel);
        reverbProcessor.setDryLevel(params.reverb.dryLevel);
        reverbProcessor.setEnabled(true);
    }
}

void SonaraAudioProcessor::setIntensity(float intensity)
{
    currentIntensity = intensity;
}

std::vector<ChangeLog> SonaraAudioProcessor::getChangeLog() const
{
    return keywordMapper.getRecentChanges();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SonaraAudioProcessor();
}

