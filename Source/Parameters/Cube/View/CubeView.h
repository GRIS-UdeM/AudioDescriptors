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
#include "../../PanelView.h"


class CubeView : public juce::Component
{
public:
    CubeView(AudioDescriptorsAudioProcessor& processor);
    ~CubeView() override;

    void paint(juce::Graphics&) override;
    void resized() override;

protected:
    /////////////////X/////////////////

    int indexFactorLoudness = 0;
    int indexFactorSpread = 1;
    int indexFactorNoise = 2;

    int indexSmoothAndRangeLoudness = 0;
    int indexSmoothAndRangePitch = 1;
    int indexSmoothAndRangeCentroid = 2;
    int indexSmoothAndRangeSpread = 3;
    int indexSmoothAndRangeNoise = 4;

    int indexMinFreqPitch = 0;
    int indexMinFreqCentroid = 1;

    int indexMaxFreqPitch = 0;
    int indexMaxFreqCentroid = 1;

private:

    //==============================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioDescriptorsAudioProcessor& mAudioProcessor;

    //==============================================================================

    PanelView mXPanel;
    PanelView mYPanel;
    PanelView mZPanel;
    PanelView mHSpanPanel;
    PanelView mVSpanPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CubeView)
};
