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
#include "../../Parameters.h"

class ElevationDome : public Parameters
{
public:
	ElevationDome(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState)
		: Parameters(audioProcessorValueTreeState)
	{
		parameterName = juce::String("Elevation");
		paramID = ParameterID::elevation;
		setParametersState();
	}

	void parameters(double range, double smooth, [[maybe_unused]] double lap, double offset) {
		double clipMax = 0.999999;
		int multiplier = 90;

		//offset *= 0.01;
		//offset = 1 - offset;
		//if (offset == 0) {
		//	offset = ALMOST_ZERO;
		//}

		double clip = juce::jlimit(0.0, clipMax, smooth);
		double inputRange = range * 0.01;
		res = clip * inputRange * multiplier;
		res -= offset;

		if (std::isnan(res)) {
			res = 0.0;
		}
		//DBG("valeur finale = " << res);
	}
};
