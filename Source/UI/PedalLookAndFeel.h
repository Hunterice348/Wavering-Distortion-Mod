#pragma once
#include <JuceHeader.h>

class PedalLookAndFeel : public juce::LookAndFeel_V4
{
public:
    PedalLookAndFeel()
    {
        setColour(juce::Label::textColourId, juce::Colours::white);
        setColour(juce::Slider::thumbColourId, juce::Colour(0xff00bfff)); // Electric Blue
        setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
        setColour(juce::TextButton::buttonColourId, juce::Colour(0xff4b0082)); // Purple
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10.0f);
        auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = 4.0f;
        
        // Background Arc
        {
            juce::Path backgroundArc;
            backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
            g.setColour(juce::Colours::darkgrey);
            g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
        
        // Active Arc
        {
            juce::Path activeArc;
            activeArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), radius, radius, 0.0f, rotaryStartAngle, toAngle, true);
            g.setColour(juce::Colours::cyan);
            g.strokePath(activeArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
        
        // Knob Body
        {
            g.setColour(juce::Colours::black);
            g.fillEllipse(bounds.reduced(radius * 0.3f));
            
            // Indicator
            juce::Point<float> indicatorPoint(bounds.getCentreX() + std::cos(toAngle) * (radius * 0.8f),
                                              bounds.getCentreY() + std::sin(toAngle) * (radius * 0.8f));
            g.setColour(juce::Colours::white);
            g.fillEllipse(indicatorPoint.x - 3, indicatorPoint.y - 3, 6, 6);
        }
    }
};
