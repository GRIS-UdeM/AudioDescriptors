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
	, domeSettings(mAudioProcessorValueTreeState)
	, cubeSettings(mAudioProcessorValueTreeState)
{
	//startTimerHz(50);
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
	//mStft.resetStft();
	mStftPitch.resetStftPitch();
	mStftSpectral.resetStftSpectral();

	mBands.resetBands();
	mDct.resetDct();

	mPitch.resetPitch();
	mLoudness.resetLoudness();
	mStats.resetStats();
	mShape.resetShape();
	mCentroid.resetCentroid();
	mSpread.resetSpread();
	mFlatness.resetFlatness();
	mOnsetDetectionAzimuth.resetOnsetDetection();
	mOnsetDetectionElevation.resetOnsetDetection();
	mOnsetDetectionHSpan.resetOnsetDetection();
	mOnsetDetectionVSpan.resetOnsetDetection();
	mOnsetDetectionX.resetOnsetDetection();
	mOnsetDetectionY.resetOnsetDetection();
	mOnsetDetectionZ.resetOnsetDetection();
}

void AudioDescriptorsAudioProcessor::initFunction()
{
	mBands.initBands(mSampleRate);
	mDct.initDct();
	mPitch.initPitch();
	mLoudness.initLoudness(mSampleRate);
	mStats.initStats();
	mCentroid.initCentroid();
	mSpread.initSpread();
	mFlatness.initFlatness();
	mOnsetDetectionAzimuth.initOnsetDetection();
	mOnsetDetectionElevation.initOnsetDetection();
	mOnsetDetectionHSpan.initOnsetDetection();
	mOnsetDetectionVSpan.initOnsetDetection();
	mOnsetDetectionX.initOnsetDetection();
	mOnsetDetectionY.initOnsetDetection();
	mOnsetDetectionZ.initOnsetDetection();
}
//==============================================================================
void AudioDescriptorsAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
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

	RealVector paddedLoudness = ops.calculatePaddedLoudness(inLoudness);
	fluid::index nFramesLoudness = ops.calculateFramesLoudness(paddedLoudness);
	RealMatrix loudnessMat(nFramesLoudness, 2);
	std::fill(paddedLoudness.begin(), paddedLoudness.end(), 0);
	paddedLoudness(ops.paddedValueLoudness(inLoudness)) <<= inLoudness;

	RealVector paddedPitch = ops.calculatePaddedPitch(inPitch);
    fluid::index nFramesPitch = ops.calculateFramesPitch(paddedPitch);
	RealMatrix pitchMat(nFramesPitch, 2);
	std::fill(paddedPitch.begin(), paddedPitch.end(), 0);
	paddedPitch(ops.paddedValuePitch(inPitch)) <<= inPitch;

	RealVector paddedSpectral = ops.calculatePaddedSpectral(inSpectral);
    fluid::index nFramesSpectral = ops.calculateFramesSpectral(paddedSpectral);
	RealMatrix shapeMat(nFramesSpectral, 7);
	std::fill(paddedSpectral.begin(), paddedSpectral.end(), 0);
	paddedSpectral(ops.paddedValueSpectral(inSpectral)) <<= inSpectral;
	RealVector  shapeStats;

	if (getInterfaceState() == true) {
		if (domeSettings.checkConditionForLoudnessAnalyse() || cubeSettings.checkConditionForLoudnessAnalyse()) {
			for (int i = 0; i < nFramesLoudness; i++) {
				RealVector loudnessDesc(2);
				RealVectorView windowLoudness = ops.calculateWindowLoudness(paddedLoudness, i);
				mLoudness.mLoudnessProcess(windowLoudness, loudnessDesc);
				loudnessMat.row(i) <<= loudnessDesc;
			}
			mLoudness.calculate(loudnessMat, *mStats.getStats());
			double loudnessValue = mLoudness.getDescLoudness();
			if (getModeState() == SpatMode::dome) {
				loudnessValue = mParamFunctions.DbToGain(loudnessValue);
				if (domeSettings.checkConditionLoudnessAzimuth()) {
					DBG("--------------Azimuth Loudness -------------------");
					processDomeParameter(domeSettings.getAzimuthDome(), 2, loudnessValue, true, false);
					mAzimuthDomeValue = domeSettings.getAzimuthDome().getValue();
				}
				if (domeSettings.checkConditionLoudnessElevation()) {
					DBG("--------------Elevation Loudness -------------------");
					processDomeParameter(domeSettings.getElevationDome(), 2, loudnessValue, false, true);
					mElevationDomeValue = domeSettings.getElevationDome().getValue();
				}
				if (domeSettings.checkConditionLoudnessHSpan()) {
					DBG("--------------HSpan Loudness -------------------");
					processDomeParameter(domeSettings.getHSpanDome(), 2, loudnessValue, false, false);
					mHspanDomeValue = domeSettings.getHSpanDome().getValue();
				}
				if (domeSettings.checkConditionLoudnessVSpan()) {
					DBG("--------------VSpan Loudness -------------------");
					processDomeParameter(domeSettings.getVSpanDome(), 2, loudnessValue, false,false);
					mVspanDomeValue = domeSettings.getVSpanDome().getValue();
				}
			}
			else {
				loudnessValue = mParamFunctions.DbToGain(loudnessValue);
				if (cubeSettings.checkConditionLoudnessX()) {
					DBG("--------------X Loudness -------------------");
					processCubeParameter(cubeSettings.getXCube(), 2, loudnessValue, false);
					mXCubeValue = cubeSettings.getXCube().getValue();
				}
				if (cubeSettings.checkConditionLoudnessY()) {
					DBG("--------------Y Loudness -------------------");
					processCubeParameter(cubeSettings.getYCube(), 2, loudnessValue, false);
					mYCubeValue = cubeSettings.getYCube().getValue();
				}
				if (cubeSettings.checkConditionLoudnessZ()) {
					DBG("--------------Z Loudness -------------------");
					processCubeParameter(cubeSettings.getZCube(), 2, loudnessValue, true);
					mZCubeValue = cubeSettings.getZCube().getValue();
				}
				if (cubeSettings.checkConditionLoudnessHSpan()) {
					DBG("--------------HSpan Loudness -------------------");
					processCubeParameter(cubeSettings.getHSpanCube(), 2, loudnessValue, false);
					mHspanCubeValue = cubeSettings.getHSpanCube().getValue();
				}
				if (cubeSettings.checkConditionLoudnessVSpan()) {
					DBG("--------------VSpan Loudness -------------------");
					processCubeParameter(cubeSettings.getVSpanCube(), 2, loudnessValue, true);
					mVspanCubeValue = cubeSettings.getVSpanCube().getValue();
				}
			}
		}
		if (domeSettings.checkConditionForPitchAnalyse() || cubeSettings.checkConditionForPitchAnalyse()) {
			ComplexVector framePitch;
			RealVector magnitudePitch;
			RealVector melsPitch;
			for (int j = 0; j < nFramesPitch; j++) {
				ops.setFramePitch(framePitch);
				ops.setMagnitudePitch(magnitudePitch);
				RealVector     pitch(2);
				RealVectorView windowPitch = ops.calculateWindowPitch(paddedPitch, j);

				mStftPitch.stftProcess(windowPitch, framePitch);
				mStftPitch.stftMagntiude(framePitch, magnitudePitch);
				mPitch.mYinProcess(magnitudePitch, pitch, mSampleRate);
				pitchMat.row(j) <<= pitch;
			}
			mPitch.calculate(pitchMat, *mStats.getStats());
			double pitchValue = mPitch.getDescPitch();
			if (getModeState() == SpatMode::dome) {
				pitchValue = mParamFunctions.frequencyToMidiNoteNumber(pitchValue);
				if (domeSettings.checkConditionPitchAzimuth()) {
					DBG("--------------Azimuth Pitch -------------------");
					processDomeParameter(domeSettings.getAzimuthDome(), 3, pitchValue, true, false);
					mAzimuthDomeValue = domeSettings.getAzimuthDome().getValue();
				}
				if (domeSettings.checkConditionPitchElevation()) {
					DBG("--------------Elevation Pitch -------------------");
					processDomeParameter(domeSettings.getElevationDome(), 3, pitchValue, false, true);
					mElevationDomeValue = domeSettings.getElevationDome().getValue();
				}
				if (domeSettings.checkConditionPitchHSpan()) {
					DBG("--------------HSpan Pitch -------------------");
					processDomeParameter(domeSettings.getHSpanDome(), 3, pitchValue, false, false);
					mHspanDomeValue = domeSettings.getHSpanDome().getValue();
				}
				if (domeSettings.checkConditionPitchVSpan()) {
					DBG("--------------VSpan Pitch -------------------");
					processDomeParameter(domeSettings.getVSpanDome(), 3, pitchValue, false, false );
					mVspanDomeValue = domeSettings.getVSpanDome().getValue();
				}
			}
			else {
				pitchValue = mParamFunctions.frequencyToMidiNoteNumber(pitchValue);
				if (cubeSettings.checkConditionPitchX()) {
					DBG("--------------X Pitch -------------------");
					processCubeParameter(cubeSettings.getXCube(), 3, pitchValue, false);
					mXCubeValue = cubeSettings.getXCube().getValue();
				}
				if (cubeSettings.checkConditionPitchY()) {
					DBG("--------------Y Pitch -------------------");
					processCubeParameter(cubeSettings.getYCube(), 3, pitchValue, false);
					mYCubeValue = cubeSettings.getYCube().getValue();
				}
				if (cubeSettings.checkConditionPitchZ()) {
					DBG("--------------Z Pitch -------------------");
					processCubeParameter(cubeSettings.getZCube(), 3, pitchValue, true);
					mZCubeValue = cubeSettings.getZCube().getValue();
				}
				if (cubeSettings.checkConditionPitchHSpan()) {
					DBG("--------------HSpan Pitch -------------------");
					processCubeParameter(cubeSettings.getHSpanCube(), 3, pitchValue, false);
					mHspanCubeValue = cubeSettings.getHSpanCube().getValue();
				}
				if (cubeSettings.checkConditionPitchVSpan()) {
					DBG("--------------VSpan Pitch -------------------");
					processCubeParameter(cubeSettings.getVSpanCube(), 3, pitchValue, true);
					mVspanCubeValue = cubeSettings.getVSpanCube().getValue();
				}
			}
		}
		if ((mSpatMode == SpatMode::dome && domeSettings.checkConditionNeedSpectralAnalyse()) ||
			(mSpatMode == SpatMode::cube && cubeSettings.checkConditionNeedSpectralAnalyse())) {
			ComplexVector  frameSpectral;
			RealVector     magnitudeSpectral;
			for (int y = 0; y < nFramesSpectral; y++) {
				ops.setFrameSpectral(frameSpectral);
				ops.setMagnitudeSpectral(magnitudeSpectral);
				RealVector     shapeDesc(7);
				RealVectorView windowSpectral = ops.calculateWindowSpectral(paddedSpectral, y);
				mStftSpectral.stftProcess(windowSpectral, frameSpectral);
				mStftSpectral.stftMagntiude(frameSpectral, magnitudeSpectral);
				mShape.mShapeProcess(magnitudeSpectral, shapeDesc, mSampleRate);
				shapeMat.row(y) <<= shapeDesc;
			}

			shapeStats = mShape.shapeCalculate(shapeMat, *mStats.getStats());
			mCentroid.calculate(shapeStats);
			double centroidValue = mCentroid.getDescCentroid(); // centroidValue when silence = 118.02870609942256
			if (bufferMagnitude == 0.0f) {
				centroidValue = 0.0;
			}

			mSpread.calculate(shapeStats);
			double spreadValue = mSpread.getDescSpread(); // spreadValue when silence  = 16.520351353896057
			if (bufferMagnitude == 0.0f) {
				spreadValue = 0.0;
			}
			double zmap = mParamFunctions.zmap(spreadValue, 0.0, 16.0);

			mFlatness.calculate(shapeStats);
			double flatnessValue = mFlatness.getDescFlatness(); // flatnessValue when silence = -6.9624443085150120e-13
			if (bufferMagnitude == 0.0f) {
				flatnessValue = -160.0;
			}

			if (getModeState() == SpatMode::dome) {
				flatnessValue = mParamFunctions.DbToGain(flatnessValue);
				flatnessValue = mParamFunctions.zmap(flatnessValue, 0.0, 0.5);
				flatnessValue = mParamFunctions.power(flatnessValue);
				if (domeSettings.checkConditionForCentroidAnalyse()) {
					if (domeSettings.checkConditionCentroidAzimuth()) {
						DBG("--------------Azimuth Centroid -------------------");
						processDomeParameter(domeSettings.getAzimuthDome(), 4, centroidValue, true, false);
						mAzimuthDomeValue = domeSettings.getAzimuthDome().getValue();
					}
					if (domeSettings.checkConditionCentroidElevation()) {
						DBG("--------------Elevation Centroid -------------------");
						processDomeParameter(domeSettings.getElevationDome(), 4, centroidValue, false, true);
						mElevationDomeValue = domeSettings.getElevationDome().getValue();
					}
					if (domeSettings.checkConditionCentroidHSpan()) {
						DBG("--------------HSpan Centroid -------------------");
						processDomeParameter(domeSettings.getHSpanDome(), 4, centroidValue, false, false);
						mHspanDomeValue = domeSettings.getHSpanDome().getValue();
					}
					if (domeSettings.checkConditionCentroidVSpan()) {
						DBG("--------------VSpan Centroid -------------------");
						processDomeParameter(domeSettings.getVSpanDome(), 4, centroidValue, false, false);
						mVspanDomeValue = domeSettings.getVSpanDome().getValue();
					}
				}
				if (domeSettings.checkConditionForSpreadAnalyse()) {
					if (domeSettings.checkConditionSpreadAzimuth()) {
						DBG("--------------Azimuth Spread -------------------");
						processDomeParameter(domeSettings.getAzimuthDome(), 5, zmap, true, false);
						mAzimuthDomeValue = domeSettings.getAzimuthDome().getValue();
					}
					if (domeSettings.checkConditionSpreadElevation()) {
						DBG("--------------Elevation Spread -------------------");
						processDomeParameter(domeSettings.getElevationDome(), 5, zmap, false, true);
						mElevationDomeValue = domeSettings.getElevationDome().getValue();
					}
					if (domeSettings.checkConditionSpreadHSpan()) {
						DBG("--------------HSpan Spread -------------------");
						processDomeParameter(domeSettings.getHSpanDome(), 5, zmap, false, false);
						mHspanDomeValue = domeSettings.getHSpanDome().getValue();
					}
					if (domeSettings.checkConditionSpreadVSpan()) {
						DBG("--------------VSpan Spread -------------------");
						processDomeParameter(domeSettings.getVSpanDome(), 5, zmap, false, false);
						mVspanDomeValue = domeSettings.getVSpanDome().getValue();
					}
				}
				if (domeSettings.checkConditionForNoiseAnalyse()) {	
					if (domeSettings.checkConditionNoiseAzimuth()) {
						DBG("--------------Azimuth Noise -------------------");
						processDomeParameter(domeSettings.getAzimuthDome(), 6, flatnessValue, true, false);
						mAzimuthDomeValue = domeSettings.getAzimuthDome().getValue();
					}
					if (domeSettings.checkConditionNoiseElevation()) {
						DBG("--------------Elevation Noise -------------------");
						processDomeParameter(domeSettings.getElevationDome(), 6, flatnessValue, false, true);
						mElevationDomeValue = domeSettings.getElevationDome().getValue();
					}
					if (domeSettings.checkConditionNoiseHSpan()) {
						DBG("--------------HSpan Noise -------------------");
						processDomeParameter(domeSettings.getHSpanDome(), 6, flatnessValue, false, false);
						mHspanDomeValue = domeSettings.getHSpanDome().getValue();
					}
					if (domeSettings.checkConditionNoiseVSpan()) {
						DBG("--------------VSpan Noise -------------------");
						processDomeParameter(domeSettings.getVSpanDome(), 6, flatnessValue, false, false);
						mVspanDomeValue = domeSettings.getVSpanDome().getValue();
					}
				}
			}
			else {
				flatnessValue = mParamFunctions.DbToGain(flatnessValue);
				flatnessValue = mParamFunctions.zmap(flatnessValue, 0.0, 0.5);
				flatnessValue = mParamFunctions.power(flatnessValue);
				if (cubeSettings.checkConditionForCentroidAnalyse()) {
					if (cubeSettings.checkConditionCentroidX()) {
						DBG("--------------X Centroid -------------------");
						processCubeParameter(cubeSettings.getXCube(), 4, centroidValue, false);
						mXCubeValue = cubeSettings.getXCube().getValue();
					}
					if (cubeSettings.checkConditionCentroidY()) {
						DBG("--------------Y Centroid -------------------");
						processCubeParameter(cubeSettings.getYCube(), 4, centroidValue, false);
						mYCubeValue = cubeSettings.getYCube().getValue();
					}
					if (cubeSettings.checkConditionCentroidZ()) {
						DBG("--------------Z Centroid -------------------");
						processCubeParameter(cubeSettings.getZCube(), 4, centroidValue, true);
						mZCubeValue = cubeSettings.getZCube().getValue();
					}
					if (cubeSettings.checkConditionCentroidHSpan()) {
						DBG("--------------HSpan Centroid -------------------");
						processCubeParameter(cubeSettings.getHSpanCube(), 4, centroidValue, false);
						mHspanCubeValue = cubeSettings.getHSpanCube().getValue();
					}
					if (cubeSettings.checkConditionCentroidVSpan()) {
						DBG("--------------VSpan Centroid -------------------");
						processCubeParameter(cubeSettings.getVSpanCube(), 4, centroidValue, true);
						mVspanCubeValue = cubeSettings.getVSpanCube().getValue();
					}
				}
				if (cubeSettings.checkConditionForSpreadAnalyse()) {
					if (cubeSettings.checkConditionSpreadX()) {
						DBG("--------------X Spread -------------------");
						processCubeParameter(cubeSettings.getXCube(), 5, zmap, false);
						mXCubeValue = cubeSettings.getXCube().getValue();
					}
					if (cubeSettings.checkConditionSpreadY()) {
						DBG("--------------Y Spread -------------------");
						processCubeParameter(cubeSettings.getYCube(), 5, zmap, false);
						mYCubeValue = cubeSettings.getYCube().getValue();
					}
					if (cubeSettings.checkConditionSpreadZ()) {
						DBG("--------------Z Spread -------------------");
						processCubeParameter(cubeSettings.getZCube(), 5, zmap, true);
						mZCubeValue = cubeSettings.getZCube().getValue();
					}
					if (cubeSettings.checkConditionSpreadHSpan()) {
						DBG("--------------HSpan Spread -------------------");
						processCubeParameter(cubeSettings.getHSpanCube(), 5, zmap, false);
						mHspanCubeValue = cubeSettings.getHSpanCube().getValue();
					}
					if (cubeSettings.checkConditionSpreadVSpan()) {
						DBG("--------------VSpan Spread -------------------");
						processCubeParameter(cubeSettings.getVSpanCube(), 5, zmap, true);
						mVspanCubeValue = cubeSettings.getVSpanCube().getValue();
					}
				}
				if (cubeSettings.checkConditionForNoiseAnalyse()) {
					if (cubeSettings.checkConditionNoiseX()) {
						DBG("--------------X Noise -------------------");
						processCubeParameter(cubeSettings.getXCube(), 6, flatnessValue, false);
						mXCubeValue = cubeSettings.getXCube().getValue();
					}
					if (cubeSettings.checkConditionNoiseY()) {
						DBG("--------------Y Noise -------------------");
						processCubeParameter(cubeSettings.getYCube(), 6, flatnessValue, false);
						mYCubeValue = cubeSettings.getYCube().getValue();
					}
					if (cubeSettings.checkConditionNoiseZ()) {
						DBG("--------------Z Noise -------------------");
						processCubeParameter(cubeSettings.getZCube(), 6, flatnessValue, true);
						mZCubeValue = cubeSettings.getZCube().getValue();
					}
					if (cubeSettings.checkConditionNoiseHSpan()) {
						DBG("--------------HSpan Noise -------------------");
						processCubeParameter(cubeSettings.getHSpanCube(), 6, flatnessValue, false);
						mHspanCubeValue = cubeSettings.getHSpanCube().getValue();
					}
					if (cubeSettings.checkConditionNoiseVSpan()) {
						DBG("--------------VSpan Noise -------------------");
						processCubeParameter(cubeSettings.getVSpanCube(), 6, flatnessValue, true);
						mVspanCubeValue = cubeSettings.getVSpanCube().getValue();
					}
				}
			}
		}
		if (domeSettings.checkConditionOnsetDetectionAnalyse() || cubeSettings.checkConditionOnsetDetectionAnalyse()) {
			if (getModeState() == SpatMode::dome) {
				if (domeSettings.checkConditionOnsetDetectionAzimuth()) {
					//DBG("--------------Azimuth Iterations Speed-----------------");
					mOnsetDetectionAzimuth.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionAzimuth.getOnsetDetectionValue() };
					processDomeParameter(domeSettings.getAzimuthDome(), 7, onsetDetectionValue, true, false);
					mAzimuthDomeValue = domeSettings.getAzimuthDome().getValue();
				}
				if (domeSettings.checkConditionOnsetDetectionElevation()) {
					//DBG("--------------Elevation Iterations Speed-----------------");
					mOnsetDetectionElevation.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionElevation.getOnsetDetectionValue() };
					processDomeParameter(domeSettings.getElevationDome(), 7, onsetDetectionValue, false, false);
					mElevationDomeValue = domeSettings.getElevationDome().getValue();
				}
				if (domeSettings.checkConditionOnsetDetectionHSpan()) {
					//DBG("--------------HSpan Iterations Speed-----------------");
					mOnsetDetectionHSpan.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionHSpan.getOnsetDetectionValue() };
					processDomeParameter(domeSettings.getHSpanDome(), 7, onsetDetectionValue, false, false);
					mHspanDomeValue = domeSettings.getHSpanDome().getValue();
				}
				if (domeSettings.checkConditionOnsetDetectionVSpan()) {
					//DBG("--------------VSpan Iterations Speed-----------------");
					mOnsetDetectionVSpan.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionVSpan.getOnsetDetectionValue() };
					processDomeParameter(domeSettings.getVSpanDome(), 7, onsetDetectionValue, false, true);
					mVspanDomeValue = domeSettings.getVSpanDome().getValue();
				}
			}
			else {
				if (cubeSettings.checkConditionOnsetDetectionX()) {
					//DBG("--------------X Iterations Speed-----------------");
					mOnsetDetectionX.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionX.getOnsetDetectionValue() };
					processCubeParameter(cubeSettings.getXCube(), 7, onsetDetectionValue, false);
					mXCubeValue = cubeSettings.getXCube().getValue();
				}
				if (cubeSettings.checkConditionOnsetDetectionY()) {
					//DBG("--------------Y Iterations Speed-----------------");
					mOnsetDetectionY.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionY.getOnsetDetectionValue() };
					processCubeParameter(cubeSettings.getYCube(), 7, onsetDetectionValue, false);
					mYCubeValue = cubeSettings.getYCube().getValue();
				}
				if (cubeSettings.checkConditionOnsetDetectionZ()) {
					//DBG("--------------Z Iterations Speed-----------------");
					mOnsetDetectionZ.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionZ.getOnsetDetectionValue() };
					processCubeParameter(cubeSettings.getZCube(), 7, onsetDetectionValue, true);
					mZCubeValue = cubeSettings.getZCube().getValue();
				}
				if (cubeSettings.checkConditionOnsetDetectionHSpan()) {
					//DBG("--------------HSpan Iterations Speed-----------------");
					mOnsetDetectionHSpan.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionHSpan.getOnsetDetectionValue() };
					processCubeParameter(cubeSettings.getHSpanCube(), 7, onsetDetectionValue, false);
					mHspanCubeValue = cubeSettings.getHSpanCube().getValue();
				}
				if (cubeSettings.checkConditionOnsetDetectionVSpan()) {
					//DBG("--------------VSpan Iterations Speed-----------------");
					mOnsetDetectionVSpan.mOnsetDetectionProcess(mDescriptorsBuffer, mSampleRate, mBlockSize);
					auto const onsetDetectionValue{ mOnsetDetectionVSpan.getOnsetDetectionValue() };
					processCubeParameter(cubeSettings.getVSpanCube(), 7, onsetDetectionValue, true);
					mVspanCubeValue = cubeSettings.getVSpanCube().getValue();
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
			domeSettings.updateDomeParametersState();
			cubeSettings.updateCubeParametersState();

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

	auto const state{ mAudioProcessorValueTreeState.copyState() };
	state;
}

void AudioDescriptorsAudioProcessor::timerCallback()
{
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

	auto* editor{ dynamic_cast<AudioDescriptorsAudioProcessorEditor*>(getActiveEditor()) };
	if (editor == nullptr) {
		return;
	}

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

/////////////Gestion de l'interface////////////////////////////////

bool AudioDescriptorsAudioProcessor::getInterfaceState() {
	return interfaceOpened;
}

void AudioDescriptorsAudioProcessor::setInterfaceState() {
	interfaceOpened = true;
}

void AudioDescriptorsAudioProcessor::processDomeParameter(Parameters& parameter, int index, double value, bool isAzimuth, bool isOffset)
{
	if (index == 2) {
		value = mParamFunctions.PourcentageConversion(value, parameter.getParamFactorLoudness());
		double smoothedValue = parameter.getSmoothLoudness().DoSmooth(value, parameter.getParamSmoothLoudness(),
			parameter.getParamSmoothCoefLoudness());
		if (isAzimuth) {
			parameter.parameters(parameter.getParamRangeLoudness(), smoothedValue,
				parameter.getParamLapLoudness());
		}
		else if (isOffset) {
			parameter.parameters(parameter.getParamRangeLoudness(), smoothedValue, 1.0,
				parameter.getParamOffsetLoudness());
		}
		else {
			parameter.parameters(parameter.getParamRangeLoudness(), smoothedValue);
		}
	}
	else if (index == 3) {
		double minFreq = mParamFunctions.frequencyToMidiNoteNumber(parameter.getParamMinFreqPitch());
		double maxFreq = mParamFunctions.frequencyToMidiNoteNumber(parameter.getParamMaxFreqPitch());
		double zmap = mParamFunctions.zmap(value, minFreq, maxFreq);
		double smoothedValuePitch = parameter.getSmoothPitch().DoSmooth(zmap, parameter.getParamSmoothPitch(),
			parameter.getParamSmoothCoefPitch());
		if (isAzimuth) {
			parameter.parameters(parameter.getParamRangePitch(), smoothedValuePitch,
				parameter.getParamLapPitch());

		} else if(isOffset){
			parameter.parameters(parameter.getParamRangePitch(), smoothedValuePitch, 1.0,
				parameter.getParamOffsetPitch());
		}
		else {
			parameter.parameters(parameter.getParamRangePitch(), smoothedValuePitch);
		}
	}
	else if (index == 4) {
		double minFreq = mParamFunctions.frequencyToMidiNoteNumber(parameter.getParamMinFreqCentroid());
		double maxFreq = mParamFunctions.frequencyToMidiNoteNumber(parameter.getParamMaxFreqCentroid());
		double zmap = mParamFunctions.zmap(value, minFreq, maxFreq);
		double smoothedValueCentroid = parameter.getSmoothCentroid().DoSmooth(zmap, parameter.getParamSmoothCentroid(),
			parameter.getParamSmoothCoefCentroid());
		if (isAzimuth) {
			parameter.parameters(parameter.getParamRangeCentroid(), smoothedValueCentroid,
				parameter.getParamLapCentroid());
		}
		else if (isOffset) {
			parameter.parameters(parameter.getParamRangeCentroid(), smoothedValueCentroid, 1.0,
				parameter.getParamOffsetCentroid());
		}
		else {
			parameter.parameters(parameter.getParamRangeCentroid(), smoothedValueCentroid);
		}
	}
	else if (index == 5) {
		double ScaleOne = parameter.getParamFactorSpread();
		ScaleOne = mParamFunctions.zmap(ScaleOne, 100.0, 500.0);
		ScaleOne = mParamFunctions.subtractFromOne(ScaleOne);
		double power = mParamFunctions.calculatePower(value, ScaleOne);
		double mExpr = mParamFunctions.expr(power);
		double ScaleTwo = parameter.getParamFactorSpread();
		ScaleTwo = mParamFunctions.ClipMyValue(ScaleTwo);
		double mValueToSmooth = mParamFunctions.valueToSmooth(mExpr, ScaleTwo);
		double mSmoothedValue = parameter.getSmoothSpread().DoSmooth(mValueToSmooth, parameter.getParamSmoothSpread(),
			parameter.getParamSmoothCoefSpread());
		if (isAzimuth) {
			parameter.parameters(parameter.getParamRangeSpread(), mSmoothedValue,
				parameter.getParamLapSpread());
		}
		else if (isOffset) {
			parameter.parameters(parameter.getParamRangeSpread(), mSmoothedValue, 1.0,
				parameter.getParamOffsetSpread());
		}
		else {
			parameter.parameters(parameter.getParamRangeSpread(), mSmoothedValue);
		}
	}
	else if (index == 6) {
		value = value * (parameter.getParamFactorNoise() * 0.01);
		value = parameter.getSmoothNoise().DoSmooth(value, parameter.getParamSmoothNoise(),
			parameter.getParamSmoothCoefNoise());
		if (isAzimuth) {
			parameter.parameters(parameter.getParamRangeNoise(), value,
				parameter.getParamLapNoise());
		}
		else if (isOffset) {
			parameter.parameters(parameter.getParamRangeNoise(), value, 1.0,
				parameter.getParamOffsetNoise());
		}
		else {
			parameter.parameters(parameter.getParamRangeNoise(), value);
		}
	}
	else if (index == 7) {
		auto& smoothOnsetDetection{ parameter.getSmoothOnsetDetection() };
		auto smoothValue = parameter.getParamSmoothOnsetDetection();
		auto smooth = smoothOnsetDetection.DoSmooth(value, smoothValue, parameter.getParamSmoothCoefOnsetDetection());
		auto range = parameter.getParamRangeOnsetDetection();
		if (isAzimuth) {
			parameter.parameters(range, smooth, parameter.getParamLapOnsetDetection());
		}
		else if (isOffset) {
			parameter.parameters(range, smooth, 1.0, parameter.getParamOffsetOnsetDetection());
		}
		else {
			parameter.parameters(range, smooth);
		}
	}
}

void AudioDescriptorsAudioProcessor::processCubeParameter(Parameters& parameter, int index, double value, bool isOffset)
{
	if (index == 2) {
		value = mParamFunctions.PourcentageConversion(value, parameter.getParamFactorLoudness());
		double smoothedValue = parameter.getSmoothLoudness().DoSmooth(value, parameter.getParamSmoothLoudness(),
			parameter.getParamSmoothCoefLoudness());
		if (isOffset) {
			parameter.parameters(parameter.getParamRangeLoudness(), smoothedValue, 1.0,
				parameter.getParamOffsetLoudness());
		}
		else {
			parameter.parameters(parameter.getParamRangeLoudness(), smoothedValue);
		}
	}
	else if (index == 3) {
		double minFreq = mParamFunctions.frequencyToMidiNoteNumber(parameter.getParamMinFreqPitch());
		double maxFreq = mParamFunctions.frequencyToMidiNoteNumber(parameter.getParamMaxFreqPitch());
		double zmap = mParamFunctions.zmap(value, minFreq, maxFreq);
		double smoothedValuePitch = parameter.getSmoothPitch().DoSmooth(zmap, parameter.getParamSmoothPitch(),
			parameter.getParamSmoothCoefPitch());
		if (isOffset) {
			parameter.parameters(parameter.getParamRangePitch(), smoothedValuePitch, 1.0,
				parameter.getParamOffsetPitch());
		}
		else {
			parameter.parameters(parameter.getParamRangePitch(), smoothedValuePitch);
		}
	}
	else if (index == 4) {
		double minFreq = mParamFunctions.frequencyToMidiNoteNumber(parameter.getParamMinFreqCentroid());
		double maxFreq = mParamFunctions.frequencyToMidiNoteNumber(parameter.getParamMaxFreqCentroid());
		double zmap = mParamFunctions.zmap(value, minFreq, maxFreq);
		double smoothedValueCentroid = parameter.getSmoothCentroid().DoSmooth(zmap, parameter.getParamSmoothCentroid(),
			parameter.getParamSmoothCoefCentroid());
		if (isOffset) {
			parameter.parameters(parameter.getParamRangeCentroid(), smoothedValueCentroid, 1.0,
				parameter.getParamOffsetCentroid());
		}
		else {
			parameter.parameters(parameter.getParamRangeCentroid(), smoothedValueCentroid);
		}
	}
	else if (index == 5) {
		double ScaleOne = parameter.getParamFactorSpread();
		ScaleOne = mParamFunctions.zmap(ScaleOne, 100.0, 500.0);
		ScaleOne = mParamFunctions.subtractFromOne(ScaleOne);
		double power = mParamFunctions.calculatePower(value, ScaleOne);
		double mExpr = mParamFunctions.expr(power);
		double ScaleTwo = parameter.getParamFactorSpread();
		ScaleTwo = mParamFunctions.ClipMyValue(ScaleTwo);
		double mValueToSmooth = mParamFunctions.valueToSmooth(mExpr, ScaleTwo);
		double mSmoothedValue = parameter.getSmoothSpread().DoSmooth(mValueToSmooth, parameter.getParamSmoothSpread(),
			parameter.getParamSmoothCoefSpread());
		if (isOffset) {
			parameter.parameters(parameter.getParamRangeSpread(), mSmoothedValue, 1.0,
				parameter.getParamOffsetSpread());
		}
		else {
			parameter.parameters(parameter.getParamRangeSpread(), mSmoothedValue);
		}
	}
	else if (index == 6) {
		value = value * (parameter.getParamFactorNoise() * 0.01);
		value = parameter.getSmoothNoise().DoSmooth(value, parameter.getParamSmoothNoise(),
			parameter.getParamSmoothCoefNoise());
		if (isOffset) {
			parameter.parameters(parameter.getParamRangeNoise(), value, 1.0,
				parameter.getParamOffsetNoise());
		}
		else {
		parameter.parameters(parameter.getParamRangeNoise(), value);
		}
	}
	else if (index == 7) {
		auto& smoothOnsetDetection{ parameter.getSmoothOnsetDetection() };
		auto smoothValue = parameter.getParamSmoothOnsetDetection();
		auto smooth = smoothOnsetDetection.DoSmooth(value, smoothValue, parameter.getParamSmoothCoefOnsetDetection());
		auto range = parameter.getParamRangeOnsetDetection();
		if (isOffset) {
			parameter.parameters(range, smooth,1.0, parameter.getParamOffsetOnsetDetection());
		}
		else {
			parameter.parameters(range, smooth);
		}
	}
}

///////////Get DomeSettings et getcubesettings////////////////
DomeSettings& AudioDescriptorsAudioProcessor::getDomeSettings() {
	return domeSettings;
}

CubeSettings& AudioDescriptorsAudioProcessor::getCubeSettings() {
	return cubeSettings;
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
