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
#include "Descriptors.hpp"


class ShapeD : public Descriptors
{
public:

	void resetShape() {
		mShape.reset(new fluid::algorithm::SpectralShape(fluid::FluidDefaultAllocator()));
	}

	void mShapeProcess(fluid::RealVector& magnitude, fluid::RealVector& shapeDesc, double mSampleRate) {
		mShape->processFrame(magnitude, shapeDesc, mSampleRate, static_cast<double>(mMinFreqSpectral), static_cast<double>(mMaxFreqSpectral), 0.95, true, true, fluid::FluidDefaultAllocator());
	}

	//void mShapeProcessFlatness(fluid::RealVector& magnitude, fluid::RealVector& shapeDesc, double mSampleRate) {
	//	mShape->processFrame(magnitude, shapeDesc, mSampleRate, mMinFreqSpectralFlatness, mMaxFreqSpectralFlatness, 0.95, true, true, fluid::FluidDefaultAllocator());
	//}

	fluid::algorithm::SpectralShape* getShape() const{
		return mShape.get();
	}

	fluid::RealVector shapeCalculate(fluid::RealMatrix& matrix, fluid::algorithm::MultiStats& stats) {
		return computeStats(matrix, stats);
	}
private:
	std::unique_ptr<fluid::algorithm::SpectralShape>  mShape;

};
