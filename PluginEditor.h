#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class TelephoneAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    TelephoneAudioProcessorEditor(TelephoneAudioProcessor&);
    ~TelephoneAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    TelephoneAudioProcessor& processor;

    juce::TextButton toggleButton{"Toggle Effect"};
    bool isEffectActive = true;

    class LEDComponent : public juce::Component
    {
    public:
        void setLEDState(bool isActive)
        {
            active = isActive;
            repaint();
        }

        void paint(juce::Graphics& g) override
        {
            g.setColour(active ? juce::Colours::red : juce::Colours::darkred);
            g.fillEllipse(getLocalBounds().toFloat());
        }

    private:
        bool active = false;
    };

    LEDComponent ledIndicator;

    void toggleEffect();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TelephoneAudioProcessorEditor)
};
