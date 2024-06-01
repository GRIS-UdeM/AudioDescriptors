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

#include "algorithms/public/MelBands.hpp"
#include "Descriptors.hpp"


class BandsD : public Descriptors
{
public:
	void resetBands() {
		mBands.reset(new fluid::algorithm::MelBands{ mNBandsSpectral, mFftSizeSpectral });
	}

	void initBands(double mSampleRate) {
		mBands->init(static_cast<double>(mMinFreqSpectral), static_cast<double>(mMaxFreqSpectral), mNBandsSpectral, mNBinsSpectral, mSampleRate, mWindowSizeSpectral);
	}

	void bandeProcess(fluid::RealVector& magnitude, fluid::RealVector& mels) {
		mBands->processFrame(magnitude, mels, false, false, true, fluid::FluidDefaultAllocator());
	}

private:
	std::unique_ptr<fluid::algorithm::MelBands>	mBands;
};
