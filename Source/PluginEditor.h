#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class SonaraAudioProcessorEditor : public juce::AudioProcessorEditor,
                                   public juce::TextEditor::Listener,
                                   public juce::Slider::Listener
{
public:
    SonaraAudioProcessorEditor(SonaraAudioProcessor&);
    ~SonaraAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    SonaraAudioProcessor& audioProcessor;
    
    // UI Components
    juce::Label titleLabel;
    juce::TextEditor textInput;
    juce::Slider intensitySlider;
    juce::Label intensityLabel;
    juce::Label changesTitleLabel;
    juce::TextEditor changesDisplay;
    
    // Custom look and feel
    class CustomLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CustomLookAndFeel()
        {
            setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff1a1a2e));
            setColour(juce::TextEditor::textColourId, juce::Colour(0xff8affb4));
            setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff10b981));
            setColour(juce::Slider::thumbColourId, juce::Colour(0xff8affb4));
            setColour(juce::Slider::trackColourId, juce::Colour(0xff10b981));
            setColour(juce::Slider::backgroundColourId, juce::Colour(0xff0f1419));
        }
    } lookAndFeel;
    
    void updateChangesDisplay();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SonaraAudioProcessorEditor)
};

