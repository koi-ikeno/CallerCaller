/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
CallerCallerProcessor::CallerCallerProcessor()
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
    , parameters(*this, nullptr, juce::Identifier("tutorial"), 
        {
            std::make_unique<juce::AudioParameterFloat>(
                "val_length",  
                "Length",  
                juce::NormalisableRange<float>(30.0f, 3000.0f,1.0f), 
                500.0f), 

            std::make_unique<juce::AudioParameterFloat>(
                "val_threshold",  
                "Threshold",  
                juce::NormalisableRange<float>(-100.0f, 0.0f, 0.05f), 
                0.0f),        
        })

{
    val_threshold = parameters.getRawParameterValue("val_threshold");
    val_length = parameters.getRawParameterValue("val_length");

    lengthMs = 1000;
    threshold = 1.0;
    isDuringNoteOn = false;
    countdown_notelength = 0;

}

CallerCallerProcessor::~CallerCallerProcessor()
{
}

//==============================================================================
const juce::String CallerCallerProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CallerCallerProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CallerCallerProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CallerCallerProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CallerCallerProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CallerCallerProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CallerCallerProcessor::getCurrentProgram()
{
    return 0;
}

void CallerCallerProcessor::setCurrentProgram (int index)
{
}

const juce::String CallerCallerProcessor::getProgramName (int index)
{
    return {};
}

void CallerCallerProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CallerCallerProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    spec.maximumBlockSize = samplesPerBlock; 
    spec.numChannels = 2; 
    spec.sampleRate = sampleRate; 

}

void CallerCallerProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CallerCallerProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CallerCallerProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    lengthMs  = (unsigned long) *val_length;
    threshold = *val_threshold;

    // decibel minus to 0db  :  linear gain -1.0 to 1.0 
    auto threshGain = juce::Decibels::decibelsToGain(threshold);

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
     
    juce::dsp::AudioBlock<float> audioBlock(buffer); 
    juce::dsp::ProcessContextReplacing<float> context(audioBlock); 

    auto&& inBlock = context.getInputBlock();
    auto len = inBlock.getNumSamples();
    auto numChannels = inBlock.getNumChannels();

    if (true)
    {
        JUCE_BEGIN_IGNORE_WARNINGS_MSVC(6255 6386)
        
        JUCE_END_IGNORE_WARNINGS_MSVC
            
            for (size_t i = 0; i < len; ++i) {
                for (size_t chan = 0; chan < numChannels; ++chan) {
                    auto* src = inBlock.getChannelPointer(chan);
                        
                    if ( ( src[i] > (threshGain ) ) && !isDuringNoteOn ) {
                    
                        //one event at a time, so offset is 0 for now.
                        midi.addEvent(juce::MidiMessage::noteOn(1, 48, (juce::uint8)127), 0);
                        isDuringNoteOn = true;
                        countdown_notelength = 
                            (unsigned long)( spec.sampleRate / 1000.0 * (float)lengthMs);
                    }      
                }

                if (isDuringNoteOn) {
                    countdown_notelength = countdown_notelength - 1;
                    if (countdown_notelength == 0) {
                        midi.addEvent(juce::MidiMessage::noteOff(1, 48), 0);
                        isDuringNoteOn = false;
                    }
                }
            }
    }
}

//==============================================================================
bool CallerCallerProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CallerCallerProcessor::createEditor()
{
    return new CallerCallerProcessorEditor (*this, parameters);
}

//==============================================================================
void CallerCallerProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = parameters.copyState(); 
    std::unique_ptr<juce::XmlElement> xml(state.createXml()); 
    copyXmlToBinary(*xml, destData); 

}

void CallerCallerProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes)); 

    if (xmlState.get() != nullptr) { 
        if (xmlState->hasTagName(parameters.state.getType())) { 
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState)); 
        }
    }

}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CallerCallerProcessor();
}

