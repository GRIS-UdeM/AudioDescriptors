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

class KurtosisD : public Descriptors
{
public:
	void resetKurtosis() {
		mKurtosisRunningStats.reset(new fluid::algorithm::RunningStats());

	}

	void initKurtosis() {
		mKurtosisRunningStats->init(mRunningStatsHistory, 1);
	}

	double getDescKurtosis() const {
		return mDescKurtosis;
	}

	void calculate(fluid::RealVector& shapeStats)
	{
		fluid::RealVector kurtosisMeanRes = fluid::RealVector(1);
		fluid::RealVector kurtosisStdDevRes = fluid::RealVector(1);
		fluid::RealVectorView kurtosisData = fluid::RealVectorView(shapeStats(fluid::Slice(3 * 7, 1)));
		fluid::RealVectorView kurtosisMeanOut = fluid::RealVectorView(kurtosisMeanRes);
		fluid::RealVectorView kurtosisStdDevOut = fluid::RealVectorView(kurtosisStdDevRes);
		mKurtosisRunningStats->process(kurtosisData, kurtosisMeanOut, kurtosisStdDevOut);
		mDescKurtosis = kurtosisMeanOut[0];
	}
private:
	std::unique_ptr<fluid::algorithm::RunningStats> mKurtosisRunningStats;
	double mDescKurtosis{};
};
