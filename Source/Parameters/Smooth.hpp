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
#include "../Constants.h"

class Smooth
{
public:

	double DoSmooth(double targetValue, double smooth, double smoothCoef) {
		smooth = juce::jmap(smooth, 0.0, 100.0, 0.0, 200.0);
		smooth = std::max(1.0, std::min(smooth, 200.0));

		if (this->startHistory || this->smoothHistory != smooth || smooth <= 1) {
			currentValue = targetValue;
			this->startHistory = false;
			this->smoothHistory = smooth;
		}
		else {
			smoothCoef *= 0.01;
			smoothCoef = 1 - smoothCoef;
			if (smoothCoef == 0) {
				smoothCoef = ALMOST_ZERO;
			}
			double logsmooth = std::log(smooth);
			double normalizedsmooth = 1.0 / logsmooth;
			double adjustment = (targetValue - currentValue) * (normalizedsmooth*smoothCoef);



			currentValue += adjustment;
		}
		//DBG("Valeur smooth = " << currentValue);
		return currentValue;
	}



	double DbToGain(const double& valueLoudness, const double& factor) {
		double linearLoudness = juce::Decibels::decibelsToGain(valueLoudness);
		linearLoudness *= (factor * 0.01);
		return linearLoudness;
	}

private:
	double currentValue = -1;
	double smoothHistory = -1;
	bool startHistory = true;
	double valueSmoothHistory = 0;

	//==============================================================================
	JUCE_LEAK_DETECTOR(Smooth)
};
