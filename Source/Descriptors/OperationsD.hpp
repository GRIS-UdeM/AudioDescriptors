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

#include "Descriptors.hpp"
#include <cmath>

class OperationsD : public Descriptors
{
public:
	fluid::RealVectorView calculateWindow(fluid::RealVector& padded, int& i) {
		return padded(fluid::Slice(i * mHopSize, mWindowSize));
	}

	fluid::RealVector calculatePadded(fluid::RealVector in) {
		return in.size() + mWindowSize + mHopSize;
	}

	fluid::index calculateFrames(fluid::RealVector padded) {
		return static_cast<fluid::index>(floor((padded.size() - mWindowSize) / mHopSize));
	}

	fluid::Slice paddedValue(fluid::RealVector in) {
		return fluid::Slice(mHalfWindow, in.size());
	}

	void setFrame(fluid::ComplexVector& frame) {
		frame.resize(mNBins);
	}

	void setMagnitude(fluid::RealVector& magnitude) {
		magnitude.resize(mNBins);
	}

	void setMels(fluid::RealVector& mels) {
		mels.resize(mNBands);
	}

	//FONCTION POUR LA LOUDNESS

	fluid::RealVectorView calculateWindowLoudness(fluid::RealVector& padded, int& i) {
		return padded(fluid::Slice(i * mHopSizeLoudness, mWindowSizeLoudness));
	}

	fluid::RealVector calculatePaddedLoudness(fluid::RealVector in) {
		return in.size() + mWindowSizeLoudness + mHopSizeLoudness;
	}

	fluid::index calculateFramesLoudness(fluid::RealVector padded) {
		return static_cast<fluid::index>(floor((padded.size() - mWindowSizeLoudness) / mHopSizeLoudness));
	}

	fluid::Slice paddedValueLoudness(fluid::RealVector in) {
		return fluid::Slice(mHalfWindowLoudness, in.size());
	}


	//FONCTION POUR LE PITCH

	fluid::RealVectorView calculateWindowPitch(fluid::RealVector& padded, int& i) {
		return padded(fluid::Slice(i * mHopSizePitch, mWindowSizePitch));
	}

	fluid::RealVector calculatePaddedPitch(fluid::RealVector in) {
		return in.size() + mWindowSizePitch + mHopSizePitch;
	}

	fluid::index calculateFramesPitch(fluid::RealVector padded) {
		return static_cast<fluid::index>(floor((padded.size() - mWindowSizePitch) / mHopSizePitch));
	}

	fluid::Slice paddedValuePitch(fluid::RealVector in) {
		return fluid::Slice(mHalfWindowPitch, in.size());
	}

	void setFramePitch(fluid::ComplexVector& frame) {
		frame.resize(mNBinsPitch);
	}

	void setMagnitudePitch(fluid::RealVector& magnitude) {
		magnitude.resize(mNBinsPitch);
	}

	void setMelsPitch(fluid::RealVector& mels) {
		mels.resize(mNBandsPitch);
	}


	//FONCTION POUR Spectral

	fluid::RealVectorView calculateWindowSpectral(fluid::RealVector& padded, int& i) {
		return padded(fluid::Slice(i * mHopSizeSpectral, mWindowSizeSpectral));
	}

	fluid::RealVector calculatePaddedSpectral(fluid::RealVector in) {
		return in.size() + mWindowSizeSpectral + mHopSizeSpectral;
	}

	fluid::index calculateFramesSpectral(fluid::RealVector padded) {
		return static_cast<fluid::index>(floor((padded.size() - mWindowSizeSpectral) / mHopSizeSpectral));
	}

	fluid::Slice paddedValueSpectral(fluid::RealVector in) {
		return fluid::Slice(mHalfWindowSpectral, in.size());
	}

	void setFrameSpectral(fluid::ComplexVector& frame) {
		frame.resize(mNBinsSpectral);
	}

	void setMagnitudeSpectral(fluid::RealVector& magnitude) {
		magnitude.resize(mNBinsSpectral);
	}

	void setMelsSpectral(fluid::RealVector& mels) {
		mels.resize(mNBandsSpectral);
	}

};
