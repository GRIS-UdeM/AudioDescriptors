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
#include <JuceHeader.h>
#include <cmath>
#include "../../Parameters.h"

class ZCube : public Parameters
{
public:
	ZCube(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState)
		: Parameters(audioProcessorValueTreeState)
	{
		parameterName = juce::String("Z");
		paramID = ParameterID::z;
		setParametersState();
	}

	void parameters(double range, double smooth, [[maybe_unused]] double lap, double offset) {
		double clipMax = 0.999999;

		double clip = juce::jlimit(0.0, clipMax, smooth);
		double inputRange = range * 0.01;
		res = clip * inputRange;
		res -= offset;
		//res = juce::jlimit(0.0, 1.0, res);

		if (std::isnan(res)) {
			res = 0.0;
		}
		//DBG("valeur finale = " << res);
	}

private:
	//==============================================================================
	JUCE_LEAK_DETECTOR(ZCube)
};
