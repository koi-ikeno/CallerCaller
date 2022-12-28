/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CallerCallerProcessorEditor::CallerCallerProcessorEditor (
    CallerCallerProcessor& p,
    juce::AudioProcessorValueTreeState& vts 
)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts), 
    lengthLabel("length", "length\n(ms)"),
    thresholdLabel("threshold", "threshold\n(dB)"),
    infoLabel("info", "Ver 0.1\nby koisignal")
{
    lengthSliderAttachment.reset(new SliderAttachment(valueTreeState, "val_threshold", lengthSlider)); 
    lengthSlider.setNumDecimalPlacesToDisplay(2);
    lengthSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour(0xff000011));
    lengthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    lengthSlider.setTextBoxStyle(juce:: Slider::TextEntryBoxPosition::TextBoxBelow,
        false, 50, 14);
    addAndMakeVisible(lengthSlider); 

    thresholdSliderAttachment.reset(new SliderAttachment(valueTreeState, "val_length", thresholdSlider)); 
    thresholdSlider.setNumDecimalPlacesToDisplay(2);
    thresholdSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colour(0xff000011));
    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    thresholdSlider.setTextBoxStyle(juce:: Slider::TextEntryBoxPosition::TextBoxBelow,
        false, 50, 14);
    addAndMakeVisible(thresholdSlider); 
    
    addAndMakeVisible(lengthLabel);
    addAndMakeVisible(thresholdLabel);
    addAndMakeVisible(infoLabel);

    lengthLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xff000011));
    thresholdLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xff000011));
    infoLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xff666666));
    infoLabel.setFont(juce::Font(11.0f));

    lengthLabel.setJustificationType(juce::Justification::Flags::centred);
    thresholdLabel.setJustificationType(juce::Justification::Flags::centred);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (190, 340);
    resized();
}

CallerCallerProcessorEditor::~CallerCallerProcessorEditor()
{
}

//==============================================================================
void CallerCallerProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
    juce::Image background = juce::ImageCache::getFromMemory(
        BinaryData::logo183_74_png,
        BinaryData::logo183_74_pngSize
    );
    g.drawImageAt(background, 4, 260);  
}

void CallerCallerProcessorEditor::resized()
{
    lengthSlider.setBounds(35, 8, 50, 200); 
    thresholdSlider.setBounds(105, 8, 50, 200); 
    
    thresholdLabel.setBounds(20, 210, 70, 42);
    lengthLabel.setBounds(90, 210, 70, 42);    

    infoLabel.setBounds(116, 295, 80, 42);
}
