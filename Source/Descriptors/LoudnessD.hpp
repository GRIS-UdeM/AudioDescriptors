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
#include "algorithms/public/Loudness.hpp"
#include "algorithms/public/RunningStats.hpp"
#include "Descriptors.hpp"


class LoudnessD : public Descriptors {
public:

	void initLoudness(double mSampleRate) {
		mLoudness->init(mWindowSizeLoudness, mSampleRate);
		mLoudnessRunningStats->init(mRunningStatsHistory, 1);
		mTruePeakRunningStats->init(mRunningStatsHistory, 1);
	}
	void resetLoudness() {
		mLoudness.reset(new fluid::algorithm::Loudness{ mWindowSizeLoudness });
		mLoudnessRunningStats.reset(new fluid::algorithm::RunningStats());
		mTruePeakRunningStats.reset(new fluid::algorithm::RunningStats());
	}

	void calculate(fluid::RealMatrix& loudnessMat, fluid::algorithm::MultiStats& mStats) {
		fluid::RealVector loudnessStats = computeStats(loudnessMat, mStats);

		fluid::RealVector loudnessMeanRes = fluid::RealVector(1);
		fluid::RealVector loudnessStdDevRes = fluid::RealVector(1);
		fluid::RealVector truePeakMeanRes = fluid::RealVector(1);
		fluid::RealVector truePeakStdDevRes = fluid::RealVector(1);
		fluid::RealVectorView loudnessData = fluid::RealVectorView(loudnessStats(fluid::Slice(0, 1)));
		fluid::RealVectorView loudnessMeanOut = fluid::RealVectorView(loudnessMeanRes);
		fluid::RealVectorView loudnessStdDevOut = fluid::RealVectorView(loudnessStdDevRes);

		fluid::RealVectorView truePeakData = fluid::RealVectorView(loudnessStats(fluid::Slice(7, 1)));
		fluid::RealVectorView truePeakMeanOut = fluid::RealVectorView(truePeakMeanRes);
		fluid::RealVectorView truePeakStdDevOut = fluid::RealVectorView(truePeakStdDevRes);

		mLoudnessRunningStats->process(loudnessData, loudnessMeanOut, loudnessStdDevOut);
		mTruePeakRunningStats->process(truePeakData, truePeakMeanOut, truePeakStdDevOut);

		mDescLoudness = loudnessMeanOut[0];
		mDescTruePeak = truePeakMeanOut[0];
	}


	double getDescLoudness() const {
		return mDescLoudness;
	}

	double getDescTruePeak() const {
		return mDescTruePeak;
	}

	void mLoudnessProcess(fluid::RealVectorView& window,fluid::RealVector& loudnessDesc) {
		mLoudness->processFrame(window, loudnessDesc, true, true);
	}



private:

	std::unique_ptr<fluid::algorithm::RunningStats> mLoudnessRunningStats;
	std::unique_ptr<fluid::algorithm::RunningStats> mTruePeakRunningStats;
	double mDescLoudness{};
	double mDescTruePeak{};
	std::unique_ptr<fluid::algorithm::Loudness> mLoudness;

	//==============================================================================
	JUCE_LEAK_DETECTOR(LoudnessD)
};
