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
#include "algorithms/public/SpectralShape.hpp"
#include "algorithms/public/STFT.hpp"
#include "Descriptors.hpp"


class ShapeD : public Descriptors
{
public:
	void reset() override {
		mShape.reset(new fluid::algorithm::SpectralShape(fluid::FluidDefaultAllocator()));
		mStft.reset(new fluid::algorithm::STFT{ mWindowSizeSpectral, mFftSizeSpectral, mHopSizeSpectral });
	}

	void mShapeProcess(fluid::RealVector& magnitude, fluid::RealVector& shapeDesc, double mSampleRate) {
		mShape->processFrame(magnitude, shapeDesc, mSampleRate, static_cast<double>(mMinFreqSpectral), static_cast<double>(mMaxFreqSpectral), 0.95, true, true, fluid::FluidDefaultAllocator());
	}

	fluid::algorithm::SpectralShape* getShape() const{
		return mShape.get();
	}

	fluid::RealVector shapeCalculate(fluid::RealMatrix& matrix, fluid::algorithm::MultiStats& stats) {
		return computeStats(matrix, stats);
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

	//==============================================================================
	// Stft stuff
	// Second argument is output
	void stftProcess(fluid::RealVectorView& window, fluid::ComplexVector& frame) {
		mStft->processFrame(window, frame);
	}

	// Second argument is output
	void stftMagntiude(fluid::ComplexVector& frame, fluid::RealVector& magnitude) {
		mStft->magnitude(frame, magnitude);
	}

private:
	void init() override {}
	double getValue() override { return 0; }

	std::unique_ptr<fluid::algorithm::SpectralShape>  mShape;
	std::unique_ptr<fluid::algorithm::STFT>	mStft;

	// SPECTRAL SHAPE FOR CENTROID, FLATNESS AND SPREAD 
	fluid::index mNBinsSpectral = 257;
	fluid::index mHopSizeSpectral = 64;
	fluid::index mWindowSizeSpectral = 256;
	fluid::index mHalfWindowSpectral = mWindowSizeSpectral / 2;
	fluid::index mFftSizeSpectral = 512;
	fluid::index mNBandsSpectral = 40;
	fluid::index mMinFreqSpectral = 20;//20 pour la
	fluid::index mMaxFreqSpectral = 20000;

	//==============================================================================
	JUCE_LEAK_DETECTOR(ShapeD)
};
