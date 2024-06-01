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


class CentroidD : public Descriptors
{
public:

	void resetCentroid() {
		mCentroidRunningStats.reset(new fluid::algorithm::RunningStats());
	}

	void initCentroid() {
		mCentroidRunningStats->init(mRunningStatsHistory, 1);
	}
	double getDescCentroid() const {
		return mDescCentroid;
	}

	void calculate(fluid::RealVector& shapeStats)
	{
		fluid::RealVector centroidMeanRes = fluid::RealVector(1);
		fluid::RealVector centroidStdDevRes = fluid::RealVector(1);
		fluid::RealVectorView centroidData = fluid::RealVectorView(shapeStats(fluid::Slice(0, 1)));
		fluid::RealVectorView centroidMeanOut = fluid::RealVectorView(centroidMeanRes);
		fluid::RealVectorView centroidStdDevOut = fluid::RealVectorView(centroidStdDevRes);
		mCentroidRunningStats->process(centroidData, centroidMeanOut, centroidStdDevOut);
		mDescCentroid = centroidMeanOut[0];
	}

private:
	std::unique_ptr<fluid::algorithm::RunningStats> mCentroidRunningStats;
	double mDescCentroid{};


};
