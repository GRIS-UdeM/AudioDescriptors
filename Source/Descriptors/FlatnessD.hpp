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
#include "algorithms/public/RunningStats.hpp"
#include "Descriptors.hpp"

class FlatnessD : public Descriptors
{
public:

	void resetFlatness() {
		mFlatnessRunningStats.reset(new fluid::algorithm::RunningStats());

	}

	void initFlatness() {
		mFlatnessRunningStats->init(mRunningStatsHistory, 1);
	}

	double getDescFlatness() const {
		return mDescFlatness;
	}

	void calculate(fluid::RealVector& shapeStats)
	{
		fluid::RealVector flatnessMeanRes = fluid::RealVector(1);
		fluid::RealVector flatnessStdDevRes = fluid::RealVector(1);
		fluid::RealVectorView flatnessData = fluid::RealVectorView(shapeStats(fluid::Slice(5 * 7, 1)));
		fluid::RealVectorView flatnessMeanOut = fluid::RealVectorView(flatnessMeanRes);
		fluid::RealVectorView flatnessStdDevOut = fluid::RealVectorView(flatnessStdDevRes);
		mFlatnessRunningStats->process(flatnessData, flatnessMeanOut, flatnessStdDevOut);
		mDescFlatness = flatnessMeanOut[0];
	}


private:
	std::unique_ptr<fluid::algorithm::RunningStats> mFlatnessRunningStats;
	double mDescFlatness{};

	//==============================================================================
	JUCE_LEAK_DETECTOR(FlatnessD)
};
