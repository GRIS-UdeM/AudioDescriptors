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

class RolloffD : public Descriptors
{
public:
	void resetRollof() {
		mRolloffRunningStats.reset(new fluid::algorithm::RunningStats());
	}

	void initRollof() {
		mRolloffRunningStats->init(mRunningStatsHistory, 1);
	}

	double getDescRolloff() const {
		return mDescRolloff;
	}

	void calculate(fluid::RealVector& shapeStats)
	{
		fluid::RealVector rolloffMeanRes = fluid::RealVector(1);
		fluid::RealVector rolloffStdDevRes = fluid::RealVector(1);
		fluid::RealVectorView rolloffData = fluid::RealVectorView(shapeStats(fluid::Slice(4 * 7, 1)));
		fluid::RealVectorView rolloffMeanOut = fluid::RealVectorView(rolloffMeanRes);
		fluid::RealVectorView rolloffStdDevOut = fluid::RealVectorView(rolloffStdDevRes);
		mRolloffRunningStats->process(rolloffData, rolloffMeanOut, rolloffStdDevOut);
		mDescRolloff = rolloffMeanOut[0];
	}


private:
	std::unique_ptr<fluid::algorithm::RunningStats> mRolloffRunningStats;
	double mDescRolloff{};

};
