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

#include <vector>
#include "algorithms/public/YINFFT.hpp"
#include "algorithms/public/RunningStats.hpp"
#include "algorithms/public/STFT.hpp"
#include "Descriptors.hpp"

class PitchD : public Descriptors {
public:
	PitchD() {
		mID = DescriptorID::pitch;
	}

	void init() override {
		mPitchRunningStats->init(mRunningStatsHistory, 1);
		//mPitchConfRunningStats->init(mRunningStatsHistory, 1);
	}

	void reset() override {
		mYin.reset(new fluid::algorithm::YINFFT{ mNBinsPitch, fluid::FluidDefaultAllocator() });
		mPitchRunningStats.reset(new fluid::algorithm::RunningStats());
		//mPitchConfRunningStats.reset(new fluid::algorithm::RunningStats());
		mStft.reset(new fluid::algorithm::STFT{ mWindowSizePitch, mFftSizePitch, mHopSizePitch });
	}

	double getValue() override {
		return mDescPitch;
	}

	void calculate(fluid::RealMatrix& pitchMat, fluid::algorithm::MultiStats& stats) {
		fluid::RealVector pitchStats = computeStats(pitchMat, stats);

		fluid::RealVector pitchMeanRes(1);
		fluid::RealVector pitchStdDevRes(1);
		//fluid::RealVector pitchConfMeanRes(1);
		//fluid::RealVector pitchConfStdDevRes(1);

		fluid::RealVectorView pitchData = fluid::RealVectorView(pitchStats(fluid::Slice(0, 1)));
		fluid::RealVectorView pitchMeanOut = fluid::RealVectorView(pitchMeanRes);
		fluid::RealVectorView pitchStdDevOut = fluid::RealVectorView(pitchStdDevRes);
		//fluid::RealVectorView pitchConfData = fluid::RealVectorView(pitchStats(fluid::Slice(7, 1)));
		//fluid::RealVectorView pitchConfMeanOut = fluid::RealVectorView(pitchConfMeanRes);
		//fluid::RealVectorView pitchConfStdDevOut = fluid::RealVectorView(pitchConfStdDevRes);

		mPitchRunningStats->process(pitchData, pitchMeanOut, pitchStdDevOut);
		//mPitchConfRunningStats->process(pitchConfData, pitchConfMeanOut, pitchConfStdDevOut);

		mDescPitch = pitchMeanOut[0];
		//mDescPitchConfidence = pitchConfMeanOut[0];
	}

	//double getDescPitchConfidence() const {
	//	return mDescPitchConfidence;
	//}

	void yinProcess(fluid::RealVector& magnitude, fluid::RealVector& pitch, double mSampleRate) {
		mYin->processFrame(magnitude, pitch, static_cast<double>(mMinFreqPitch), static_cast<double>(mMaxFreqPitch), mSampleRate, fluid::FluidDefaultAllocator());
	}

	//FONCTION POUR LE PITCH
	fluid::RealVectorView calculateWindow(fluid::RealVector& padded, int& i) {
		return padded(fluid::Slice(i * mHopSizePitch, mWindowSizePitch));
	}

	fluid::RealVector calculatePadded(fluid::RealVector in) {
		return in.size() + mWindowSizePitch + mHopSizePitch;
	}

	fluid::index calculateFrames(fluid::RealVector padded) {
		return static_cast<fluid::index>(floor((padded.size() - mWindowSizePitch) / mHopSizePitch));
	}

	fluid::Slice paddedValue(fluid::RealVector in) {
		return fluid::Slice(mHalfWindowPitch, in.size());
	}

	void setFrame(fluid::ComplexVector& frame) {
		frame.resize(mNBinsPitch);
	}

	void setMagnitude(fluid::RealVector& magnitude) {
		magnitude.resize(mNBinsPitch);
	}

	//void setMels(fluid::RealVector& mels) {
	//	mels.resize(mNBandsPitch);
	//}

	//==============================================================================
	// Stft stuff
	// Second argument is output
	void stftProcess(fluid::RealVectorView& window, fluid::ComplexVector& frame) {
		mStft->processFrame(window, frame);
	}

	// Second argument is output
	void stftMagnitude(fluid::ComplexVector& frame, fluid::RealVector& magnitude) {
		mStft->magnitude(frame, magnitude);
	}

	//==============================================================================
private:
	std::unique_ptr<fluid::algorithm::RunningStats> mPitchRunningStats;
	//std::unique_ptr<fluid::algorithm::RunningStats> mPitchConfRunningStats;
	double mDescPitch{};
	//double mDescPitchConfidence{};
	std::unique_ptr<fluid::algorithm::YINFFT> mYin;
	std::unique_ptr<fluid::algorithm::STFT>	mStft;

	//PITCH VALUE 
	fluid::index mNBinsPitch = 513;
	fluid::index mHopSizePitch = 512;
	fluid::index mWindowSizePitch = 1024;
	fluid::index mHalfWindowPitch = mWindowSizePitch / 2;
	fluid::index mFftSizePitch = 1024;
	fluid::index mNBandsPitch = 40;
	fluid::index mMinFreqPitch = 40;
	fluid::index mMaxFreqPitch = 10000;

	//==============================================================================
	JUCE_LEAK_DETECTOR(PitchD)
};
