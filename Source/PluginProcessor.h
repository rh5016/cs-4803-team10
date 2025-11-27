#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "AudioProcessing/Equalizer.h"
#include "AudioProcessing/Compressor.h"
#include "AudioProcessing/ReverbProcessor.h"
#include "KeywordMapper.h"
#include "ChangesLogger.h"
#include <functional>

class SonaraAudioProcessor : public juce::AudioProcessor
{
public:
    SonaraAudioProcessor();
    ~SonaraAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    // Custom parameters
    void processTextInput(const juce::String& text);
    void setIntensity(float intensity);
    std::vector<ChangeLog> getChangeLog() const;
    
    // Gemini LLM integration
    void setGeminiApiKey(const juce::String& apiKey);
    bool isGeminiEnabled() const;
    void processTextInputWithGemini(const juce::String& text, std::function<void()> onComplete = nullptr);
    
private:
    Equalizer equalizer;
    Compressor compressor;
    ReverbProcessor reverbProcessor;
    KeywordMapper keywordMapper;
    
    double currentSampleRate = 44100.0;
    float currentIntensity = 1.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SonaraAudioProcessor)
};

