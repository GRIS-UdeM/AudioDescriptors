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

#include <cmath>
#include "../../SpatialParameter.h"

class ElevationDome : public SpatialParameter
{
public:
	ElevationDome(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState, ParameterFunctions& functions)
		: SpatialParameter(audioProcessorValueTreeState, functions)
	{
		parameterName = juce::String("Elevation");
		paramID = ParameterID::elevation;
		setParametersState();
	}

	void process(const DescriptorID& descID, double valueToProcess) override {
		auto range{ 0.0 };
		auto offset{ 1.0 };
		auto smooth{ 0.0 };

		switch (descID)
		{
		case DescriptorID::loudness:
			range = paramRangeLoudness;
			offset = paramOffsetLoudness;
			smooth = processLoudness(valueToProcess);
			break;
		case DescriptorID::pitch:
			range = paramRangePitch;
			offset = paramOffsetPitch;
			smooth = processPitch(valueToProcess);
			break;
		case DescriptorID::centroid:
			range = paramRangeCentroid;
			offset = paramOffsetCentroid;
			smooth = processCentroid(valueToProcess);
			break;
		case DescriptorID::spread:
			range = paramRangeSpread;
			offset = paramOffsetSpread;
			smooth = processSpread(valueToProcess);
			break;
		case DescriptorID::noise:
			range = paramRangeNoise;
			offset = paramOffsetNoise;
			smooth = processNoise(valueToProcess);
			break;
		case DescriptorID::iterationsSpeed:
			range = paramRangeOD;
			offset = paramOffsetOD;
			smooth = processSmoothedOnsetDetection(valueToProcess);
			break;
		case DescriptorID::invalid:
		default:
			break;
		}

		double clipMax = 0.999999;
		int multiplier = 90;

		double clip = juce::jlimit(0.0, clipMax, smooth);
		double inputRange = range * 0.01;
		res = clip * inputRange * multiplier;
		res -= offset;

		if (std::isnan(res)) {
			res = 0.0;
		}
	}

private:
	//==============================================================================
	JUCE_LEAK_DETECTOR(ElevationDome)
};
