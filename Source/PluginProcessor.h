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

#include "FluidVersion.hpp"
#include "Descriptors/PitchD.hpp"
#include "Descriptors/LoudnessD.hpp"
#include "Descriptors/StatsD.hpp"
#include "Descriptors/ShapeD.hpp"
#include "Descriptors/CentroidD.hpp"
#include "Descriptors/SpreadD.hpp"
#include "Descriptors/FlatnessD.hpp"
#include "Descriptors/StftD.hpp"
#include "Descriptors/BandsD.hpp"
#include "Descriptors/DctD.hpp"
#include "Descriptors/OnsetDetectionD.hpp"
#include "Descriptors/OperationsD.hpp"

#include "ParameterFunctions.h"
#include "Parameters/Dome/src/DomeSettings.hpp"
#include "Parameters/Cube/src/CubeSettings.hpp"

#include "Constants.h"

//==============================================================================
/*Clean version code reference
*/
class AudioDescriptorsAudioProcessorEditor;

enum class SpatMode{ dome, cube };

class AudioDescriptorsAudioProcessor : public juce::AudioProcessor
    , private juce::Timer
{
public:
    //==============================================================================
    AudioDescriptorsAudioProcessor();
    ~AudioDescriptorsAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    void timerCallback() override;

    //==============================================================================
    [[nodiscard]] bool createOscConnection(juce::String const& address, int oscPort);
    [[nodiscard]] bool disconnectOsc();
    [[nodiscard]] bool isOscConnected() const { return mOscConnected; }
    //[[nodiscard]] bool isOscActive() const { return mOscActivated; }
    void initializeOSC();
    //void setOscActive(bool state);
    void sendOscMessage();

    //==============================================================================
    void resetFunction();
    void initFunction();

    //==============================================================================

    SpatMode getModeState();
    void setModeState(SpatMode spatMode);
    juce::String getSpatModeToString();

    void processDomeParameter(Parameters& parameter, int index, double value, bool isAzimuth, bool isOffset);
    void processCubeParameter(Parameters& parameter, int index, double value, bool isOffset);

    void setOnsetDetectionMetric(ParameterID paramID, const int metric);
    void setOnsetDetectionThreshold(ParameterID paramID, const float tresh);
    void setOnsetDetectionMinTime(ParameterID paramID, const double minTime);
    void setOnsetDetectionMaxTime(ParameterID paramID, const double maxTime);

    DomeSettings& getDomeSettings();
    CubeSettings& getCubeSettings();

    juce::String& getCurrentOscAddress();
    void setCurrentOscAddress(juce::String& address);
    int getCurrentOscPort();
    void setCurrentOscPort(int const port);
    int getControlGrisId();
    void setControlGrisId(int const id);

private:
    //==============================================================================
    double mSampleRate{};
    int mBlockSize{};
    juce::AudioBuffer<float> mDescriptorsBuffer;
    SpatMode mSpatMode{ SpatMode::dome };

    juce::AudioProcessorValueTreeState mAudioProcessorValueTreeState;

    // OSC
    //bool mOscActivated{ true };
    bool mOscConnected{ false };
    bool mOscOutputConnected{ false };
    int mControlGrisId{ 1 };
    juce::String mCurrentOscAddress{ "127.0.0.1" };
    int mLastConnectedOscPort{ -1 };
    int mCurrentOscOutputPort{ 9000 };
    //juce::String mCurrentOscOutputAddress{ "192.168.1.100" };
    juce::OSCSender mOscSender;

    double mAzimuthDomeValue{};
    double mElevationDomeValue{};
    double mHspanDomeValue{};
    double mVspanDomeValue{};

    double mXCubeValue{};
    double mYCubeValue{};
    double mZCubeValue{};
    double mHspanCubeValue{};
    double mVspanCubeValue{};

    OperationsD ops;
    DctD mDct;
    BandsD mBands;

    StftD mStftPitch;
    StftD mStftSpectral;

    StatsD mStats;
    ShapeD mShape;

    PitchD mPitch;
    LoudnessD mLoudness;
    CentroidD mCentroid;
    SpreadD mSpread;
    FlatnessD mFlatness;
    OnsetDetectionD mOnsetDetectionAzimuth;
    OnsetDetectionD mOnsetDetectionElevation;
    OnsetDetectionD mOnsetDetectionHSpan;
    OnsetDetectionD mOnsetDetectionVSpan;
    OnsetDetectionD mOnsetDetectionX;
    OnsetDetectionD mOnsetDetectionY;
    OnsetDetectionD mOnsetDetectionZ;

    ParameterFunctions mParamFunctions;

    DomeSettings domeSettings;
    CubeSettings cubeSettings;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioDescriptorsAudioProcessor)
};
