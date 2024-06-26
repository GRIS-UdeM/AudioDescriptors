/*
 This file is part of AudioDescriptors.

 Developers: Hicheme BEN GAIED, Ga�l LANE L�PINE

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

class SkewnessD : public Descriptors
{
public:
	void resetSkewness() {
		mSkewnessRunningStats.reset(new fluid::algorithm::RunningStats());
	}

	void initSkewness() {
		mSkewnessRunningStats->init(mRunningStatsHistory, 1);
	}

	double getDescSkewness() const {
		return mDescSkewness;
	}


	void calculate(fluid::RealVector& shapeStats)
	{
		fluid::RealVector skewnessMeanRes = fluid::RealVector(1);
		fluid::RealVector skewnessStdDevRes = fluid::RealVector(1);
		fluid::RealVectorView skewnessData = fluid::RealVectorView(shapeStats(fluid::Slice(2 * 7, 1)));
		fluid::RealVectorView skewnessMeanOut = fluid::RealVectorView(skewnessMeanRes);
		fluid::RealVectorView skewnessStdDevOut = fluid::RealVectorView(skewnessStdDevRes);
		mSkewnessRunningStats->process(skewnessData, skewnessMeanOut, skewnessStdDevOut);
		mDescSkewness = skewnessMeanOut[0];
	}

private:
	std::unique_ptr<fluid::algorithm::RunningStats> mSkewnessRunningStats;
	double mDescSkewness{};

	//==============================================================================
	JUCE_LEAK_DETECTOR(SkewnessD)
};
