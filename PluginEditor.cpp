#include "PluginEditor.h"
#include "PluginProcessor.h"

TelephoneAudioProcessorEditor::TelephoneAudioProcessorEditor(TelephoneAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Button setup
    toggleButton.setClickingTogglesState(false);
    toggleButton.onClick = [this]() { toggleEffect(); };
    addAndMakeVisible(toggleButton);

    // LED setup
    addAndMakeVisible(ledIndicator);
    ledIndicator.setLEDState(true); // Initially ON

    setSize(200, 150);
}

TelephoneAudioProcessorEditor::~TelephoneAudioProcessorEditor() {}

void TelephoneAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("Telephone", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void TelephoneAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    auto textArea = bounds.removeFromTop(40);

    ledIndicator.setBounds(bounds.removeFromTop(30).withSizeKeepingCentre(20, 20));

    toggleButton.setBounds(bounds.removeFromBottom(40).reduced(10));
}

void TelephoneAudioProcessorEditor::toggleEffect()
{
    isEffectActive = !isEffectActive;
    processor.setEffectActive(isEffectActive);
    ledIndicator.setLEDState(isEffectActive);
}
