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
#include "vector"

using namespace fluid;
using namespace algorithm;

//==============================================================================
AudioDescriptorsAudioProcessor::AudioDescriptorsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
	, mAudioProcessorValueTreeState(*this, nullptr, juce::Identifier(JucePlugin_Name), {})
	, mAzimuthDome(mAudioProcessorValueTreeState, mParamFunctions)
	, mElevationDome(mAudioProcessorValueTreeState, mParamFunctions)
	, mHSpanDome(mAudioProcessorValueTreeState, mParamFunctions)
	, mVSpanDome(mAudioProcessorValueTreeState, mParamFunctions)
	, mXCube(mAudioProcessorValueTreeState, mParamFunctions)
	, mYCube(mAudioProcessorValueTreeState, mParamFunctions)
	, mZCube(mAudioProcessorValueTreeState, mParamFunctions)
	, mHSpanCube(mAudioProcessorValueTreeState, mParamFunctions)
	, mVSpanCube(mAudioProcessorValueTreeState, mParamFunctions)
	, mSpatParametersDomeRefs{ &mAzimuthDome, &mElevationDome, &mHSpanDome, &mVSpanDome }
	, mSpatParametersCubeRefs{ &mXCube, &mYCube, &mZCube, &mHSpanCube, &mVSpanCube }
	, mSpatParametersDomeValueRefs{ &mAzimuthDomeValue, &mElevationDomeValue, &mHspanDomeValue, &mVspanDomeValue }
	, mSpatParametersCubeValueRefs{ &mXCubeValue, &mYCubeValue, &mZCubeValue, &mHspanCubeValue, &mVspanCubeValue }
	, mDomeOnsetDetectionRefs{ &mOnsetDetectionAzimuth , &mOnsetDetectionElevation, &mOnsetDetectionHSpan, &mOnsetDetectionVSpan }
	, mCubeOnsetDetectionRefs{ &mOnsetDetectionX , &mOnsetDetectionY, &mOnsetDetectionZ, &mOnsetDetectionHSpan, &mOnsetDetectionVSpan }
{
	//startTimerHz(50);
	DBG("Fluid v" << fluid::client::fluidVersion());
}

AudioDescriptorsAudioProcessor::~AudioDescriptorsAudioProcessor()
{
	[[maybe_unused]] auto const success{ disconnectOsc() };
}

//==============================================================================
const juce::String AudioDescriptorsAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool AudioDescriptorsAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool AudioDescriptorsAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool AudioDescriptorsAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double AudioDescriptorsAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int AudioDescriptorsAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int AudioDescriptorsAudioProcessor::getCurrentProgram()
{
	return 0;
}

void AudioDescriptorsAudioProcessor::setCurrentProgram(int /*index*/)
{
}

const juce::String AudioDescriptorsAudioProcessor::getProgramName(int /*index*/)
{
	return {};
}

void AudioDescriptorsAudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/)
{
}

void AudioDescriptorsAudioProcessor::resetFunction()
{
	mPitch.reset();
	mLoudness.reset();
	mStats.reset();
	mShape.reset();
	mCentroid.reset();
	mSpread.reset();
	mFlatness.reset();
	mOnsetDetectionAzimuth.reset();
	mOnsetDetectionElevation.reset();
	mOnsetDetectionHSpan.reset();
	mOnsetDetectionVSpan.reset();
	mOnsetDetectionX.reset();
	mOnsetDetectionY.reset();
	mOnsetDetectionZ.reset();
}

void AudioDescriptorsAudioProcessor::initFunction()
{
	mPitch.init();
	mLoudness.init(mSampleRate);
	mStats.init();
	mCentroid.init();
	mSpread.init();
	mFlatness.init();
	mOnsetDetectionAzimuth.init();
	mOnsetDetectionElevation.init();
	mOnsetDetectionHSpan.init();
	mOnsetDetectionVSpan.init();
	mOnsetDetectionX.init();
	mOnsetDetectionY.init();
	mOnsetDetectionZ.init();
}
//==============================================================================
void AudioDescriptorsAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	mSampleRate = sampleRate;
	mBlockSize = samplesPerBlock;

	resetFunction();
	initFunction();

	initializeOSC();
}

void AudioDescriptorsAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioDescriptorsAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void AudioDescriptorsAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
	if (!isOscConnected()) {
		initializeOSC();
	}

	mAzimuthDomeValue = 0.0;
	mElevationDomeValue = 0.0;
	mHspanDomeValue = 0.0;
	mVspanDomeValue = 0.0;

	mXCubeValue = 0.0;
	mYCubeValue = 0.0;
	mZCubeValue = 0.0;
	mHspanCubeValue = 0.0;
	mVspanCubeValue = 0.0;

	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	mDescriptorsBuffer.clear();
	mDescriptorsBuffer.setSize(1, buffer.getNumSamples());
	if (totalNumInputChannels > 1) {
		mDescriptorsBuffer.addFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
		mDescriptorsBuffer.addFrom(0, 0, buffer, 1, 0, buffer.getNumSamples());
		mDescriptorsBuffer.applyGain(0.945f);
	}
	else {
		mDescriptorsBuffer.addFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
	}

	auto bufferMagnitude = mDescriptorsBuffer.getMagnitude(0, mDescriptorsBuffer.getNumSamples());
	auto* channelData = mDescriptorsBuffer.getReadPointer(0);

	// FLUCOMA
	RealVector inSpectral(mBlockSize);
	RealVector inLoudness(mBlockSize);
	RealVector inPitch(mBlockSize);

	for (auto i = 0; i < mDescriptorsBuffer.getNumSamples(); ++i) {
		inSpectral[i] = channelData[i];
		inLoudness[i] = channelData[i];
		inPitch[i] = channelData[i];
	}

	RealVector paddedLoudness = mLoudness.calculatePaddedLoudness(inLoudness);
	fluid::index nFramesLoudness = mLoudness.calculateFramesLoudness(paddedLoudness);
	RealMatrix loudnessMat(nFramesLoudness, 2);
	std::fill(paddedLoudness.begin(), paddedLoudness.end(), 0);
	paddedLoudness(mLoudness.paddedValueLoudness(inLoudness)) <<= inLoudness;

	RealVector paddedPitch = mPitch.calculatePaddedPitch(inPitch);
    fluid::index nFramesPitch = mPitch.calculateFramesPitch(paddedPitch);
	RealMatrix pitchMat(nFramesPitch, 2);
	std::fill(paddedPitch.begin(), paddedPitch.end(), 0);
	paddedPitch(mPitch.paddedValuePitch(inPitch)) <<= inPitch;

	RealVector paddedSpectral = mShape.calculatePaddedSpectral(inSpectral);
    fluid::index nFramesSpectral = mShape.calculateFramesSpectral(paddedSpectral);
	RealMatrix shapeMat(nFramesSpectral, 7);
	std::fill(paddedSpectral.begin(), paddedSpectral.end(), 0);
	paddedSpectral(mShape.paddedValueSpectral(inSpectral)) <<= inSpectral;
	RealVector  shapeStats;

	if (shouldProcessDomeLoudnessAnalysis() || shouldProcessCubeLoudnessAnalysis()) {
		for (int i = 0; i < nFramesLoudness; i++) {
			RealVector loudnessDesc(2);
			RealVectorView windowLoudness = mLoudness.calculateWindowLoudness(paddedLoudness, i);
			mLoudness.mLoudnessProcess(windowLoudness, loudnessDesc);
			loudnessMat.row(i) <<= loudnessDesc;
		}
		mLoudness.calculate(loudnessMat, *mStats.getStats());
		double loudnessValue = mLoudness.getValue();
		loudnessValue = juce::Decibels::decibelsToGain(loudnessValue);
		if (getModeState() == SpatMode::dome) {
			for (int i{}; i < mSpatParametersDomeRefs.size(); ++i) {
				if (mSpatParametersDomeRefs[i]->shouldProcessLoudnessAnalysis()) {
					mSpatParametersDomeRefs[i]->process(mLoudness.getID(), loudnessValue);
					*mSpatParametersDomeValueRefs[i] = mSpatParametersDomeRefs[i]->getDiffValue();
				}
			}
		}
		else {
			for (int i{}; i < mSpatParametersCubeRefs.size(); ++i) {
				if (mSpatParametersCubeRefs[i]->shouldProcessLoudnessAnalysis()) {
					mSpatParametersCubeRefs[i]->process(mLoudness.getID(), loudnessValue);
					*mSpatParametersCubeValueRefs[i] = mSpatParametersCubeRefs[i]->getDiffValue();
				}
			}
		}
	}

	if (shouldProcessDomePitchAnalysis() || shouldProcessCubePitchAnalysis()) {
		ComplexVector framePitch;
		RealVector magnitudePitch;
		RealVector melsPitch;
		for (int j = 0; j < nFramesPitch; j++) {
			mPitch.setFramePitch(framePitch);
			mPitch.setMagnitudePitch(magnitudePitch);
			RealVector     pitch(2);
			RealVectorView windowPitch = mPitch.calculateWindowPitch(paddedPitch, j);

			mPitch.stftProcess(windowPitch, framePitch);
			mPitch.stftMagntiude(framePitch, magnitudePitch);
			mPitch.mYinProcess(magnitudePitch, pitch, mSampleRate);
			pitchMat.row(j) <<= pitch;
		}
		mPitch.calculate(pitchMat, *mStats.getStats());
		double pitchValue = mPitch.getValue();
		pitchValue = mParamFunctions.frequencyToMidiNoteNumber(pitchValue);
		if (getModeState() == SpatMode::dome) {
			for (int i{}; i < mSpatParametersDomeRefs.size(); ++i) {
				if (mSpatParametersDomeRefs[i]->shouldProcessPitchAnalysis()) {
					mSpatParametersDomeRefs[i]->process(mPitch.getID(), pitchValue);
					*mSpatParametersDomeValueRefs[i] = mSpatParametersDomeRefs[i]->getDiffValue();
				}
			}
		}
		else {
			for (int i{}; i < mSpatParametersCubeRefs.size(); ++i) {
				if (mSpatParametersCubeRefs[i]->shouldProcessPitchAnalysis()) {
					mSpatParametersCubeRefs[i]->process(mPitch.getID(), pitchValue);
					*mSpatParametersCubeValueRefs[i] = mSpatParametersCubeRefs[i]->getDiffValue();
				}
			}
		}
	}

	if ((mSpatMode == SpatMode::dome && shouldProcessDomeSpectralAnalysis()) ||
		(mSpatMode == SpatMode::cube && shouldProcessCubeSpectralAnalysis())) {
		ComplexVector  frameSpectral;
		RealVector     magnitudeSpectral;
		for (int y = 0; y < nFramesSpectral; y++) {
			mShape.setFrameSpectral(frameSpectral);
			mShape.setMagnitudeSpectral(magnitudeSpectral);
			RealVector     shapeDesc(7);
			RealVectorView windowSpectral = mShape.calculateWindowSpectral(paddedSpectral, y);
			mShape.stftProcess(windowSpectral, frameSpectral);
			mShape.stftMagntiude(frameSpectral, magnitudeSpectral);
			mShape.mShapeProcess(magnitudeSpectral, shapeDesc, mSampleRate);
			shapeMat.row(y) <<= shapeDesc;
		}

		shapeStats = mShape.shapeCalculate(shapeMat, *mStats.getStats());

		if (shouldProcessDomeCentroidAnalysis() || shouldProcessCubeCentroidAnalysis()) {
			mCentroid.calculate(shapeStats);
			double centroidValue = mCentroid.getValue(); // centroidValue when silence = 118.02870609942256
			if (bufferMagnitude == 0.0f) {
				centroidValue = 0.0;
			}
			if (getModeState() == SpatMode::dome) {
				for (int i{}; i < mSpatParametersDomeRefs.size(); ++i) {
					if (mSpatParametersDomeRefs[i]->shouldProcessCentroidAnalysis()) {
						mSpatParametersDomeRefs[i]->process(mCentroid.getID(), centroidValue);
						*mSpatParametersDomeValueRefs[i] = mSpatParametersDomeRefs[i]->getDiffValue();
					}
				}
			}
			else {
				for (int i{}; i < mSpatParametersCubeRefs.size(); ++i) {
					if (mSpatParametersCubeRefs[i]->shouldProcessCentroidAnalysis()) {
						mSpatParametersCubeRefs[i]->process(mCentroid.getID(), centroidValue);
						*mSpatParametersCubeValueRefs[i] = mSpatParametersCubeRefs[i]->getDiffValue();
					}
				}
			}
		}

		if (shouldProcessDomeSpreadAnalysis() || shouldProcessCubeSpreadAnalysis()) {
			mSpread.calculate(shapeStats);
			double spreadValue = mSpread.getValue(); // spreadValue when silence  = 16.520351353896057
			if (bufferMagnitude == 0.0f) {
				spreadValue = 0.0;
			}
			spreadValue = mParamFunctions.zmap(spreadValue, 0.0, 16.0);

			if (getModeState() == SpatMode::dome) {
				for (int i{}; i < mSpatParametersDomeRefs.size(); ++i) {
					if (mSpatParametersDomeRefs[i]->shouldProcessSpreadAnalysis()) {
						mSpatParametersDomeRefs[i]->process(mSpread.getID(), spreadValue);
						*mSpatParametersDomeValueRefs[i] = mSpatParametersDomeRefs[i]->getDiffValue();
					}
				}
			}
			else {
				for (int i{}; i < mSpatParametersCubeRefs.size(); ++i) {
					if (mSpatParametersCubeRefs[i]->shouldProcessSpreadAnalysis()) {
						mSpatParametersCubeRefs[i]->process(mSpread.getID(), spreadValue);
						*mSpatParametersCubeValueRefs[i] = mSpatParametersCubeRefs[i]->getDiffValue();
					}
				}
			}
		}

		if (shouldProcessDomeNoiseAnalysis() || shouldProcessCubeNoiseAnalysis()) {
			mFlatness.calculate(shapeStats);
			double flatnessValue = mFlatness.getValue(); // flatnessValue when silence = -6.9624443085150120e-13
			if (bufferMagnitude == 0.0f) {
				flatnessValue = -160.0;
			}
			flatnessValue = juce::Decibels::decibelsToGain(flatnessValue);
			flatnessValue = mParamFunctions.zmap(flatnessValue, 0.0, 0.5);
			flatnessValue = mParamFunctions.power(flatnessValue);

			if (getModeState() == SpatMode::dome) {
				for (int i{}; i < mSpatParametersDomeRefs.size(); ++i) {
					if (mSpatParametersDomeRefs[i]->shouldProcessNoiseAnalysis()) {
						mSpatParametersDomeRefs[i]->process(mFlatness.getID(), flatnessValue);
						*mSpatParametersDomeValueRefs[i] = mSpatParametersDomeRefs[i]->getDiffValue();
					}
				}
			}
			else {
				for (int i{}; i < mSpatParametersCubeRefs.size(); ++i) {
					if (mSpatParametersCubeRefs[i]->shouldProcessNoiseAnalysis()) {
						mSpatParametersCubeRefs[i]->process(mFlatness.getID(), flatnessValue);
						*mSpatParametersCubeValueRefs[i] = mSpatParametersCubeRefs[i]->getDiffValue();
					}
				}
			}
		}
	}

	if (shouldProcessDomeOnsetDetectionAnalysis() || shouldProcessCubeOnsetDetectionAnalysis()) {
		if (getModeState() == SpatMode::dome) {
			for (int i{}; i < mSpatParametersDomeRefs.size(); ++i) {
				if (mSpatParametersDomeRefs[i]->shouldProcessOnsetDetectionAnalysis()) {
					mDomeOnsetDetectionRefs[i]->process(mDescriptorsBuffer, mSampleRate, mBlockSize);
					mSpatParametersDomeRefs[i]->process(mDomeOnsetDetectionRefs[i]->getID(), mDomeOnsetDetectionRefs[i]->getValue());
					*mSpatParametersDomeValueRefs[i] = mSpatParametersDomeRefs[i]->getDiffValue();
				}
			}
		}
		else {
			for (int i{}; i < mSpatParametersCubeRefs.size(); ++i) {
				if (mSpatParametersCubeRefs[i]->shouldProcessOnsetDetectionAnalysis()) {
					mCubeOnsetDetectionRefs[i]->process(mDescriptorsBuffer, mSampleRate, mBlockSize);
					mSpatParametersCubeRefs[i]->process(mCubeOnsetDetectionRefs[i]->getID(), mCubeOnsetDetectionRefs[i]->getValue());
					*mSpatParametersCubeValueRefs[i] = mSpatParametersCubeRefs[i]->getDiffValue();
				}
			}
		}
	}

	sendOscMessage();
}
//==============================================================================
bool AudioDescriptorsAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioDescriptorsAudioProcessor::createEditor()
{
	return new AudioDescriptorsAudioProcessorEditor (*this);
}

//==============================================================================
void AudioDescriptorsAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	// global properties
	juce::String spatMode{};

	if (mSpatMode == SpatMode::dome) {
		spatMode = juce::String("Dome");
	}
	else {
		spatMode = juce::String("Cube");
	}
	mAudioProcessorValueTreeState.state.setProperty({ juce::String("SpatMode") }, spatMode, nullptr);
	mAudioProcessorValueTreeState.state.setProperty({ juce::String("CG_Id") }, mControlGrisId, nullptr);
	mAudioProcessorValueTreeState.state.setProperty({ juce::String("OSCAddress") }, mCurrentOscAddress, nullptr);
	mAudioProcessorValueTreeState.state.setProperty({ juce::String("OSCPort") }, mCurrentOscOutputPort, nullptr);

	auto const state{ mAudioProcessorValueTreeState.copyState() };
	auto xmlState{ state.createXml() };

	if (xmlState != nullptr) {
		copyXmlToBinary(*xmlState, destData);
	}
}

void AudioDescriptorsAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	auto xmlState = getXmlFromBinary(data, sizeInBytes);

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(mAudioProcessorValueTreeState.state.getType())) {
			mAudioProcessorValueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
			for (const auto& spatParam : mSpatParametersDomeRefs) {
				spatParam->updateParameterState();
			}
			for (const auto& spatParam : mSpatParametersCubeRefs) {
				spatParam->updateParameterState();
			}

			// global properties
			juce::String spatMode{};
			spatMode = mAudioProcessorValueTreeState.state.getProperty({ juce::String("SpatMode") }).toString();
			if (spatMode.compare(juce::String("Cube")) == 0) {
				mSpatMode = SpatMode::cube;
			}
			else {
				mSpatMode = SpatMode::dome;
			}
			mControlGrisId = mAudioProcessorValueTreeState.state.getProperty({ juce::String("CG_Id") }).toString().getIntValue();
			mCurrentOscAddress = mAudioProcessorValueTreeState.state.getProperty({ juce::String("OSCAddress") }).toString();
			mCurrentOscOutputPort = mAudioProcessorValueTreeState.state.getProperty({ juce::String("OSCPort") }).toString().getIntValue();
		}
}

bool AudioDescriptorsAudioProcessor::createOscConnection(juce::String const& address, int oscPort)
{
	if (!disconnectOsc()) {
		return false;
	}
	
	mOscConnected = mOscSender.connect(address, oscPort);
	if (!mOscConnected) {
		DBG("OSC can't establish connection");
		return false;
	}

	mLastConnectedOscPort = oscPort;
	DBG("OSC connected at " << address << ", port " << oscPort);

	return true;
}

bool AudioDescriptorsAudioProcessor::disconnectOsc()
{
	if (mOscConnected) {
		if (mOscSender.disconnect()) {
			mOscConnected = false;
			mLastConnectedOscPort = -1;
		}
	}
	return !mOscConnected;
}

//void AudioDescriptorsAudioProcessor::setOscActive(bool state)
//{
//	mOscActivated = state;
//
//	if (state) {
//		if (mLastConnectedOscPort != mCurrentOscPort || !mOscConnected) {
//			mOscConnected = createOscConnection(mCurrentOscAddress, mCurrentOscPort);
//		}
//	}
//	else {
//		mOscConnected = !disconnectOsc();
//	}
//	mAudioProcessorValueTreeState.state.setProperty("oscConnected", isOscActive(), nullptr);
//}

void AudioDescriptorsAudioProcessor::initializeOSC()
{
	if (!mOscConnected /*&& mOscActivated*/) {
		// Connect OSC to default socket
		//mCurrentOscAddress = juce::IPAddress::getLocalAddress().toString();
		//mCurrentOscAddress = juce::String("10.0.0.24");
		//mCurrentOscAddress = juce::String("10.0.0.78");
		mOscConnected = createOscConnection(mCurrentOscAddress, mCurrentOscOutputPort);
	}
}

void AudioDescriptorsAudioProcessor::sendOscMessage()
{
	// /controlgris/1/traj/1/x		value => [0.0; 1.0]
	// /controlgris/1/traj/1/y		value => [0.0; 1.0]
	// /controlgris/1/traj/1/z		value => [0.0; 1.0]
	// /controlgris/1/traj/1/xyz/1	value => [0.0; 1.0]
	// /controlgris/1/traj/1/xyz/2	value => [0.0; 1.0]
	// /controlgris/1/traj/1/xyz/3	value => [0.0; 1.0]
	// /controlgris/1/traj/1/xy		value => [0.0; 1.0][0.0; 1.0]
	// /controlgris/1/traj/1/xyz	value => [0.0; 1.0][0.0; 1.0][0.0; 1.0]
	// /controlgris/1/azispan		value => [0.0; 1.0]
	// /controlgris/1/elespan		value => [0.0; 1.0]

	// /controlgris/1/desc/1/x		value => [-1.66; 1.66]								// CUBE
	// /controlgris/1/desc/1/y		value => [-1.66; 1.66]								// CUBE
	// /controlgris/1/desc/1/z		value => [-1.0; 1.0]								// CUBE
	// /controlgris/1/desc/1/xyz/1	value => [-1.66; 1.66]								// CUBE
	// /controlgris/1/desc/1/xyz/2	value => [-1.66; 1.66]								// CUBE
	// /controlgris/1/desc/1/xyz/3	value => [-1.0; 1.0]								// CUBE
	// /controlgris/1/desc/1/xy		value => [-180; 180][-90; 90]						// DOME			azimuth + elevation
	// /controlgris/1/desc/1/xyz	value => [-1.66; 1.66][-1.66; 1.66][0.0; 1.0]		// CUBE			X + Y + Z
	// /controlgris/1/desc/azispan	value => [0.0; 1.0]									// DOME + CUBE
	// /controlgris/1/desc/elespan	value => [0.0; 1.0]									// DOME + CUBE

	if (!mOscConnected) {
		return;
	}

	// Uncomment to stop processing when editor window is closed
	//auto* editor{ dynamic_cast<AudioDescriptorsAudioProcessorEditor*>(getActiveEditor()) };
	//if (editor == nullptr) {
	//	return;
	//}

	juce::OSCMessage message(juce::OSCAddressPattern("/tmp"));
	auto const pluginInstance = juce::String{ "/controlgris/" } + juce::String{ mControlGrisId };

	if (mSpatMode == SpatMode::dome) {
		message.setAddressPattern(juce::OSCAddressPattern(pluginInstance + "/desc/1/domeparams"));
		message.addFloat32(static_cast<float>(mAzimuthDomeValue));
		message.addFloat32(static_cast<float>(mElevationDomeValue));
		message.addFloat32(static_cast<float>(mHspanDomeValue));
		message.addFloat32(static_cast<float>(mVspanDomeValue));
		mOscSender.send(message);
		message.clear();
	}
	else {
		message.setAddressPattern(juce::OSCAddressPattern(pluginInstance + "/desc/1/cubeparams"));
		message.addFloat32(static_cast<float>(mXCubeValue));
		message.addFloat32(static_cast<float>(mYCubeValue));
		message.addFloat32(static_cast<float>(mZCubeValue));
		message.addFloat32(static_cast<float>(mHspanCubeValue));
		message.addFloat32(static_cast<float>(mVspanCubeValue));
		mOscSender.send(message);
		message.clear();
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new AudioDescriptorsAudioProcessor();
}

void AudioDescriptorsAudioProcessor::setOnsetDetectionMetric(ParameterID paramID, int const metric)
{
	int transposedMetric{};
	switch (metric)
	{
	case 1: // Energy
	case 2: // High Frequency Content
	case 3: // Spectral Flux
		transposedMetric = metric - 1;
		break;
	default: // case 4 : // Rectified Complex Phase Deviation
		transposedMetric = 9;
		break;
	}

	switch (paramID)
	{
	case ParameterID::azimuth:
		mOnsetDetectionAzimuth.setOnesetDetectionMetric(transposedMetric);
		break;
	case ParameterID::elevation:
		mOnsetDetectionElevation.setOnesetDetectionMetric(transposedMetric);
		break;
	case ParameterID::x:
		mOnsetDetectionX.setOnesetDetectionMetric(transposedMetric);
		break;
	case ParameterID::y:
		mOnsetDetectionY.setOnesetDetectionMetric(transposedMetric);
		break;
	case ParameterID::z:
		mOnsetDetectionZ.setOnesetDetectionMetric(transposedMetric);
		break;
	case ParameterID::azimuthspan:
		mOnsetDetectionHSpan.setOnesetDetectionMetric(transposedMetric);
		break;
	case ParameterID::elevationspan:
		mOnsetDetectionVSpan.setOnesetDetectionMetric(transposedMetric);
		break;
	default:
		break;
	}
}

void AudioDescriptorsAudioProcessor::setOnsetDetectionThreshold(ParameterID paramID, const float tresh)
{
	switch (paramID)
	{
	case ParameterID::azimuth:
		mOnsetDetectionAzimuth.setOnsetDetectionThreshold(tresh);
		break;
	case ParameterID::elevation:
		mOnsetDetectionElevation.setOnsetDetectionThreshold(tresh);
		break;
	case ParameterID::x:
		mOnsetDetectionX.setOnsetDetectionThreshold(tresh);
		break;
	case ParameterID::y:
		mOnsetDetectionY.setOnsetDetectionThreshold(tresh);
		break;
	case ParameterID::z:
		mOnsetDetectionZ.setOnsetDetectionThreshold(tresh);
		break;
	case ParameterID::azimuthspan:
		mOnsetDetectionHSpan.setOnsetDetectionThreshold(tresh);
		break;
	case ParameterID::elevationspan:
		mOnsetDetectionVSpan.setOnsetDetectionThreshold(tresh);
		break;
	default:
		break;
	}
}

void AudioDescriptorsAudioProcessor::setOnsetDetectionMinTime(ParameterID paramID, const double minTime)
{
	switch (paramID)
	{
	case ParameterID::azimuth:
		mOnsetDetectionAzimuth.setOnsetDetectionMinTime(minTime);
		break;
	case ParameterID::elevation:
		mOnsetDetectionElevation.setOnsetDetectionMinTime(minTime);
		break;
	case ParameterID::x:
		mOnsetDetectionX.setOnsetDetectionMinTime(minTime);
		break;
	case ParameterID::y:
		mOnsetDetectionY.setOnsetDetectionMinTime(minTime);
		break;
	case ParameterID::z:
		mOnsetDetectionZ.setOnsetDetectionMinTime(minTime);
		break;
	case ParameterID::azimuthspan:
		mOnsetDetectionHSpan.setOnsetDetectionMinTime(minTime);
		break;
	case ParameterID::elevationspan:
		mOnsetDetectionVSpan.setOnsetDetectionMinTime(minTime);
		break;
	default:
		break;
	}
}

void AudioDescriptorsAudioProcessor::setOnsetDetectionMaxTime(ParameterID paramID, const double maxTime)
{
	switch (paramID)
	{
	case ParameterID::azimuth:
		mOnsetDetectionAzimuth.setOnsetDetectionMaxTime(maxTime);
		break;
	case ParameterID::elevation:
		mOnsetDetectionElevation.setOnsetDetectionMaxTime(maxTime);
		break;
	case ParameterID::x:
		mOnsetDetectionX.setOnsetDetectionMaxTime(maxTime);
		break;
	case ParameterID::y:
		mOnsetDetectionY.setOnsetDetectionMaxTime(maxTime);
		break;
	case ParameterID::z:
		mOnsetDetectionZ.setOnsetDetectionMaxTime(maxTime);
		break;
	case ParameterID::azimuthspan:
		mOnsetDetectionHSpan.setOnsetDetectionMaxTime(maxTime);
		break;
	case ParameterID::elevationspan:
		mOnsetDetectionVSpan.setOnsetDetectionMaxTime(maxTime);
		break;
	default:
		break;
	}
}

void AudioDescriptorsAudioProcessor::setOnsetDetectionFromClick(ParameterID paramID, const double timeValue)
{
	switch (paramID)
	{
	case ParameterID::azimuth:
		mOnsetDetectionAzimuth.setOnsetDetectionFromClick(timeValue);
		break;
	case ParameterID::elevation:
		mOnsetDetectionElevation.setOnsetDetectionFromClick(timeValue);
		break;
	case ParameterID::x:
		mOnsetDetectionX.setOnsetDetectionFromClick(timeValue);
		break;
	case ParameterID::y:
		mOnsetDetectionY.setOnsetDetectionFromClick(timeValue);
		break;
	case ParameterID::z:
		mOnsetDetectionZ.setOnsetDetectionFromClick(timeValue);
		break;
	case ParameterID::azimuthspan:
		mOnsetDetectionHSpan.setOnsetDetectionFromClick(timeValue);
		break;
	case ParameterID::elevationspan:
		mOnsetDetectionVSpan.setOnsetDetectionFromClick(timeValue);
		break;
	default:
		break;
	}
}

void AudioDescriptorsAudioProcessor::processDomeParameter(SpatialParameter& parameter, const DescriptorID& descId, double value)
{
	parameter.process(descId, value);
}

void AudioDescriptorsAudioProcessor::processCubeParameter(SpatialParameter& parameter, const DescriptorID& descId, double value)
{
	parameter.process(descId, value);
}

juce::String& AudioDescriptorsAudioProcessor::getCurrentOscAddress()
{
	return mCurrentOscAddress;
}

void AudioDescriptorsAudioProcessor::setCurrentOscAddress(juce::String& address)
{
	mCurrentOscAddress = address;
}

int AudioDescriptorsAudioProcessor::getCurrentOscPort()
{
	return mCurrentOscOutputPort;
}

void AudioDescriptorsAudioProcessor::setCurrentOscPort(int const port)
{
	mCurrentOscOutputPort = port;
}

int AudioDescriptorsAudioProcessor::getControlGrisId()
{
	return mControlGrisId;
}

void AudioDescriptorsAudioProcessor::setControlGrisId(int const id)
{
	mControlGrisId = id;
}

juce::Point<int> AudioDescriptorsAudioProcessor::getWindowDimensions() const
{
	return mWindowDimWidthHeight;
}

void AudioDescriptorsAudioProcessor::setWindowDimensions(juce::Point<int> dimensions)
{
	mWindowDimWidthHeight = dimensions;
}

AzimuthDome& AudioDescriptorsAudioProcessor::getAzimuthDome()
{
	return mAzimuthDome;
}

ElevationDome& AudioDescriptorsAudioProcessor::getElevationDome()
{
	return mElevationDome;
}

HspanDome& AudioDescriptorsAudioProcessor::getHSpanDome()
{
	return mHSpanDome;
}

VspanDome& AudioDescriptorsAudioProcessor::getVSpanDome()
{
	return mVSpanDome;
}

XCube& AudioDescriptorsAudioProcessor::getXCube()
{
	return mXCube;
}

YCube& AudioDescriptorsAudioProcessor::getYCube()
{
	return mYCube;
}

ZCube& AudioDescriptorsAudioProcessor::getZCube()
{
	return mZCube;
}

HspanCube& AudioDescriptorsAudioProcessor::getHSpanCube()
{
	return mHSpanCube;
}

VspanCube& AudioDescriptorsAudioProcessor::getVSpanCube()
{
	return mVSpanCube;
}

bool AudioDescriptorsAudioProcessor::shouldProcessDomeSpectralAnalysis()
{
	for (const auto& spatParam : mSpatParametersDomeRefs) {
		if (spatParam->needsSpectralAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessDomeLoudnessAnalysis()
{
	for (const auto& spatParam : mSpatParametersDomeRefs) {
		if (spatParam->shouldProcessLoudnessAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessDomePitchAnalysis()
{
	for (const auto& spatParam : mSpatParametersDomeRefs) {
		if (spatParam->shouldProcessPitchAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessDomeCentroidAnalysis()
{
	for (const auto& spatParam : mSpatParametersDomeRefs) {
		if (spatParam->shouldProcessCentroidAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessDomeSpreadAnalysis()
{
	for (const auto& spatParam : mSpatParametersDomeRefs) {
		if (spatParam->shouldProcessSpreadAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessDomeNoiseAnalysis()
{
	for (const auto& spatParam : mSpatParametersDomeRefs) {
		if (spatParam->shouldProcessNoiseAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessDomeOnsetDetectionAnalysis()
{
	for (const auto& spatParam : mSpatParametersDomeRefs) {
		if (spatParam->shouldProcessOnsetDetectionAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessCubeSpectralAnalysis()
{
	for (const auto& spatParam : mSpatParametersCubeRefs) {
		if (spatParam->needsSpectralAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessCubeLoudnessAnalysis()
{
	for (const auto& spatParam : mSpatParametersCubeRefs) {
		if (spatParam->shouldProcessLoudnessAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessCubePitchAnalysis()
{
	for (const auto& spatParam : mSpatParametersCubeRefs) {
		if (spatParam->shouldProcessPitchAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessCubeCentroidAnalysis()
{
	for (const auto& spatParam : mSpatParametersCubeRefs) {
		if (spatParam->shouldProcessCentroidAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessCubeSpreadAnalysis()
{
	for (const auto& spatParam : mSpatParametersCubeRefs) {
		if (spatParam->shouldProcessSpreadAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessCubeNoiseAnalysis()
{
	for (const auto& spatParam : mSpatParametersCubeRefs) {
		if (spatParam->shouldProcessNoiseAnalysis()) {
			return true;
		}
	}
	return false;
}

bool AudioDescriptorsAudioProcessor::shouldProcessCubeOnsetDetectionAnalysis()
{
	for (const auto& spatParam : mSpatParametersCubeRefs) {
		if (spatParam->shouldProcessOnsetDetectionAnalysis()) {
			return true;
		}
	}
	return false;
}

juce::String AudioDescriptorsAudioProcessor::getSpatModeToString()
{
	if (mSpatMode == SpatMode::dome) {
		return juce::String("Dome");
	}
	else {
		return juce::String("Cube");
	}
}

SpatMode AudioDescriptorsAudioProcessor::getModeState() {
	return mSpatMode;
}

void AudioDescriptorsAudioProcessor::setModeState(SpatMode spatMode) {
	mSpatMode = spatMode;
}
