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

#include "../../Parameters/Dome/src/AzimuthDome.hpp"
#include "../../Parameters/Dome/src/ElevationDome.hpp"
#include "../../Parameters/Dome/src/HspanDome.hpp"
#include "../../Parameters/Dome/src/VspanDome.hpp"
#include "../../Parameters/Smooth.hpp"
#include "../../Constants.h"

class DomeSettings
{
public:

    bool checkConditionNeedSpectralAnalyse() {
        if (mAzimuthDome.getParameterComboBoxIndex() > 3 || mElevationDome.getParameterComboBoxIndex() > 3 || mHSpanDome.getParameterComboBoxIndex() > 3 || mVSpanDome.getParameterComboBoxIndex() > 3) {
            if ((mAzimuthDome.getParameterComboBoxIndex() > 3 && mAzimuthDome.getParameterComboBoxIndex() < 7) &&
                (
                ((mAzimuthDome.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mAzimuthDome.getParamMaxFreqArray()[indexMaxFreqCentroid] > mAzimuthDome.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mAzimuthDome.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mAzimuthDome.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mAzimuthDome.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mAzimuthDome.getParamFactorArray()[indexFactorNoise] > 0))
                )
                ) {
                return true;
            }

            if ((mElevationDome.getParameterComboBoxIndex() > 3 && mElevationDome.getParameterComboBoxIndex() < 7) &&
                (
                (mElevationDome.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mElevationDome.getParamMaxFreqArray()[indexMaxFreqCentroid] > mElevationDome.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mElevationDome.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mElevationDome.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mElevationDome.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mElevationDome.getParamFactorArray()[indexFactorNoise] > 0)
                )
                ) {
                return true;
            }

            if ((mHSpanDome.getParameterComboBoxIndex() > 3 && mHSpanDome.getParameterComboBoxIndex() < 7) &&
                (
                (mHSpanDome.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mHSpanDome.getParamMaxFreqArray()[indexMaxFreqCentroid] > mHSpanDome.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mHSpanDome.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mHSpanDome.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mHSpanDome.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mHSpanDome.getParamFactorArray()[indexFactorNoise] > 0)
                )
                ) {
                return true;
            }

            if ((mVSpanDome.getParameterComboBoxIndex() > 3 && mVSpanDome.getParameterComboBoxIndex() < 7) &&
                (
                (mVSpanDome.getParamRangeArray()[indexSmoothAndRangeCentroid] != 0 && mVSpanDome.getParamMaxFreqArray()[indexMaxFreqCentroid] > mVSpanDome.getParamMinFreqArray()[indexMinFreqCentroid]) ||
                (mVSpanDome.getParamRangeArray()[indexSmoothAndRangeSpread] != 0 && mVSpanDome.getParamFactorArray()[indexFactorSpread] > 0) ||
                (mVSpanDome.getParamRangeArray()[indexSmoothAndRangeNoise] != 0 && mVSpanDome.getParamFactorArray()[indexFactorNoise] > 0)
                )
                ) {
                return true;
            }
        }
        return false;
    }

    bool checkConditionLoudnessAzimuth() {
        if (mAzimuthDome.getParameterComboBoxIndex() == 2 && mAzimuthDome.getParamArrayValue(indexFactorLoudness, mAzimuthDome.getParamFactorArray()) > 0 &&
            mAzimuthDome.getParamArrayValue(indexSmoothAndRangeLoudness, mAzimuthDome.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessElevation() {
        if (mElevationDome.getParameterComboBoxIndex() == 2 && mElevationDome.getParamArrayValue(indexFactorLoudness, mElevationDome.getParamFactorArray()) > 0 &&
            mElevationDome.getParamArrayValue(indexSmoothAndRangeLoudness, mElevationDome.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessHSpan() {
        if (
            mHSpanDome.getParameterComboBoxIndex() == 2 &&
            mHSpanDome.getParamArrayValue(indexFactorLoudness, mHSpanDome.getParamFactorArray()) > 0 &&
            mHSpanDome.getParamArrayValue(indexSmoothAndRangeLoudness, mHSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessVSpan() {
        if (
            mVSpanDome.getParameterComboBoxIndex() == 2 &&
            mVSpanDome.getParamArrayValue(indexFactorLoudness, mVSpanDome.getParamFactorArray()) > 0 &&
            mVSpanDome.getParamArrayValue(indexSmoothAndRangeLoudness, mVSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionForLoudnessAnalyse() {
        if ((checkConditionLoudnessAzimuth()) ||
            (checkConditionLoudnessElevation()) ||
            (checkConditionLoudnessHSpan()) ||
            (checkConditionLoudnessVSpan())
            ) {
            return true;
        }
        return false;
    }

    bool checkConditionPitchAzimuth() {
        if (mAzimuthDome.getParameterComboBoxIndex() == 3 &&
            mAzimuthDome.getParamArrayValue(indexMinFreqPitch, mAzimuthDome.getParamMinFreqArray()) < mAzimuthDome.getParamArrayValue(indexMaxFreqPitch, mAzimuthDome.getParamMaxFreqArray()) &&
            mAzimuthDome.getParamArrayValue(indexSmoothAndRangePitch, mAzimuthDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionAnalyse() {
        if (mAzimuthDome.getParameterComboBoxIndex() == 7 || mElevationDome.getParameterComboBoxIndex() == 7 || mHSpanDome.getParameterComboBoxIndex() == 7 || mVSpanDome.getParameterComboBoxIndex() == 7) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionAzimuth() {
        if (mAzimuthDome.getParameterComboBoxIndex() == 7 && mAzimuthDome.getParameterMinTime() < mAzimuthDome.getParameterMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionElevation() {
        if (mElevationDome.getParameterComboBoxIndex() == 7 && mElevationDome.getParameterMinTime() < mElevationDome.getParameterMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionHSpan() {
        if (mHSpanDome.getParameterComboBoxIndex() == 7 && mHSpanDome.getParameterMinTime() < mHSpanDome.getParameterMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionVSpan() {
        if (mVSpanDome.getParameterComboBoxIndex() == 7 && mVSpanDome.getParameterMinTime() < mVSpanDome.getParameterMaxTime()) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchElevation() {
        if (mElevationDome.getParameterComboBoxIndex() == 3 &&
            mElevationDome.getParamArrayValue(indexMinFreqPitch, mElevationDome.getParamMinFreqArray()) < mElevationDome.getParamArrayValue(indexMaxFreqPitch, mElevationDome.getParamMaxFreqArray()) &&
            mElevationDome.getParamArrayValue(indexSmoothAndRangePitch, mElevationDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchHSpan() {
        if (
            mHSpanDome.getParameterComboBoxIndex() == 3 &&
            mHSpanDome.getParamArrayValue(indexMinFreqPitch, mHSpanDome.getParamMinFreqArray()) < mHSpanDome.getParamArrayValue(indexMaxFreqPitch, mHSpanDome.getParamMaxFreqArray()) &&
            mHSpanDome.getParamArrayValue(indexSmoothAndRangePitch, mHSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchVSpan() {
        if (
            mVSpanDome.getParameterComboBoxIndex() == 3 &&
            mVSpanDome.getParamArrayValue(indexMinFreqPitch, mVSpanDome.getParamMinFreqArray()) < mVSpanDome.getParamArrayValue(indexMaxFreqPitch, mVSpanDome.getParamMaxFreqArray()) &&
            mVSpanDome.getParamArrayValue(indexSmoothAndRangePitch, mVSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionForPitchAnalyse() {
        if (checkConditionPitchAzimuth() || checkConditionPitchElevation()|| checkConditionPitchHSpan()|| checkConditionPitchVSpan()) {
            return true;
        }
        return false;
    }

    bool checkConditionNoiseAzimuth() {
        if (mAzimuthDome.getParameterComboBoxIndex() == 6 && mAzimuthDome.getParamArrayValue(indexFactorNoise, mAzimuthDome.getParamFactorArray()) > 0 &&
            mAzimuthDome.getParamArrayValue(indexSmoothAndRangeNoise, mAzimuthDome.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseElevation() {
        if (mElevationDome.getParameterComboBoxIndex() == 6 &&
            mElevationDome.getParamArrayValue(indexFactorNoise, mElevationDome.getParamFactorArray()) > 0 &&
            mElevationDome.getParamArrayValue(indexSmoothAndRangeNoise, mElevationDome.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseHSpan() {
        if (
            mHSpanDome.getParameterComboBoxIndex() == 6 &&
            mHSpanDome.getParamArrayValue(indexFactorNoise, mHSpanDome.getParamFactorArray()) > 0 &&
            mHSpanDome.getParamArrayValue(indexSmoothAndRangeNoise, mHSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseVSpan() {
        if (
            mVSpanDome.getParameterComboBoxIndex() == 6 &&
            mVSpanDome.getParamArrayValue(indexFactorNoise, mVSpanDome.getParamFactorArray()) > 0 &&
            mVSpanDome.getParamArrayValue(indexSmoothAndRangeNoise, mVSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionForNoiseAnalyse() {
        if (checkConditionNoiseAzimuth() ||
            checkConditionNoiseElevation() ||
            checkConditionNoiseHSpan() ||
            checkConditionNoiseVSpan()
            ) {
            return true;
        }
        return false;
    }

    bool checkConditionSpreadAzimuth() {
        if (mAzimuthDome.getParameterComboBoxIndex() == 5 && mAzimuthDome.getParamArrayValue(indexFactorSpread, mAzimuthDome.getParamFactorArray()) > 0 &&
            mAzimuthDome.getParamArrayValue(indexSmoothAndRangeSpread, mAzimuthDome.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadElevation() {
        if (mElevationDome.getParameterComboBoxIndex() == 5 &&
            mElevationDome.getParamArrayValue(indexFactorSpread, mElevationDome.getParamFactorArray()) > 0 &&
            mElevationDome.getParamArrayValue(indexSmoothAndRangeSpread, mElevationDome.getParamRangeArray()) != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadHSpan() {
        if (
            mHSpanDome.getParameterComboBoxIndex() == 5 &&
            mHSpanDome.getParamArrayValue(indexFactorSpread, mHSpanDome.getParamFactorArray()) > 0 &&
            mHSpanDome.getParamArrayValue(indexSmoothAndRangeSpread, mHSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadVSpan() {
        if (
            mVSpanDome.getParameterComboBoxIndex() == 5 &&
            mVSpanDome.getParamArrayValue(indexFactorSpread, mVSpanDome.getParamFactorArray()) > 0 &&
            mVSpanDome.getParamArrayValue(indexSmoothAndRangeSpread, mVSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionForSpreadAnalyse() {
        if (checkConditionSpreadAzimuth() ||
            checkConditionSpreadElevation() ||
            checkConditionSpreadHSpan() ||
            checkConditionSpreadVSpan()
            ) {
            return true;
        }
        return false;
    }

    bool checkConditionCentroidAzimuth() {
        if (mAzimuthDome.getParameterComboBoxIndex() == 4 &&
            mAzimuthDome.getParamArrayValue(indexMinFreqCentroid, mAzimuthDome.getParamMinFreqArray()) < mAzimuthDome.getParamArrayValue(indexMaxFreqCentroid, mAzimuthDome.getParamMaxFreqArray()) &&
            mAzimuthDome.getParamArrayValue(indexSmoothAndRangeCentroid, mAzimuthDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidElevation() {
        if (mElevationDome.getParameterComboBoxIndex() == 4 &&
            mElevationDome.getParamArrayValue(indexMinFreqCentroid, mElevationDome.getParamMinFreqArray()) < mElevationDome.getParamArrayValue(indexMaxFreqCentroid, mElevationDome.getParamMaxFreqArray()) &&
            mElevationDome.getParamArrayValue(indexSmoothAndRangeCentroid, mElevationDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidHSpan() {
        if (
            mHSpanDome.getParameterComboBoxIndex() == 4 &&
            mHSpanDome.getParamArrayValue(indexMinFreqCentroid, mHSpanDome.getParamMinFreqArray()) < mHSpanDome.getParamArrayValue(indexMaxFreqCentroid, mHSpanDome.getParamMaxFreqArray()) &&
            mHSpanDome.getParamArrayValue(indexSmoothAndRangeCentroid, mHSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidVSpan() {
        if (
            mVSpanDome.getParameterComboBoxIndex() == 4 &&
            mVSpanDome.getParamArrayValue(indexMinFreqCentroid, mVSpanDome.getParamMinFreqArray()) < mVSpanDome.getParamArrayValue(indexMaxFreqCentroid, mVSpanDome.getParamMaxFreqArray()) &&
            mVSpanDome.getParamArrayValue(indexSmoothAndRangeCentroid, mVSpanDome.getParamRangeArray()) != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionForCentroidAnalyse() {
        if (checkConditionCentroidAzimuth() || checkConditionCentroidElevation() || checkConditionCentroidHSpan() || checkConditionCentroidVSpan()) {
            return true;
        }
        return false;
    }

//=================================================================================
    AzimuthDome& getAzimuthDome() {
        return mAzimuthDome;
    }

    ElevationDome& getElevationDome() {
        return mElevationDome;
    }

    HspanDome& getHSpanDome() {
        return mHSpanDome;
    }

    VspanDome& getVSpanDome() {
        return mVSpanDome;
    }

//=================================================================================
private:
    AzimuthDome mAzimuthDome;
    ElevationDome mElevationDome;
    HspanDome mHSpanDome;
    VspanDome mVSpanDome;
};