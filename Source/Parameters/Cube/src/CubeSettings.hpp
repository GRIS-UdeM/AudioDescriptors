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
#include "../../Parameters/Cube/src/XCube.hpp"
#include "../../Parameters/Cube/src/YCube.hpp"
#include "../../Parameters/Cube/src/ZCube.hpp"
#include "../../Parameters/Cube/src/HspanCube.hpp"
#include "../../Parameters/Cube/src/VspanCube.hpp"
#include "../../Parameters/Smooth.hpp"
#include "../../Constants.h"

class CubeSettings
{
public:
/// /////////////////////////// Les conditions de verifications//////////////////////////

    bool checkConditionNeedSpectralAnalyse() {
        if (mXCube.getParameterComboBoxIndex() > 3 || mYCube.getParameterComboBoxIndex() > 3 || mZCube.getParameterComboBoxIndex() > 3 || mHSpanCube.getParameterComboBoxIndex() > 3 || mVSpanCube.getParameterComboBoxIndex() > 3) {
            if ((mXCube.getParameterComboBoxIndex() > 3 && mXCube.getParameterComboBoxIndex() < 7) &&
                (
                (mXCube.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mXCube.getParamMaxFreqArray()[indexMaxFreqCentroid] > mXCube.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mXCube.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mXCube.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mXCube.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mXCube.getParamFactorArray()[indexFactorNoise] > 0)
                )
                ) {
                return true;
            }

            if ((mYCube.getParameterComboBoxIndex() > 3 && mYCube.getParameterComboBoxIndex() < 7) &&
                (
                (mYCube.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mYCube.getParamMaxFreqArray()[indexMaxFreqCentroid] > mYCube.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mYCube.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mYCube.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mYCube.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mYCube.getParamFactorArray()[indexFactorNoise] > 0)
                )
                ) {
                return true;
            }

            if ((mZCube.getParameterComboBoxIndex() > 3 && mZCube.getParameterComboBoxIndex() < 7) &&
                (
                (mZCube.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mZCube.getParamMaxFreqArray()[indexMaxFreqCentroid] > mZCube.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mZCube.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mZCube.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mZCube.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mZCube.getParamFactorArray()[indexFactorNoise] > 0)
                )
                ) {
                return true;
            }

            if ((mHSpanCube.getParameterComboBoxIndex() > 3 && mHSpanCube.getParameterComboBoxIndex() < 7) &&
                (
                (mHSpanCube.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mHSpanCube.getParamMaxFreqArray()[indexMaxFreqCentroid] > mHSpanCube.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mHSpanCube.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mHSpanCube.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mHSpanCube.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mHSpanCube.getParamFactorArray()[indexFactorNoise] > 0)
                )
                ) {
                return true;
            }

            if ((mVSpanCube.getParameterComboBoxIndex() > 3 && mVSpanCube.getParameterComboBoxIndex() < 7) &&
                (
                (mVSpanCube.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mVSpanCube.getParamMaxFreqArray()[indexMaxFreqCentroid] > mVSpanCube.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mVSpanCube.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mVSpanCube.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mVSpanCube.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mVSpanCube.getParamFactorArray()[indexFactorNoise] > 0)
                )
                ) {
                return true;
            }
        }
        return false;
    }
    bool checkConditionLoudnessX() {
        if (mXCube.getParameterComboBoxIndex() == 2 && mXCube.getParamArrayValue(indexFactorLoudness, mXCube.getParamFactorArray()) > 0 && mXCube.getParamArrayValue(indexSmoothAndRangeLoudness, mXCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessY() {
        if (mYCube.getParameterComboBoxIndex() == 2 && mYCube.getParamArrayValue(indexFactorLoudness, mYCube.getParamFactorArray()) > 0 && mYCube.getParamArrayValue(indexSmoothAndRangeLoudness, mYCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessZ() {
        if (mZCube.getParameterComboBoxIndex() == 2 && mZCube.getParamArrayValue(indexFactorLoudness, mZCube.getParamFactorArray()) > 0 && mZCube.getParamArrayValue(indexSmoothAndRangeLoudness, mZCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessHSpan() {
        if (mHSpanCube.getParameterComboBoxIndex() == 2 && mHSpanCube.getParamArrayValue(indexFactorLoudness, mHSpanCube.getParamFactorArray()) > 0 && mHSpanCube.getParamArrayValue(indexSmoothAndRangeLoudness, mHSpanCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessVSpan() {
        if (mVSpanCube.getParameterComboBoxIndex() == 2 && mVSpanCube.getParamArrayValue(indexFactorLoudness, mVSpanCube.getParamFactorArray()) > 0 && mVSpanCube.getParamArrayValue(indexSmoothAndRangeLoudness, mVSpanCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionForLoudnessAnalyse() {
        if ((checkConditionLoudnessX()) ||
            (checkConditionLoudnessY()) ||
            (checkConditionLoudnessHSpan()) ||
            (checkConditionLoudnessVSpan()) ||
            (checkConditionLoudnessZ())
            ) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionAnalyse() {
        if (mXCube.getParameterComboBoxIndex() == 7 || mYCube.getParameterComboBoxIndex() == 7 || mZCube.getParameterComboBoxIndex() == 7 || mHSpanCube.getParameterComboBoxIndex() == 7 || mVSpanCube.getParameterComboBoxIndex() == 7) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionX() {
        if (mXCube.getParameterComboBoxIndex() == 7 && mXCube.getParameterMinTime() < mXCube.getParameterMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionY() {
        if (mYCube.getParameterComboBoxIndex() == 7 && mYCube.getParameterMinTime() < mYCube.getParameterMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionZ() {
        if (mZCube.getParameterComboBoxIndex() == 7 && mZCube.getParameterMinTime() < mZCube.getParameterMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionHSpan() {
        if (mHSpanCube.getParameterComboBoxIndex() == 7 && mHSpanCube.getParameterMinTime() < mHSpanCube.getParameterMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionVSpan() {
        if (mVSpanCube.getParameterComboBoxIndex() == 7 && mVSpanCube.getParameterMinTime() < mVSpanCube.getParameterMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionPitchX() {
        if (mXCube.getParameterComboBoxIndex() == 3 &&
            mXCube.getParamArrayValue(indexMinFreqPitch, mXCube.getParamMinFreqArray()) < mXCube.getParamArrayValue(indexMaxFreqPitch, mXCube.getParamMaxFreqArray()) &&
            mXCube.getParamArrayValue(indexSmoothAndRangePitch, mXCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchY() {
        if (mYCube.getParameterComboBoxIndex() == 3 &&
            mYCube.getParamArrayValue(indexMinFreqPitch, mYCube.getParamMinFreqArray()) < mYCube.getParamArrayValue(indexMaxFreqPitch, mYCube.getParamMaxFreqArray()) &&
            mYCube.getParamArrayValue(indexSmoothAndRangePitch, mYCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchZ() {
        if (mZCube.getParameterComboBoxIndex() == 3 &&
            mZCube.getParamArrayValue(indexMinFreqPitch, mZCube.getParamMinFreqArray()) < mZCube.getParamArrayValue(indexMaxFreqPitch, mZCube.getParamMaxFreqArray()) &&
            mZCube.getParamArrayValue(indexSmoothAndRangePitch, mZCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchHSpan() {
        if (mHSpanCube.getParameterComboBoxIndex() == 3 &&
            mHSpanCube.getParamArrayValue(indexMinFreqPitch, mHSpanCube.getParamMinFreqArray()) < mHSpanCube.getParamArrayValue(indexMaxFreqPitch, mHSpanCube.getParamMaxFreqArray()) &&
            mHSpanCube.getParamArrayValue(indexSmoothAndRangePitch, mHSpanCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchVSpan() {
        if (mVSpanCube.getParameterComboBoxIndex() == 3 &&
            mVSpanCube.getParamArrayValue(indexMinFreqPitch, mVSpanCube.getParamMinFreqArray()) < mVSpanCube.getParamArrayValue(indexMaxFreqPitch, mVSpanCube.getParamMaxFreqArray()) &&
            mVSpanCube.getParamArrayValue(indexSmoothAndRangePitch, mVSpanCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionForPitchAnalyse() {
        if (checkConditionPitchX() || checkConditionPitchY() || checkConditionPitchZ() || checkConditionPitchHSpan() || checkConditionPitchVSpan()) {
            return true;
        }
        return false;
    }

    bool checkConditionNoiseX() {
        if (mXCube.getParameterComboBoxIndex() == 6 && mXCube.getParamArrayValue(indexFactorNoise, mXCube.getParamFactorArray()) > 0 && mXCube.getParamArrayValue(indexSmoothAndRangeNoise, mXCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseY() {
        if (mYCube.getParameterComboBoxIndex() == 6 && mYCube.getParamArrayValue(indexFactorNoise, mYCube.getParamFactorArray()) > 0 && mYCube.getParamArrayValue(indexSmoothAndRangeNoise, mYCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseZ() {
        if (mZCube.getParameterComboBoxIndex() == 6 && mZCube.getParamArrayValue(indexFactorNoise, mZCube.getParamFactorArray()) > 0 && mZCube.getParamArrayValue(indexSmoothAndRangeNoise, mZCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseHSpan() {
        if (mHSpanCube.getParameterComboBoxIndex() == 6 && mHSpanCube.getParamArrayValue(indexFactorNoise, mHSpanCube.getParamFactorArray()) > 0 && mHSpanCube.getParamArrayValue(indexSmoothAndRangeNoise, mHSpanCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseVSpan() {
        if (mVSpanCube.getParameterComboBoxIndex() == 6 && mVSpanCube.getParamArrayValue(indexFactorNoise, mVSpanCube.getParamFactorArray()) > 0 && mVSpanCube.getParamArrayValue(indexSmoothAndRangeNoise, mVSpanCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionForNoiseAnalyse() {
        if (checkConditionNoiseX() ||
            checkConditionNoiseY() ||
            checkConditionNoiseZ() ||
            checkConditionNoiseHSpan() ||
            checkConditionNoiseVSpan()
            ) {
            return true;
        }
        return false;
    }

    bool checkConditionSpreadX() {
        if (mXCube.getParameterComboBoxIndex() == 5 && mXCube.getParamArrayValue(indexFactorSpread, mXCube.getParamFactorArray()) > 0 && mXCube.getParamArrayValue(indexSmoothAndRangeSpread, mXCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadY() {
        if (mYCube.getParameterComboBoxIndex() == 5 && mYCube.getParamArrayValue(indexFactorSpread, mYCube.getParamFactorArray()) > 0 && mYCube.getParamArrayValue(indexSmoothAndRangeSpread, mYCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadZ() {
        if (mZCube.getParameterComboBoxIndex() == 5 && mZCube.getParamArrayValue(indexFactorSpread, mZCube.getParamFactorArray()) > 0 && mZCube.getParamArrayValue(indexSmoothAndRangeSpread, mZCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadHSpan() {
        if (mHSpanCube.getParameterComboBoxIndex() == 5 && mHSpanCube.getParamArrayValue(indexFactorSpread, mHSpanCube.getParamFactorArray()) > 0 && mHSpanCube.getParamArrayValue(indexSmoothAndRangeSpread, mHSpanCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadVSpan() {
        if (mVSpanCube.getParameterComboBoxIndex() == 5 && mVSpanCube.getParamArrayValue(indexFactorSpread, mVSpanCube.getParamFactorArray()) > 0 && mVSpanCube.getParamArrayValue(indexSmoothAndRangeSpread, mVSpanCube.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionForSpreadAnalyse() {
        if (checkConditionSpreadX() ||
            checkConditionSpreadY() ||
            checkConditionSpreadZ() ||
            checkConditionSpreadHSpan() ||
            checkConditionSpreadVSpan()
            ) {
            return true;
        }
        return false;
    }

    bool checkConditionCentroidX() {
        if (mXCube.getParameterComboBoxIndex() == 4 &&
            mXCube.getParamArrayValue(indexMinFreqCentroid, mXCube.getParamMinFreqArray()) < mXCube.getParamArrayValue(indexMaxFreqCentroid, mXCube.getParamMaxFreqArray()) &&
            mXCube.getParamArrayValue(indexSmoothAndRangeCentroid, mXCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidY() {
        if (mYCube.getParameterComboBoxIndex() == 4 &&
            mYCube.getParamArrayValue(indexMinFreqCentroid, mYCube.getParamMinFreqArray()) < mYCube.getParamArrayValue(indexMaxFreqCentroid, mYCube.getParamMaxFreqArray()) &&
            mYCube.getParamArrayValue(indexSmoothAndRangeCentroid, mYCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidZ() {
        if (mZCube.getParameterComboBoxIndex() == 4 &&
            mZCube.getParamArrayValue(indexMinFreqCentroid, mZCube.getParamMinFreqArray()) < mZCube.getParamArrayValue(indexMaxFreqCentroid, mZCube.getParamMaxFreqArray()) &&
            mZCube.getParamArrayValue(indexSmoothAndRangeCentroid, mZCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidHSpan() {
        if (mHSpanCube.getParameterComboBoxIndex() == 4 &&
            mHSpanCube.getParamArrayValue(indexMinFreqCentroid, mHSpanCube.getParamMinFreqArray()) < mHSpanCube.getParamArrayValue(indexMaxFreqCentroid, mHSpanCube.getParamMaxFreqArray()) &&
            mHSpanCube.getParamArrayValue(indexSmoothAndRangeCentroid, mHSpanCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidVSpan() {
        if (mVSpanCube.getParameterComboBoxIndex() == 4 &&
            mVSpanCube.getParamArrayValue(indexMinFreqCentroid, mVSpanCube.getParamMinFreqArray()) < mVSpanCube.getParamArrayValue(indexMaxFreqCentroid, mVSpanCube.getParamMaxFreqArray()) &&
            mVSpanCube.getParamArrayValue(indexSmoothAndRangeCentroid, mVSpanCube.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionForCentroidAnalyse() {
        if (checkConditionCentroidX() || checkConditionCentroidY() || checkConditionCentroidZ() || checkConditionCentroidHSpan() || checkConditionCentroidVSpan()) {
            return true;
        }
        return false;
    }

    XCube& getXCube() {
        return mXCube;
    }

    ZCube& getZCube() {
        return mZCube;
    }

    YCube& getYCube() {
        return mYCube;
    }

    HspanCube& getHSpanCube() {
        return mHSpanCube;
    }

    VspanCube& getVSpanCube() {
        return mVSpanCube;
    }

private:

    XCube mXCube;
    YCube mYCube;
    ZCube mZCube;
    HspanCube mHSpanCube;
    VspanCube mVSpanCube;
};