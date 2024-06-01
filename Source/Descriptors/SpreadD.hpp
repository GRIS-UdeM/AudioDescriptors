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


class SpreadD : public Descriptors
{
public:

	void resetSpread() {
		mSpreadRunningStats.reset(new fluid::algorithm::RunningStats());
	}

	void initSpread() {
		mSpreadRunningStats->init(mRunningStatsHistory, 1);
	}
	double getDescSpread() const {
		return mDescSpread;
	}

	void calculate(fluid::RealVector& shapeStats)
	{
		fluid::RealVector spreadMeanRes = fluid::RealVector(1);
		fluid::RealVector spreadStdDevRes = fluid::RealVector(1);
		fluid::RealVectorView spreadData = fluid::RealVectorView(shapeStats(fluid::Slice(1 * 7, 1)));
		fluid::RealVectorView spreadMeanOut = fluid::RealVectorView(spreadMeanRes);
		fluid::RealVectorView spreadStdDevOut = fluid::RealVectorView(spreadStdDevRes);
		mSpreadRunningStats->process(spreadData, spreadMeanOut, spreadStdDevOut);
		mDescSpread = spreadMeanOut[0];

	}
private:
	std::unique_ptr<fluid::algorithm::RunningStats> mSpreadRunningStats;
	double mDescSpread{};


};
