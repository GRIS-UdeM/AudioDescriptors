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

#include "Parameters.h"
#include "PanelView.h"

Parameters::Parameters(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState, ParameterFunctions& functions)
	: mAPVTS(audioProcessorValueTreeState)
	, mFunctions(functions)
{
}

juce::String const& Parameters::getParameterName() const
{
	return parameterName;
}

void Parameters::addObserver(PanelView* observer)
{
	mObservers.push_back(observer);
}

void Parameters::removeObserver(PanelView* observer)
{
	auto it = std::find(mObservers.begin(), mObservers.end(), observer);
	if (it != mObservers.end()) {
		mObservers.erase(it);
	}
}

void Parameters::notifyObservers()
{
	for (auto observer : mObservers) {
		observer->addNewParamValueToDataGraph(lastRes);
	}
}

double Parameters::getDiffValue()
{
	auto diff = lastRes - res;
	lastRes = res;
	notifyObservers();
	return diff;
}

double Parameters::getValue()
{
	return lastRes;
}

double Parameters::processLoudness(double valueToProcess)
{
	valueToProcess = mFunctions.PourcentageConversion(valueToProcess, paramFactorLoudness);
	valueToProcess = processSmoothedLoudness(valueToProcess);
	return valueToProcess;
}

double Parameters::processPitch(double valueToProcess)
{
	auto res{ 0.0 };
	double minFreq = mFunctions.frequencyToMidiNoteNumber(paramMinFreqPitch);
	double maxFreq = mFunctions.frequencyToMidiNoteNumber(paramMaxFreqPitch);
	double zmap = mFunctions.zmap(valueToProcess, minFreq, maxFreq);
	res = processSmoothedPitch(zmap);
	return res;
}

double Parameters::processCentroid(double valueToProcess)
{
	auto res{ 0.0 };
	double minFreq = mFunctions.frequencyToMidiNoteNumber(paramMinFreqCentroid);
	double maxFreq = mFunctions.frequencyToMidiNoteNumber(paramMaxFreqCentroid);
	double zmap = mFunctions.zmap(valueToProcess, minFreq, maxFreq);
	res = processSmoothedCentroid(zmap);
	return res;
}

double Parameters::processSpread(double valueToProcess)
{
	auto res{ 0.0 };
	double ScaleOne = paramFactorSpread;
	ScaleOne = mFunctions.zmap(ScaleOne, 100.0, 500.0);
	ScaleOne = mFunctions.subtractFromOne(ScaleOne);
	double power = mFunctions.calculatePower(valueToProcess, ScaleOne);
	double mExpr = mFunctions.expr(power);
	double ScaleTwo = paramFactorSpread;
	ScaleTwo = mFunctions.ClipMyValue(ScaleTwo);
	double valueToSmooth = mFunctions.valueToSmooth(mExpr, ScaleTwo);
	res = processSmoothedSpread(valueToSmooth);
	return res;
}

double Parameters::processNoise(double valueToProcess)
{
	valueToProcess = valueToProcess * (paramFactorNoise * 0.01);
	valueToProcess = processSmoothedNoise(valueToProcess);
	return valueToProcess;
}

double Parameters::processSmoothedLoudness(double targetValue) {
	return mSmoothLoudness.doSmoothing(targetValue, paramSmoothLoudness, paramSmoothCoefLoudness);
}

double Parameters::processSmoothedPitch(double targetValue) {
	return mSmoothPitch.doSmoothing(targetValue, paramSmoothPitch, paramSmoothCoefPitch);
}

double Parameters::processSmoothedCentroid(double targetValue) {
	return mSmoothCentroid.doSmoothing(targetValue, paramSmoothCentroid, paramSmoothCoefCentroid);
}

double Parameters::processSmoothedSpread(double targetValue) {
	return mSmoothSpread.doSmoothing(targetValue, paramSmoothSpread, paramSmoothCoefSpread);
}

double Parameters::processSmoothedNoise(double targetValue) {
	return mSmoothNoise.doSmoothing(targetValue, paramSmoothNoise, paramSmoothCoefNoise);
}

double Parameters::processSmoothedOnsetDetection(double targetValue) {
	return mSmoothOnsetDetection.doSmoothing(targetValue, paramSmoothOD, paramSmoothCoefOD);
}
