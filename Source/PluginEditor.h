/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CallerCallerProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CallerCallerProcessorEditor (CallerCallerProcessor&,
        juce::AudioProcessorValueTreeState& vts); // juce::AudioÅ`vtsÇí«â¡
    ~CallerCallerProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CallerCallerProcessor& audioProcessor;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment; 
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment; 

    juce::AudioProcessorValueTreeState& valueTreeState; 

    juce::Slider lengthSlider; 
    std::unique_ptr<SliderAttachment> lengthSliderAttachment; 
    juce::Slider thresholdSlider; 
    std::unique_ptr<SliderAttachment> thresholdSliderAttachment; 

    juce::Label  thresholdLabel, lengthLabel, infoLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CallerCallerProcessorEditor)
};
