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
    DomeSettings() = delete;
    explicit DomeSettings(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState)
        : mAzimuthDome(audioProcessorValueTreeState)
        , mElevationDome(audioProcessorValueTreeState)
        , mHSpanDome(audioProcessorValueTreeState)
        , mVSpanDome(audioProcessorValueTreeState)
    {};

    bool checkConditionNeedSpectralAnalyse() {
        if (mAzimuthDome.getParamDescriptorComboBoxIndex() > 3 || mElevationDome.getParamDescriptorComboBoxIndex() > 3 || mHSpanDome.getParamDescriptorComboBoxIndex() > 3 || mVSpanDome.getParamDescriptorComboBoxIndex() > 3) {
            if ((mAzimuthDome.getParamDescriptorComboBoxIndex() > 3 && mAzimuthDome.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mAzimuthDome.getParamRangeCentroid() != 0 && mAzimuthDome.getParamMaxFreqCentroid() > mAzimuthDome.getParamMinFreqCentroid()) ||
                (mAzimuthDome.getParamRangeSpread() != 0 && mAzimuthDome.getParamFactorSpread() > 0) ||
                (mAzimuthDome.getParamRangeNoise() != 0 && mAzimuthDome.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }

            if ((mElevationDome.getParamDescriptorComboBoxIndex() > 3 && mElevationDome.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mElevationDome.getParamRangeCentroid() != 0 && mElevationDome.getParamMaxFreqCentroid() > mElevationDome.getParamMinFreqCentroid()) ||
                (mElevationDome.getParamRangeSpread() != 0 && mElevationDome.getParamFactorSpread() > 0) ||
                (mElevationDome.getParamRangeNoise() != 0 && mElevationDome.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }

            if ((mHSpanDome.getParamDescriptorComboBoxIndex() > 3 && mHSpanDome.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mHSpanDome.getParamRangeCentroid() != 0 && mHSpanDome.getParamMaxFreqCentroid() > mHSpanDome.getParamMinFreqCentroid()) ||
                (mHSpanDome.getParamRangeSpread() != 0 && mHSpanDome.getParamFactorSpread() > 0) ||
                (mHSpanDome.getParamRangeNoise() != 0 && mHSpanDome.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }

            if ((mVSpanDome.getParamDescriptorComboBoxIndex() > 3 && mVSpanDome.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mVSpanDome.getParamRangeCentroid() != 0 && mVSpanDome.getParamMaxFreqCentroid() > mVSpanDome.getParamMinFreqCentroid()) ||
                (mVSpanDome.getParamRangeSpread() != 0 && mVSpanDome.getParamFactorSpread() > 0) ||
                (mVSpanDome.getParamRangeNoise() != 0 && mVSpanDome.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }
        }
        return false;
    }

    bool checkConditionLoudnessAzimuth() {
        if (
            mAzimuthDome.getParamDescriptorComboBoxIndex() == 2 &&
            mAzimuthDome.getParamFactorLoudness() > 0 &&
            mAzimuthDome.getParamRangeLoudness() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessElevation() {
        if (
            mElevationDome.getParamDescriptorComboBoxIndex() == 2 &&
            mElevationDome.getParamFactorLoudness() > 0 &&
            mElevationDome.getParamRangeLoudness() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessHSpan() {
        if (
            mHSpanDome.getParamDescriptorComboBoxIndex() == 2 &&
            mHSpanDome.getParamFactorLoudness() > 0 &&
            mHSpanDome.getParamRangeLoudness() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessVSpan() {
        if (
            mVSpanDome.getParamDescriptorComboBoxIndex() == 2 &&
            mVSpanDome.getParamFactorLoudness() > 0 &&
            mVSpanDome.getParamRangeLoudness() != 0
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
        if (mAzimuthDome.getParamDescriptorComboBoxIndex() == 3 &&
            mAzimuthDome.getParamMinFreqPitch() < mAzimuthDome.getParamMaxFreqPitch() &&
            mAzimuthDome.getParamRangePitch() != 0
            ) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionAnalyse() {
        if (mAzimuthDome.getParamDescriptorComboBoxIndex() == 7 || mElevationDome.getParamDescriptorComboBoxIndex() == 7 || mHSpanDome.getParamDescriptorComboBoxIndex() == 7 || mVSpanDome.getParamDescriptorComboBoxIndex() == 7) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionAzimuth() {
        if (mAzimuthDome.getParamDescriptorComboBoxIndex() == 7 && mAzimuthDome.getParamMinTime() < mAzimuthDome.getParamMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionElevation() {
        if (mElevationDome.getParamDescriptorComboBoxIndex() == 7 && mElevationDome.getParamMinTime() < mElevationDome.getParamMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionHSpan() {
        if (mHSpanDome.getParamDescriptorComboBoxIndex() == 7 && mHSpanDome.getParamMinTime() < mHSpanDome.getParamMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionVSpan() {
        if (mVSpanDome.getParamDescriptorComboBoxIndex() == 7 && mVSpanDome.getParamMinTime() < mVSpanDome.getParamMaxTime()) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchElevation() {
        if (mElevationDome.getParamDescriptorComboBoxIndex() == 3 &&
            mElevationDome.getParamMinFreqPitch() < mElevationDome.getParamMaxFreqPitch() &&
            mElevationDome.getParamRangePitch() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchHSpan() {
        if (
            mHSpanDome.getParamDescriptorComboBoxIndex() == 3 &&
            mHSpanDome.getParamMinFreqPitch() < mHSpanDome.getParamMaxFreqPitch() &&
            mHSpanDome.getParamRangePitch() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchVSpan() {
        if (
            mVSpanDome.getParamDescriptorComboBoxIndex() == 3 &&
            mVSpanDome.getParamMinFreqPitch() < mVSpanDome.getParamMaxFreqPitch() &&
            mVSpanDome.getParamRangePitch() != 0
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
        if (
            mAzimuthDome.getParamDescriptorComboBoxIndex() == 6 &&
            mAzimuthDome.getParamFactorNoise() > 0 &&
            mAzimuthDome.getParamRangeNoise() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseElevation() {
        if (mElevationDome.getParamDescriptorComboBoxIndex() == 6 &&
            mElevationDome.getParamFactorNoise() > 0 &&
            mElevationDome.getParamRangeNoise() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseHSpan() {
        if (
            mHSpanDome.getParamDescriptorComboBoxIndex() == 6 &&
            mHSpanDome.getParamFactorNoise() > 0 &&
            mHSpanDome.getParamRangeNoise() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseVSpan() {
        if (
            mVSpanDome.getParamDescriptorComboBoxIndex() == 6 &&
            mVSpanDome.getParamFactorNoise() > 0 &&
            mVSpanDome.getParamRangeNoise() != 0
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
        if (
            mAzimuthDome.getParamDescriptorComboBoxIndex() == 5 &&
            mAzimuthDome.getParamFactorSpread() > 0 &&
            mAzimuthDome.getParamRangeSpread() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadElevation() {
        if (mElevationDome.getParamDescriptorComboBoxIndex() == 5 &&
            mElevationDome.getParamFactorSpread() > 0 &&
            mElevationDome.getParamRangeSpread() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadHSpan() {
        if (
            mHSpanDome.getParamDescriptorComboBoxIndex() == 5 &&
            mHSpanDome.getParamFactorSpread() > 0 &&
            mHSpanDome.getParamRangeSpread() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadVSpan() {
        if (
            mVSpanDome.getParamDescriptorComboBoxIndex() == 5 &&
            mVSpanDome.getParamFactorSpread() > 0 &&
            mVSpanDome.getParamRangeSpread() != 0
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
        if (mAzimuthDome.getParamDescriptorComboBoxIndex() == 4 &&
            mAzimuthDome.getParamMinFreqCentroid() < mAzimuthDome.getParamMaxFreqCentroid() &&
            mAzimuthDome.getParamRangeCentroid() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidElevation() {
        if (mElevationDome.getParamDescriptorComboBoxIndex() == 4 &&
            mElevationDome.getParamMinFreqCentroid() < mElevationDome.getParamMaxFreqCentroid() &&
            mElevationDome.getParamRangeCentroid() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidHSpan() {
        if (
            mHSpanDome.getParamDescriptorComboBoxIndex() == 4 &&
            mHSpanDome.getParamMinFreqCentroid() < mHSpanDome.getParamMaxFreqCentroid() &&
            mHSpanDome.getParamRangeCentroid() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidVSpan() {
        if (
            mVSpanDome.getParamDescriptorComboBoxIndex() == 4 &&
            mVSpanDome.getParamMinFreqCentroid() < mVSpanDome.getParamMaxFreqCentroid() &&
            mVSpanDome.getParamRangeCentroid() != 0
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
    void updateDomeParametersState() {
        mAzimuthDome.updateParameterState();
        mElevationDome.updateParameterState();
        mHSpanDome.updateParameterState();
        mVSpanDome.updateParameterState();
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