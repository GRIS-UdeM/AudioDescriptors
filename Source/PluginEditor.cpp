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

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AudioDescriptorsAudioProcessorEditor::AudioDescriptorsAudioProcessorEditor(AudioDescriptorsAudioProcessor& p)
    : AudioProcessorEditor(&p), mAudioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 800);
    setResizable(true, true);

    addAndMakeVisible(mViewportCube);
    addAndMakeVisible(mViewportDome);

    mViewportDome.setViewedComponent(new DomeView(p));
    mViewportCube.setViewedComponent(new CubeView(p));

    setMode(mAudioProcessor.getModeState());

    ////////////////////////////Panel de selection
    addAndMakeVisible(CGOscIdLabel);
    CGOscIdLabel.setText("CG osc Id", juce::dontSendNotification);
    CGOscIdLabel.attachToComponent(&CGOscIdSlider, true);

    // Configuration de CGOscIdSlider
    addAndMakeVisible(CGOscIdSlider);
    CGOscIdSlider.setRange(1, 128, 1); // valeurs min, max, et intervalle
    CGOscIdSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    CGOscIdSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 20);
    CGOscIdSlider.onValueChange = [this] {
        mAudioProcessor.setControlGrisId(static_cast<int>(CGOscIdSlider.getValue()));
        };

    // Configuration de modeButton
    addAndMakeVisible(modeLabel);
    modeLabel.setText("Mode", juce::dontSendNotification);
    modeLabel.attachToComponent(&modeButton, true);
    addAndMakeVisible(modeButton);
    modeButton.setButtonText(mAudioProcessor.getSpatModeToString());
    modeButton.onClick = [this] {
        auto currentSpatMode = mAudioProcessor.getModeState();
        if (currentSpatMode == SpatMode::dome) {
            mAudioProcessor.setModeState(SpatMode::cube);
            setMode(SpatMode::cube);
        }
        else {
            mAudioProcessor.setModeState(SpatMode::dome);
            setMode(SpatMode::dome);
        }
        };

    // Configuration de oscButton
    addAndMakeVisible(oscButton);
    oscButton.setButtonText("OSC");
    oscButton.onClick = [this] {
        [[maybe_unused]] auto success{ mAudioProcessor.disconnectOsc() };
        mAudioProcessor.initializeOSC();
        if (mAudioProcessor.isOscConnected()) {
            hostEditor.applyColourToAllText(juce::Colours::lightgreen);
            portEditor.applyColourToAllText(juce::Colours::lightgreen);
            repaint();
            return;
        }
        hostEditor.applyColourToAllText(juce::Colours::red);
        portEditor.applyColourToAllText(juce::Colours::red);
        repaint();
        };

    // Configuration de hostLabel et hostEditor
    addAndMakeVisible(hostLabel);
    hostLabel.setText("Host", juce::dontSendNotification);
    hostLabel.attachToComponent(&hostEditor, true);

    addAndMakeVisible(hostEditor);
    hostEditor.setText(mAudioProcessor.getCurrentOscAddress());
    hostEditor.setInputRestrictions(15, "0123456789.");
    hostEditor.onReturnKey = [this] {
        auto addr{ hostEditor.getText() };
        mAudioProcessor.setCurrentOscAddress(addr);
        };
    hostEditor.onFocusLost = hostEditor.onReturnKey;

    // Configuration de portLabel et portEditor
    addAndMakeVisible(portLabel);
    portLabel.setText("Port", juce::dontSendNotification);
    portLabel.attachToComponent(&portEditor, true);

    addAndMakeVisible(portEditor);
    portEditor.setText(juce::String(mAudioProcessor.getCurrentOscPort()));
    portEditor.setInputRestrictions(6, "0123456789");
    portEditor.onReturnKey = [this] {
        auto port{ portEditor.getText().getIntValue() };
        mAudioProcessor.setCurrentOscPort(port);
        };
    portEditor.onFocusLost = portEditor.onReturnKey;
}

AudioDescriptorsAudioProcessorEditor::~AudioDescriptorsAudioProcessorEditor()
{
    //mAudioProcessor.resetEditor();
}

//==============================================================================
void AudioDescriptorsAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::white);
    //drawFrame(mAudioProcessor, g);
}


void AudioDescriptorsAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    auto firstThird = area.removeFromTop(area.getHeight() / 40);
    //auto secondThird = area.removeFromTop(area.getHeight() / 2);
    auto thirdThird = area;

    const juce::FlexItem::Margin sliderMargin(0, 200, 0, 0);
    juce::FlexItem::Margin margin(0, 10, 0, 0);
    juce::FlexBox generalFlexBoxConfig;
    generalFlexBoxConfig.flexDirection = juce::FlexBox::Direction::row;
    generalFlexBoxConfig.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    // Panel Content Configuration
    source = generalFlexBoxConfig;
    source.items.add(juce::FlexItem(modeLabel).withMaxWidth(50).withFlex(1));
    source.items.add(juce::FlexItem(modeButton).withFlex(3));
    source.items.add(juce::FlexItem(CGOscIdLabel).withMinWidth(50).withFlex(1));
    source.items.add(juce::FlexItem(CGOscIdSlider).withFlex(3));
    source.items.add(juce::FlexItem(hostLabel).withMaxWidth(50).withFlex(1));
    source.items.add(juce::FlexItem(hostEditor).withFlex(3));
    source.items.add(juce::FlexItem(portLabel).withMaxWidth(50).withFlex(1));
    source.items.add(juce::FlexItem(portEditor).withMaxWidth(200).withFlex(3));
    source.items.add(juce::FlexItem(oscButton).withMaxWidth(50).withFlex(1));

    juce::FlexBox panelFlexBox;
    panelFlexBox.flexDirection = juce::FlexBox::Direction::column;
    panelFlexBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    panelFlexBox.alignItems = juce::FlexBox::AlignItems::stretch;
    panelFlexBox.items.add(juce::FlexItem(source).withHeight(30).withMargin(juce::FlexItem::Margin(2)));


    juce::FlexBox parentFlexBoxTwo;
    parentFlexBoxTwo.flexDirection = juce::FlexBox::Direction::row;
    parentFlexBoxTwo.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parentFlexBoxTwo.alignItems = juce::FlexBox::AlignItems::stretch;
    parentFlexBoxTwo.items.add(juce::FlexItem(mViewportCube).withFlex(1));
    parentFlexBoxTwo.performLayout(thirdThird);

    juce::FlexBox parentFlexBoxOne;
    parentFlexBoxOne.flexDirection = juce::FlexBox::Direction::row;
    parentFlexBoxOne.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parentFlexBoxOne.alignItems = juce::FlexBox::AlignItems::stretch;
    parentFlexBoxOne.items.add(juce::FlexItem(mViewportDome).withFlex(1));
    parentFlexBoxOne.performLayout(thirdThird);

    juce::FlexBox parentFlexBoxThree;
    parentFlexBoxThree.flexDirection = juce::FlexBox::Direction::row;
    parentFlexBoxThree.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parentFlexBoxThree.alignItems = juce::FlexBox::AlignItems::stretch;
    parentFlexBoxThree.items.add(juce::FlexItem(panelFlexBox).withFlex(1));
    parentFlexBoxThree.performLayout(firstThird);
}

void AudioDescriptorsAudioProcessorEditor::drawFrame(AudioDescriptorsAudioProcessor& /*audioProcessor*/, juce::Graphics& /*g*/)
{

}



void AudioDescriptorsAudioProcessorEditor::visibilityChanged()
{
    if (isFirstTimeVisible && isVisible())
    {
        mAudioProcessor.setInterfaceState();
        isFirstTimeVisible = false;
        //DBG("L'éditeur est visible pour la première fois.");
    }
}

void AudioDescriptorsAudioProcessorEditor::setMode(SpatMode spatMode)
{
    if (spatMode == SpatMode::dome)
    {
        modeButton.setButtonText("Dome");
        mViewportDome.setVisible(true);
        mViewportCube.setVisible(false);
        mAudioProcessor.setModeState(SpatMode::dome);
    }
    else
    {
        modeButton.setButtonText("Cube");
        mViewportDome.setVisible(false);
        mViewportCube.setVisible(true);
        mAudioProcessor.setModeState(SpatMode::cube);
    }
}
