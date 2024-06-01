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

#include "CubeView.h"
#include "../Source/PluginProcessor.h"

CubeView::CubeView(AudioDescriptorsAudioProcessor& processor)
    : mAudioProcessor(processor)
    , mXPanel(processor, processor.getCubeSettings().getXCube())
    , mYPanel(processor, processor.getCubeSettings().getYCube())
    , mZPanel(processor, processor.getCubeSettings().getZCube())
    , mHSpanPanel(processor, processor.getCubeSettings().getHSpanCube())
    , mVSpanPanel(processor, processor.getCubeSettings().getVSpanCube())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1000, 1000);
    //setResizable(true, true);

    addAndMakeVisible(mXPanel);
    mXPanel.setVisible(true);

    addAndMakeVisible(mYPanel);
    mYPanel.setVisible(true);

    addAndMakeVisible(mZPanel);
    mZPanel.setVisible(true);

    addAndMakeVisible(mHSpanPanel);
    mHSpanPanel.setVisible(true);

    addAndMakeVisible(mVSpanPanel);
    mVSpanPanel.setVisible(true);
}

CubeView::~CubeView()
{
    // Nettoyage si nécessaire
}

void CubeView::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::white);
}

void CubeView::resized()
{
    auto area = getLocalBounds().reduced(10);

    auto firstThird = area.removeFromTop(area.getHeight() / 3);
    auto secondThird = area.removeFromTop(area.getHeight() / 2);
    auto thirdThird = area;


    const juce::FlexItem::Margin sliderMargin(0, 200, 0, 0);
    juce::FlexItem::Margin margin(0, 10, 0, 0);
    juce::FlexBox generalFlexBoxConfig;
    generalFlexBoxConfig.flexDirection = juce::FlexBox::Direction::row;
    generalFlexBoxConfig.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    juce::FlexBox zFlexBoxNext;
    //////////
    juce::FlexBox parentFlexBoxOne;
    parentFlexBoxOne.flexDirection = juce::FlexBox::Direction::row; // Disposition horizontale
    parentFlexBoxOne.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parentFlexBoxOne.alignItems = juce::FlexBox::AlignItems::stretch;
    parentFlexBoxOne.items.add(juce::FlexItem(mXPanel).withFlex(1));
    parentFlexBoxOne.items.add(juce::FlexItem(mYPanel).withFlex(1));
    parentFlexBoxOne.performLayout(firstThird); // Utilise topHalf pour le layout

    // Configuration de la FlexBox parente pour le bas (centroid)
    juce::FlexBox parentFlexBoxTwo;
    parentFlexBoxTwo.flexDirection = juce::FlexBox::Direction::row; // Disposition horizontale
    parentFlexBoxTwo.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parentFlexBoxTwo.alignItems = juce::FlexBox::AlignItems::stretch;
    parentFlexBoxTwo.items.add(juce::FlexItem(mHSpanPanel).withFlex(1));
    parentFlexBoxTwo.items.add(juce::FlexItem(mVSpanPanel).withFlex(1));
    parentFlexBoxTwo.performLayout(secondThird); // Utilise bottomHalf pour le layout

    juce::FlexBox parentFlexBoxThree;
    parentFlexBoxThree.flexDirection = juce::FlexBox::Direction::row; // Disposition horizontale
    parentFlexBoxThree.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parentFlexBoxThree.alignItems = juce::FlexBox::AlignItems::stretch;
    parentFlexBoxThree.items.add(juce::FlexItem(mZPanel).withFlex(1));
    parentFlexBoxThree.items.add(juce::FlexItem(zFlexBoxNext).withFlex(1));
    parentFlexBoxThree.performLayout(thirdThird); // Utilise bottomHalf pour le layout
}
