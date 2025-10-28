#include "PluginProcessor.h"
#include "PluginEditor.h"

SonaraAudioProcessorEditor::SonaraAudioProcessorEditor(SonaraAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setLookAndFeel(&lookAndFeel);
    
    // Setup title
    titleLabel.setText("SONARA", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xff8affb4));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Setup text input
    textInput.setTextToShowWhenEmpty("Type your desired sound effect here...", juce::Colour(0x66999999));
    textInput.setFont(juce::Font(16.0f));
    textInput.addListener(this);
    addAndMakeVisible(textInput);
    
    // Setup intensity slider
    intensitySlider.setRange(0.0, 2.0, 0.1);
    intensitySlider.setValue(1.0);
    intensitySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    intensitySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    intensitySlider.addListener(this);
    addAndMakeVisible(intensitySlider);
    
    intensityLabel.setText("Intensity", juce::dontSendNotification);
    intensityLabel.setFont(juce::Font(14.0f));
    intensityLabel.setColour(juce::Label::textColourId, juce::Colour(0xffcbd5e1));
    addAndMakeVisible(intensityLabel);
    
    // Setup changes display
    changesTitleLabel.setText("Changes Applied:", juce::dontSendNotification);
    changesTitleLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    changesTitleLabel.setColour(juce::Label::textColourId, juce::Colour(0xffa78bfa));
    addAndMakeVisible(changesTitleLabel);
    
    changesDisplay.setReadOnly(true);
    changesDisplay.setMultiLine(true);
    changesDisplay.setReturnKeyStartsNewLine(true);
    changesDisplay.setFont(juce::Font(14.0f, juce::Font::plain));
    changesDisplay.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff0f1419));
    changesDisplay.setColour(juce::TextEditor::textColourId, juce::Colour(0xffcbd5e1));
    changesDisplay.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff1e293b));
    addAndMakeVisible(changesDisplay);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(500, 700);
}

SonaraAudioProcessorEditor::~SonaraAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void SonaraAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Dark gradient background
    juce::ColourGradient gradient(juce::Colour(0xff0a1628), 0, 0,
                                   juce::Colour(0xff000000), 0, (float)getHeight(),
                                   false);
    g.setGradientFill(gradient);
    g.fillAll();
    
    // Draw subtle grid
    g.setColour(juce::Colour(0x20ffffff));
    for (int y = 0; y < getHeight(); y += 40) {
        g.drawLine(0, (float)y, (float)getWidth(), (float)y, 0.5f);
    }
    for (int x = 0; x < getWidth(); x += 40) {
        g.drawLine((float)x, 0, (float)x, (float)getHeight(), 0.5f);
    }
    
    // Draw accent line at top
    g.setColour(juce::Colour(0xff10b981));
    g.fillRect(0, 0, getWidth(), 2);
}

void SonaraAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    titleLabel.setBounds(area.removeFromTop(50));
    area.removeFromTop(20);
    
    auto inputArea = area.removeFromTop(120);
    textInput.setBounds(inputArea);
    
    area.removeFromTop(20);
    
    auto sliderArea = area.removeFromTop(40);
    intensityLabel.setBounds(sliderArea.removeFromLeft(80));
    intensitySlider.setBounds(sliderArea);
    
    area.removeFromTop(30);
    
    changesTitleLabel.setBounds(area.removeFromTop(30));
    area.removeFromTop(10);
    
    changesDisplay.setBounds(area);
}

void SonaraAudioProcessorEditor::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    if (&editor == &textInput)
    {
        juce::String text = textInput.getText();
        if (text.trim().isNotEmpty())
        {
            float intensity = (float)intensitySlider.getValue();
            audioProcessor.processTextInput(text);
            audioProcessor.setIntensity(intensity);
            updateChangesDisplay();
        }
    }
}

void SonaraAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &intensitySlider)
    {
        juce::String currentText = textInput.getText();
        if (currentText.trim().isNotEmpty())
        {
            audioProcessor.processTextInput(currentText);
            audioProcessor.setIntensity((float)intensitySlider.getValue());
            updateChangesDisplay();
        }
    }
}

void SonaraAudioProcessorEditor::updateChangesDisplay()
{
    auto changes = audioProcessor.getChangeLog();
    juce::String changesText;
    
    if (changes.empty())
    {
        changesText = "No changes yet. Enter text to apply effects!";
    }
    else
    {
        for (const auto& change : changes)
        {
            changesText += change.description + "\n";
        }
    }
    
    changesDisplay.setText(changesText);
    changesDisplay.setColour(juce::TextEditor::textColourId, juce::Colour(0xffcbd5e1));
}

