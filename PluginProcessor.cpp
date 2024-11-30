#include "PluginProcessor.h"
#include "PluginEditor.h"

TelephoneAudioProcessor::TelephoneAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

TelephoneAudioProcessor::~TelephoneAudioProcessor() {}

void TelephoneAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    filters.prepare(spec);

    auto highPassCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 400.0f, 0.7071f);
    auto lowPassCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 2500.0f, 0.7071f);

    if (highPassCoefficients != nullptr && lowPassCoefficients != nullptr)
    {
        *filters.get<0>().coefficients = *highPassCoefficients;
        *filters.get<1>().coefficients = *lowPassCoefficients;
    }

    filters.reset(); // Reset filter states to prevent noise
}

void TelephoneAudioProcessor::releaseResources() {}

bool TelephoneAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void TelephoneAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // If the effect is active, apply the telephone EQ filters
    if (effectActive)
    {
        juce::dsp::AudioBlock<float> block(buffer);

        for (size_t channel = 0; channel < block.getNumChannels(); ++channel)
        {
            juce::dsp::AudioBlock<float> singleChannelBlock = block.getSingleChannelBlock(channel);
            juce::dsp::ProcessContextReplacing<float> context(singleChannelBlock);
            filters.process(context);
        }
    }
    else
    {
    }
}


juce::AudioProcessorEditor* TelephoneAudioProcessor::createEditor()
{
    return new TelephoneAudioProcessorEditor(*this);
}

bool TelephoneAudioProcessor::hasEditor() const { return true; }

const juce::String TelephoneAudioProcessor::getName() const { return "telephone"; }

bool TelephoneAudioProcessor::acceptsMidi() const { return false; }
bool TelephoneAudioProcessor::producesMidi() const { return false; }
bool TelephoneAudioProcessor::isMidiEffect() const { return false; }
double TelephoneAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int TelephoneAudioProcessor::getNumPrograms() { return 1; }
int TelephoneAudioProcessor::getCurrentProgram() { return 0; }
void TelephoneAudioProcessor::setCurrentProgram(int) {}
const juce::String TelephoneAudioProcessor::getProgramName(int) { return {}; }
void TelephoneAudioProcessor::changeProgramName(int, const juce::String&) {}

void TelephoneAudioProcessor::getStateInformation(juce::MemoryBlock&) {}
void TelephoneAudioProcessor::setStateInformation(const void*, int) {}

void TelephoneAudioProcessor::setEffectActive(bool isActive)
{
    effectActive = isActive;
}

bool TelephoneAudioProcessor::isEffectActive() const
{
    return effectActive;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TelephoneAudioProcessor();
}
