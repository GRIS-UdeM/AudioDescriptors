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

#if JUCE_LINUX
#include "../PCH.h"
#endif

#include <JuceHeader.h>
#include "algorithms/public/MultiStats.hpp"

enum class DescriptorID { invalid = -1, loudness = 0, pitch, centroid, spread, noise, iterationsSpeed };

class Descriptors {
public:
	Descriptors() = default;
	virtual ~Descriptors() = default;

	virtual void init() = 0;
	virtual void reset() = 0;
	virtual double getValue() = 0;

	virtual DescriptorID getID() {
		return mID;
	}

	static int toInt(DescriptorID descID) {
		return static_cast<int>(descID) + 2;
	}

	static DescriptorID fromInt(int value) {
		switch (value)
		{
		case 2: return DescriptorID::loudness;
		case 3: return DescriptorID::pitch;
		case 4: return DescriptorID::centroid;
		case 5: return DescriptorID::spread;
		case 6: return DescriptorID::noise;
		case 7: return DescriptorID::iterationsSpeed;
		default:
			return DescriptorID::invalid;
		}
	}

	fluid::RealVector computeStats(fluid::RealMatrixView matrix, fluid::algorithm::MultiStats stats) {
		fluid::index dim = matrix.cols();
		fluid::RealMatrix tmp(dim, 7);
		fluid::RealVector result(dim * 7);
		stats.process(matrix.transpose(), tmp);

		for (int j = 0; j < dim; j++)
		{
			result(fluid::Slice(j * 7, 7)) <<= tmp.row(j);
		}

		return result;
	}

protected:
	DescriptorID mID{ DescriptorID::invalid };
	int mRunningStatsHistory = 1;

private:
	//==============================================================================
	JUCE_LEAK_DETECTOR(Descriptors)
};
