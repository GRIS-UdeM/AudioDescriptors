/*
 This file is part of AudioDescriptors.

 Developers: Hicheme BEN GAIED, Gaël LANE LÉPINE

 AudioDescriptors is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 AudioDescriptors is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 See LICENCE file in the project root for full license information on the
 GNU General Public License. For more information, see
 <https://www.gnu.org/licenses/>.
*/

//==============================================================================

#pragma once

#include <JuceHeader.h>
#include "../Source/PluginProcessor.h"

#include "Parameters/Dome/src/AzimuthDome.hpp"

class PanelView : public juce::Component, public juce::ComboBox::Listener
{
public:
    PanelView(AudioDescriptorsAudioProcessor& processor, Parameters& parameter);
    ~PanelView() override;
    void comboBoxChanged(juce::ComboBox* comboBox) override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AudioDescriptorsAudioProcessor& mAudioProcessor;
    Parameters& mParameter;

    juce::ComboBox mDescriptorMetricComboBox;
    juce::ComboBox mDescriptorComboBox;
    juce::Slider mDescriptorFactorSlider;
    juce::Slider mDescriptorSmoothSlider;
    juce::Slider mDescriptorSmoothCoefSlider;
    juce::Slider mDescriptorRangeSlider;
    juce::Slider mDescriptorMinFreqSlider;
    juce::Slider mDescriptorMaxFreqSlider;
    juce::Slider mDescriptorThresholdSlider;
    juce::Slider mDescriptorMinTimeSlider;
    juce::Slider mDescriptorMaxTimeSlider;
    juce::Slider mDescriptorLapSlider;
    juce::Slider mDescriptorOffsetSlider;
    juce::Label mDescriptorLabel;
    juce::Label mDescriptorOffsetLabel;
    juce::Label mDescriptorFactorLabel;
    juce::Label mDescriptorSmoothLabel;
    juce::Label mDescriptorMoreSmoothLabel;
    juce::Label mDescriptorRangeLabel;
    juce::Label mDescriptorMinFreqLabel;
    juce::Label mDescriptorMaxFreqLabel;
    juce::Label mDescriptorThresholdLabel;
    juce::Label mDescriptorMinTimeLabel;
    juce::Label mDescriptorMaxTimeLabel;
    juce::Label mDescriptorLapLabel;

    juce::FlexBox mDescriptorFactorBox;
    juce::FlexBox mDescriptorSmoothBox;
    juce::FlexBox mDescriptorMoreSmoothBox;
    juce::FlexBox mDescriptorRangeBox;
    juce::FlexBox mDescriptorMinFreqBox;
    juce::FlexBox mDescriptorMaxFreqBox;
    juce::FlexBox mDescriptorThresholdBox;
    juce::FlexBox mDescriptorMinTimeBox;
    juce::FlexBox mDescriptorMaxTimeBox;
    juce::FlexBox mDescriptorMetricBox;
    juce::FlexBox mDescriptorLapBox;
    juce::FlexBox mDescriptorOffsetBox;

    juce::StringArray boxItems{ "Select parameters", "Loudness", "Pitch", "Centroid", "Spread", "Noise", "Iterations Speed" };
    juce::StringArray metricBoxItems{ "Energy", "High Frequency Content", "Spectral Flux", "Rectified Complex Domain" };

    bool isAzimuth = false;
    bool isOffset = false;
};
