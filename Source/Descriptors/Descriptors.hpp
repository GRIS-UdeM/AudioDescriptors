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
#include "algorithms/public/MultiStats.hpp"


class Descriptors {
public:
	Descriptors() = default;

	fluid::RealVector computeStats(fluid::RealMatrixView matrix, fluid::algorithm::MultiStats stats)
	{
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
	fluid::index mNBins = 513;
	fluid::index mFftSize = 2 * (mNBins - 1);
	fluid::index mHopSize = 1024;
	fluid::index mWindowSize = 1024;
	fluid::index mHalfWindow = mWindowSize / 2;
	fluid::index mNBands = 40;
	fluid::index mNCoefs = 13;
	//fluid::index mMinFreq = 20;
	//fluid::index mMaxFreq = 20000;
	int mRunningStatsHistory = 1;

	//LOUDNESS VALUE 
	fluid::index mHopSizeLoudness = 256;
	fluid::index mWindowSizeLoudness = 512;
	fluid::index mHalfWindowLoudness = mWindowSizeLoudness / 2;

	//PITCH VALUE 
	fluid::index mNBinsPitch = 513;
	fluid::index mHopSizePitch = 512;
	fluid::index mWindowSizePitch = 1024;
	fluid::index mHalfWindowPitch = mWindowSizePitch / 2;
	fluid::index mFftSizePitch = 1024;
	fluid::index mNBandsPitch = 40;
	fluid::index mMinFreqPitch = 40;
	fluid::index mMaxFreqPitch = 10000;


	//SPECTRAL SHAPE FOR CENTROID FLATNESS ET SPREAD 
	fluid::index mNBinsSpectral = 257;
	fluid::index mHopSizeSpectral = 64;
	fluid::index mWindowSizeSpectral = 256;
	fluid::index mHalfWindowSpectral = mWindowSizeSpectral / 2;
	fluid::index mFftSizeSpectral = 512;
	fluid::index mNBandsSpectral = 40;
	fluid::index mMinFreqSpectral = 20;//20 pour la
	fluid::index mMaxFreqSpectral = 20000;

	//fluid::index mMinFreqSpectralFlatness = 20;//20 pour la
	//fluid::index mMaxFreqSpectralFlatness = 20000;

private:
	//==============================================================================
	JUCE_LEAK_DETECTOR(Descriptors)
};
