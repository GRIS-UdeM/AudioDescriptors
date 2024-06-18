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

Parameters::Parameters(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState)
	: mAPVTS(audioProcessorValueTreeState) 
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
		observer->addNewParamValue(lastRes);
	}
}

double Parameters::getDiffValue()
{
	auto diff = lastRes - res;
	lastRes = res;
	//DBG("valeur finale = " << diff);
	notifyObservers();
	return diff;
}

double Parameters::getValue()
{
	return lastRes;
}