#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cstdlib>

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
    // SECURITY: API key should be loaded from environment variable or secure storage
    // For development, check if environment variable is set, otherwise use hardcoded key
    // WARNING: Never commit API keys to version control!
    
    juce::String apiKey;
    
    // Try to get from environment variable first (more secure)
    const char* envKey = std::getenv("GEMINI_API_KEY");
    
    if (envKey != nullptr && juce::String(envKey).trim().isNotEmpty()) {
        apiKey = juce::String(envKey).trim();
    } else {
        // Fallback to hardcoded key (for development only)
        // TODO: Remove this before committing to public repository
        apiKey = "AIzaSyAfQd48-sPRWVnDNPwmj1yRJH_klDpIei4";
    }
    
    if (apiKey.isNotEmpty()) {
        setGeminiApiKey(apiKey);
    }
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
    
    // Reset EQ to flat response first
    equalizer.setHighShelf(10000.0f, 0.0f);
    equalizer.setMidPeak(2000.0f, 0.0f, 1.0f);
    equalizer.setLowShelf(100.0f, 0.0f);
    
    // Apply EQ settings (always apply, even if 0, to ensure proper reset)
    equalizer.setHighShelf(params.eq.highShelfFreq, params.eq.highShelfGain);
    equalizer.setMidPeak(params.eq.midFreq, params.eq.midGain, params.eq.midQ);
    equalizer.setLowShelf(params.eq.lowShelfFreq, params.eq.lowShelfGain);
    
    // Apply compressor settings
    compressor.setThreshold(params.compressor.threshold);
    compressor.setRatio(params.compressor.ratio);
    compressor.setAttack(params.compressor.attack);
    compressor.setRelease(params.compressor.release);
    compressor.setMakeupGain(params.compressor.makeupGain);
    compressor.setEnabled(params.compressor.enabled);
    
    // Apply reverb settings
    reverbProcessor.setRoomSize(params.reverb.roomSize);
    reverbProcessor.setDamping(params.reverb.damping);
    reverbProcessor.setWidth(params.reverb.width);
    reverbProcessor.setWetLevel(params.reverb.wetLevel);
    reverbProcessor.setDryLevel(params.reverb.dryLevel);
    reverbProcessor.setEnabled(params.reverb.enabled);
}

void SonaraAudioProcessor::setIntensity(float intensity)
{
    currentIntensity = intensity;
}

std::vector<ChangeLog> SonaraAudioProcessor::getChangeLog() const
{
    return keywordMapper.getRecentChanges();
}

void SonaraAudioProcessor::setGeminiApiKey(const juce::String& apiKey)
{
    keywordMapper.setGeminiApiKey(apiKey);
}

bool SonaraAudioProcessor::isGeminiEnabled() const
{
    return keywordMapper.isGeminiEnabled();
}

void SonaraAudioProcessor::processTextInputWithGemini(const juce::String& text, std::function<void()> onComplete)
{
    keywordMapper.processTextWithGemini(text, currentIntensity, [this, onComplete](const AudioParameters& params) {
        // Apply the parameters (same logic as processTextInput)
        // Reset EQ to flat response first
        equalizer.setHighShelf(10000.0f, 0.0f);
        equalizer.setMidPeak(2000.0f, 0.0f, 1.0f);
        equalizer.setLowShelf(100.0f, 0.0f);
        
        // Apply EQ settings
        equalizer.setHighShelf(params.eq.highShelfFreq, params.eq.highShelfGain);
        equalizer.setMidPeak(params.eq.midFreq, params.eq.midGain, params.eq.midQ);
        equalizer.setLowShelf(params.eq.lowShelfFreq, params.eq.lowShelfGain);
        
        // Apply compressor settings
        compressor.setThreshold(params.compressor.threshold);
        compressor.setRatio(params.compressor.ratio);
        compressor.setAttack(params.compressor.attack);
        compressor.setRelease(params.compressor.release);
        compressor.setMakeupGain(params.compressor.makeupGain);
        compressor.setEnabled(params.compressor.enabled);
        
        // Apply reverb settings
        reverbProcessor.setRoomSize(params.reverb.roomSize);
        reverbProcessor.setDamping(params.reverb.damping);
        reverbProcessor.setWidth(params.reverb.width);
        reverbProcessor.setWetLevel(params.reverb.wetLevel);
        reverbProcessor.setDryLevel(params.reverb.dryLevel);
        reverbProcessor.setEnabled(params.reverb.enabled);
        
        // Call completion callback if provided
        if (onComplete) {
            onComplete();
        }
    });
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SonaraAudioProcessor();
}

