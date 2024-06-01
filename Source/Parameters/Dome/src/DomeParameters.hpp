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

class DomeParameters
{
public:


	double DbToGain(const double& value) {
		double linearLoudness = juce::Decibels::decibelsToGain(value);
		return linearLoudness;
	}

	double PourcentageConversion(const double& value, const double& factor) {
		return value * (factor * 0.01);
	}

	double frequencyToMidiNoteNumber(double frequency) {
		if (frequency <= 0.0) {
			return -1;
		}
		double midiNoteNumber = 69.0 + 12.0 * std::log2(frequency / 440.0);

		// Arrondir au nombre MIDI le plus proche
		return std::round(midiNoteNumber);
	}

	double zmap(double value, double inputMin, double inputMax) {
		// Assurez-vous que la valeur est comprise dans la plage d'entrée
		if (value < inputMin) {
			value = inputMin;
		}
		else if (value > inputMax) {
			value = inputMax;
		}

		// Mappez la valeur d'entrée vers la plage de sortie [-1, 1]
		// Formule : (valeur - minIn) / (maxIn - minIn) * (maxOut - minOut) + minOut
		return (value - inputMin) / (inputMax - inputMin) * (1.0 - 0.0) + 0.0;
	}


	double subtractFromOne(double input) {
		return 1.0 - input;
	}


	double calculatePower(double base, double exponent) {
		return pow(base, exponent);
	}


	double expr(double f1) {
		if (f1 < 0.5) {
			return 4.0 * pow(f1, 3);
		}
		else {
			return 0.5 * pow((2.0 * f1 - 2.0), 3) + 1.0;
		}
	}

	double ClipMyValue(double value) {

		double clipedValue = juce::jlimit(0.0, 100.0, value);
		return clipedValue * 0.01;
	}

	double valueToSmooth(double valueOne, double clipedValue) {
		return valueOne * clipedValue;
	}

	double power(double input) {
		return pow(input, 0.4);
	}
};
