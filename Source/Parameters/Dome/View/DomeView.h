/*
 This file is part of AudioDescriptors.

 Developers: Hicheme BEN GAIED, Ga�l LANE L�PINE

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


class DomeView : public juce::Component
{
public:
    DomeView(AudioDescriptorsAudioProcessor& processor);
    ~DomeView() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:

    //==============================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioDescriptorsAudioProcessor& mAudioProcessor;

    //==============================================================================

    PanelView mAzimuthPanel;
    PanelView mElevationPanel;
    PanelView mHSpanPanel;
    PanelView mVSpanPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DomeView)
};
