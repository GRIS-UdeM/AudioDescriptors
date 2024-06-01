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
#include "PluginProcessor.h"
#include "Parameters/Dome/View/DomeView.h"
#include "Parameters/Cube/View/CubeView.h"

//==============================================================================
/**
*/
class AudioDescriptorsAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    AudioDescriptorsAudioProcessorEditor(AudioDescriptorsAudioProcessor&);
    ~AudioDescriptorsAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;


    void visibilityChanged() override;

    void setMode(SpatMode spatMode);


private:
    //==============================================================================
    void drawFrame(AudioDescriptorsAudioProcessor& audioProcessor, juce::Graphics& g);

    //==============================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioDescriptorsAudioProcessor& mAudioProcessor;
    juce::Viewport mViewportDome;
    juce::Viewport mViewportCube;
    //==============================================================================

    /// <Panel de selection>
    juce::Label CGOscIdLabel, modeLabel, portLabel, hostLabel;
    juce::Slider CGOscIdSlider; // Curseur pour sélectionner l'ID OSC de ControlGris
    juce::TextButton modeButton, oscButton;
    juce::TextEditor hostEditor, portEditor;

    juce::FlexBox source;
    juce::FlexBox mode;
    juce::FlexBox host;
    juce::FlexBox port;

    bool isFirstTimeVisible = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioDescriptorsAudioProcessorEditor)
};
