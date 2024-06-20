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
#include "algorithms/public/STFT.hpp"
#include "Descriptors.hpp"


class StftD : public Descriptors
{
public:
	void resetStft() {
		mStft.reset(new fluid::algorithm::STFT{ mWindowSize, mFftSize, mHopSize });
	}

	void resetStftPitch() {
		mStft.reset(new fluid::algorithm::STFT{ mWindowSizePitch, mFftSizePitch, mHopSizePitch});
	}

	void resetStftSpectral() {
		mStft.reset(new fluid::algorithm::STFT{ mWindowSizeSpectral, mFftSizeSpectral, mHopSizeSpectral });
	}

	//Second agrument c'est le out
	void stftProcess(fluid::RealVectorView& window, fluid::ComplexVector& frame) {
		mStft->processFrame(window, frame);
	}

	//Second agrument c'est le out
	void stftMagntiude(fluid::ComplexVector& frame, fluid::RealVector& magnitude) {
		mStft->magnitude(frame, magnitude);
	}

private:
	std::unique_ptr<fluid::algorithm::STFT>	mStft;

	//==============================================================================
	JUCE_LEAK_DETECTOR(StftD)
};
