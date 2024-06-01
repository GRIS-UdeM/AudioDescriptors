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
#include "Descriptors.hpp"


class PitchD : public Descriptors {
public:

	void initPitch() {
		mPitchRunningStats->init(mRunningStatsHistory, 1);
		mPitchConfRunningStats->init(mRunningStatsHistory, 1);
	}
	void resetPitch() {
		mYin.reset(new fluid::algorithm::YINFFT{ mNBinsPitch, fluid::FluidDefaultAllocator() });
		mPitchRunningStats.reset(new fluid::algorithm::RunningStats());
		mPitchConfRunningStats.reset(new fluid::algorithm::RunningStats());
	}

	void calculate(fluid::RealMatrix& pitchMat, fluid::algorithm::MultiStats& mStats) {
		fluid::RealVector pitchStats = computeStats(pitchMat, mStats);

		fluid::RealVector pitchMeanRes(1);
		fluid::RealVector pitchStdDevRes(1);
		fluid::RealVector pitchConfMeanRes(1);
		fluid::RealVector pitchConfStdDevRes(1);

		fluid::RealVectorView pitchData = fluid::RealVectorView(pitchStats(fluid::Slice(0, 1)));
		fluid::RealVectorView pitchMeanOut = fluid::RealVectorView(pitchMeanRes);
		fluid::RealVectorView pitchStdDevOut = fluid::RealVectorView(pitchStdDevRes);
		fluid::RealVectorView pitchConfData = fluid::RealVectorView(pitchStats(fluid::Slice(7, 1)));
		fluid::RealVectorView pitchConfMeanOut = fluid::RealVectorView(pitchConfMeanRes);
		fluid::RealVectorView pitchConfStdDevOut = fluid::RealVectorView(pitchConfStdDevRes);

		mPitchRunningStats->process(pitchData, pitchMeanOut, pitchStdDevOut);
		mPitchConfRunningStats->process(pitchConfData, pitchConfMeanOut, pitchConfStdDevOut);

		mDescPitch = pitchMeanOut[0];
		mDescPitchConfidence = pitchConfMeanOut[0];
	}


	double getDescPitch() const {
		return mDescPitch;
	}

	double getDescPitchConfidence() const {
		return mDescPitchConfidence;
	}

	void mYinProcess(fluid::RealVector& magnitude, fluid::RealVector& pitch, double mSampleRate) {
		mYin->processFrame(magnitude, pitch, static_cast<double>(mMinFreqPitch), static_cast<double>(mMaxFreqPitch), mSampleRate, fluid::FluidDefaultAllocator());
	}



private:
	std::unique_ptr<fluid::algorithm::RunningStats> mPitchRunningStats;
	std::unique_ptr<fluid::algorithm::RunningStats> mPitchConfRunningStats;
	double mDescPitch{};
	double mDescPitchConfidence{};
	std::unique_ptr<fluid::algorithm::YINFFT>	mYin;

};
