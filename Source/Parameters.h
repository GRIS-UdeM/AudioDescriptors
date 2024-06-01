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

#include <JuceHeader.h>

#include "Parameters/Smooth.hpp"

enum class ParameterID { invalid = -1, azimuth = 0, elevation, x, y, z, azimuthspan, elevationspan};

class Parameters
{
public:

	Parameters() = default;
	virtual ~Parameters() = default;

	virtual void parameters(double range, double smooth, double lap = 1.0, double offset = 1.0) = 0;
	
	virtual juce::String const & getParameterName() const {
		return parameterName;
	}

	double getValue()
	{
		auto diff = lastRes - res;
		lastRes = res;
		//DBG("valeur finale = " << diff);
		return diff;
	}


	//====================================================================
	ParameterID getParameterID() const {
		return paramID;
	}

	void setParameterComboBoxIndex(int index) {
		paramComboBoxIndex = index;
	}

	int getParameterComboBoxIndex() const {
		return paramComboBoxIndex;
	}

	void setParameterMetricComboBoxIndex(int index) {
		paramMetricComboBoxIndex = index;
	}

	int getParameterMetricComboBoxIndex() const {
		return paramMetricComboBoxIndex;
	}

	void setParameterThreshold(double threshold) {
		paramThreshold = threshold;
	}

	double getParameterThreshold() const {
		return paramThreshold;
	}

	void setParameterMinTime(double minTime) {
		paramMinTime = minTime;
	}

	double getParameterMinTime() const {
		return paramMinTime;
	}

	void setParameterMaxTime(double maxTime) {
		paramMaxTime = maxTime;
	}

	double getParameterMaxTime() const {
		return paramMaxTime;
	}

	double* getParamFactorArray() {
		return paramFactorArray;
	}

	double* getParamLapArray() {
		return paramLapArray;
	}

	double* getParamOffsetArray() {
		return paramOffsetArray;
	}

	double* getParamSmoothArray() {
		return paramSmoothArray;
	}

	double* getParamMoreSmoothArray() {
		return paramMoreSmoothArray;
	}

	double* getParamRangeArray() {
		return paramRangeArray;
	}

	double* getParamMinFreqArray() {
		return paramMinFreqArray;
	}

	double* getParamMaxFreqArray() {
		return paramMaxFreqArray;
	}

	// get-set for parameters arrays (factor, smooth, range, minFreq, maxFreq)
	void setParamArrayValue(double value, int index, double* paramArray) {
		paramArray[index] = value;
	}

	double getParamArrayValue(int index, double* paramArray) const {
		return paramArray[index];
	}

	//====================================================================
	Smooth& getSmoothLoudness() {
		return smoothLoudness;
	}

	Smooth& getSmoothPitch() {
		return smoothPitch;
	}

	Smooth& getSmoothCentroid() {
		return smoothCentroid;
	}

	Smooth& getSmoothSpread() {
		return smoothSpread;
	}

	Smooth& getSmoothNoise() {
		return smoothNoise;

	}
	Smooth& getSmoothOnsetDetection() {
		return smoothOnsetDetection;
	}

protected:
	ParameterID paramID = ParameterID::invalid;
	double res{};
	double lastRes{};
	juce::String parameterName{};

	Smooth smoothLoudness;
	Smooth smoothPitch;
	Smooth smoothCentroid;
	Smooth smoothSpread;
	Smooth smoothNoise;
	Smooth smoothOnsetDetection;

	int paramComboBoxIndex = 1;
	double paramFactorArray[3] = { 100.0, 100.0, 100.0 }; // Index 0 pour loudness, index 1 pour pitch, index 2 pour centroid, index 3 spread, index 4 noise
	double paramSmoothArray[6] = { 5.0, 5.0, 5.0, 5.0, 5.0, 5.0 }; // Index 0 pour loudness, index 1 pour pitch, index 2 pour centroid, index 3 spread, index 4 noise, index 5 onsetDetection
	double paramMoreSmoothArray[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; // Index 0 pour loudness, index 1 pour pitch, index 2 pour centroid, index 3 spread, index 4 noise, index 5 onsetDetection
	double paramRangeArray[6] = { 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 };
	double paramLapArray[6] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	double paramOffsetArray[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	double paramMinFreqArray[2] = { 20.0, 20.0 };
	double paramMaxFreqArray[2] = { 10000.0, 10000.0 };
	int paramMetricComboBoxIndex = 4;
	double paramThreshold = 0.1;
	double paramMinTime = 0.1;
	double paramMaxTime = 10.0;
};
