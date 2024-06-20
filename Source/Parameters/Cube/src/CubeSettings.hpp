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
    CubeSettings() = delete;
    explicit CubeSettings(juce::AudioProcessorValueTreeState& audioProcessorValueTreeState)
        : mXCube(audioProcessorValueTreeState)
        , mYCube(audioProcessorValueTreeState)
        , mZCube(audioProcessorValueTreeState)
        , mHSpanCube(audioProcessorValueTreeState)
        , mVSpanCube(audioProcessorValueTreeState)
    {};

/// /////////////////////////// Les conditions de verifications//////////////////////////

    bool checkConditionNeedSpectralAnalyse() {
        if (mXCube.getParamDescriptorComboBoxIndex() > 3 || mYCube.getParamDescriptorComboBoxIndex() > 3 || mZCube.getParamDescriptorComboBoxIndex() > 3 || mHSpanCube.getParamDescriptorComboBoxIndex() > 3 || mVSpanCube.getParamDescriptorComboBoxIndex() > 3) {
            if ((mXCube.getParamDescriptorComboBoxIndex() > 3 && mXCube.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mXCube.getParamRangeCentroid() != 0 && mXCube.getParamMaxFreqCentroid() > mXCube.getParamMinFreqCentroid()) ||
                (mXCube.getParamRangeSpread() != 0 && mXCube.getParamFactorSpread() > 0) ||
                (mXCube.getParamRangeNoise() != 0 && mXCube.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }

            if ((mYCube.getParamDescriptorComboBoxIndex() > 3 && mYCube.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mYCube.getParamRangeCentroid() != 0 && mYCube.getParamMaxFreqCentroid() > mYCube.getParamMinFreqCentroid()) ||
                (mYCube.getParamRangeSpread() != 0 && mYCube.getParamFactorSpread() > 0) ||
                (mYCube.getParamRangeNoise() != 0 && mYCube.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }

            if ((mZCube.getParamDescriptorComboBoxIndex() > 3 && mZCube.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mZCube.getParamRangeCentroid() != 0 && mZCube.getParamMaxFreqCentroid() > mZCube.getParamMinFreqCentroid()) ||
                (mZCube.getParamRangeSpread() != 0 && mZCube.getParamFactorSpread() > 0) ||
                (mZCube.getParamRangeNoise() != 0 && mZCube.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }

            if ((mHSpanCube.getParamDescriptorComboBoxIndex() > 3 && mHSpanCube.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mHSpanCube.getParamRangeCentroid() != 0 && mHSpanCube.getParamMaxFreqCentroid() > mHSpanCube.getParamMinFreqCentroid()) ||
                (mHSpanCube.getParamRangeSpread() != 0 && mHSpanCube.getParamFactorSpread() > 0) ||
                (mHSpanCube.getParamRangeNoise() != 0 && mHSpanCube.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }

            if ((mVSpanCube.getParamDescriptorComboBoxIndex() > 3 && mVSpanCube.getParamDescriptorComboBoxIndex() < 7) &&
                (
                (mVSpanCube.getParamRangeCentroid() != 0 && mVSpanCube.getParamMaxFreqCentroid() > mVSpanCube.getParamMinFreqCentroid()) ||
                (mVSpanCube.getParamRangeSpread() != 0 && mVSpanCube.getParamFactorSpread() > 0) ||
                (mVSpanCube.getParamRangeNoise() != 0 && mVSpanCube.getParamFactorNoise() > 0)
                )
                ) {
                return true;
            }
        }
        return false;
    }
    bool checkConditionLoudnessX() {
        if (mXCube.getParamDescriptorComboBoxIndex() == 2 && mXCube.getParamFactorLoudness() > 0 && mXCube.getParamRangeLoudness() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessY() {
        if (mYCube.getParamDescriptorComboBoxIndex() == 2 && mYCube.getParamFactorLoudness() > 0 && mYCube.getParamRangeLoudness() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessZ() {
        if (mZCube.getParamDescriptorComboBoxIndex() == 2 && mZCube.getParamFactorLoudness() > 0 && mZCube.getParamRangeLoudness() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessHSpan() {
        if (mHSpanCube.getParamDescriptorComboBoxIndex() == 2 && mHSpanCube.getParamFactorLoudness() > 0 && mHSpanCube.getParamRangeLoudness() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionLoudnessVSpan() {
        if (mVSpanCube.getParamDescriptorComboBoxIndex() == 2 && mVSpanCube.getParamFactorLoudness() > 0 && mVSpanCube.getParamRangeLoudness() != 0) {
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
        if (mXCube.getParamDescriptorComboBoxIndex() == 7 || mYCube.getParamDescriptorComboBoxIndex() == 7 || mZCube.getParamDescriptorComboBoxIndex() == 7 || mHSpanCube.getParamDescriptorComboBoxIndex() == 7 || mVSpanCube.getParamDescriptorComboBoxIndex() == 7) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionX() {
        if (mXCube.getParamDescriptorComboBoxIndex() == 7 && mXCube.getParamMinTime() < mXCube.getParamMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionY() {
        if (mYCube.getParamDescriptorComboBoxIndex() == 7 && mYCube.getParamMinTime() < mYCube.getParamMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionZ() {
        if (mZCube.getParamDescriptorComboBoxIndex() == 7 && mZCube.getParamMinTime() < mZCube.getParamMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionHSpan() {
        if (mHSpanCube.getParamDescriptorComboBoxIndex() == 7 && mHSpanCube.getParamMinTime() < mHSpanCube.getParamMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionOnsetDetectionVSpan() {
        if (mVSpanCube.getParamDescriptorComboBoxIndex() == 7 && mVSpanCube.getParamMinTime() < mVSpanCube.getParamMaxTime()) {
            return true;
        }
        return false;
    }

    bool checkConditionPitchX() {
        if (mXCube.getParamDescriptorComboBoxIndex() == 3 &&
            mXCube.getParamMinFreqPitch() < mXCube.getParamMaxFreqPitch() &&
            mXCube.getParamRangePitch() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchY() {
        if (mYCube.getParamDescriptorComboBoxIndex() == 3 &&
            mYCube.getParamMinFreqPitch() < mYCube.getParamMaxFreqPitch() &&
            mYCube.getParamRangePitch() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchZ() {
        if (mZCube.getParamDescriptorComboBoxIndex() == 3 &&
            mZCube.getParamMinFreqPitch() < mZCube.getParamMaxFreqPitch() &&
            mZCube.getParamRangePitch() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchHSpan() {
        if (mHSpanCube.getParamDescriptorComboBoxIndex() == 3 &&
            mHSpanCube.getParamMinFreqPitch() < mHSpanCube.getParamMaxFreqPitch() &&
            mHSpanCube.getParamRangePitch() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionPitchVSpan() {
        if (mVSpanCube.getParamDescriptorComboBoxIndex() == 3 &&
            mVSpanCube.getParamMinFreqPitch() < mVSpanCube.getParamMaxFreqPitch() &&
            mVSpanCube.getParamRangePitch() != 0
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
        if (mXCube.getParamDescriptorComboBoxIndex() == 6 && mXCube.getParamFactorNoise() > 0 && mXCube.getParamRangeNoise() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseY() {
        if (mYCube.getParamDescriptorComboBoxIndex() == 6 && mYCube.getParamFactorNoise() > 0 && mYCube.getParamRangeNoise() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseZ() {
        if (mZCube.getParamDescriptorComboBoxIndex() == 6 && mZCube.getParamFactorNoise() > 0 && mZCube.getParamRangeNoise() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseHSpan() {
        if (mHSpanCube.getParamDescriptorComboBoxIndex() == 6 && mHSpanCube.getParamFactorNoise() > 0 && mHSpanCube.getParamRangeNoise() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionNoiseVSpan() {
        if (mVSpanCube.getParamDescriptorComboBoxIndex() == 6 && mVSpanCube.getParamFactorNoise() > 0 && mVSpanCube.getParamRangeNoise() != 0) {
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
        if (mXCube.getParamDescriptorComboBoxIndex() == 5 && mXCube.getParamFactorSpread() > 0 && mXCube.getParamRangeSpread() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadY() {
        if (mYCube.getParamDescriptorComboBoxIndex() == 5 && mYCube.getParamFactorSpread() > 0 && mYCube.getParamRangeSpread() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadZ() {
        if (mZCube.getParamDescriptorComboBoxIndex() == 5 && mZCube.getParamFactorSpread() > 0 && mZCube.getParamRangeSpread() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadHSpan() {
        if (mHSpanCube.getParamDescriptorComboBoxIndex() == 5 && mHSpanCube.getParamFactorSpread() > 0 && mHSpanCube.getParamRangeSpread() != 0) {
            return true;
        }
        return false;
    }
    bool checkConditionSpreadVSpan() {
        if (mVSpanCube.getParamDescriptorComboBoxIndex() == 5 && mVSpanCube.getParamFactorSpread() > 0 && mVSpanCube.getParamRangeSpread() != 0) {
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
        if (mXCube.getParamDescriptorComboBoxIndex() == 4 &&
            mXCube.getParamMinFreqCentroid() < mXCube.getParamMaxFreqCentroid() &&
            mXCube.getParamRangeCentroid() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidY() {
        if (mYCube.getParamDescriptorComboBoxIndex() == 4 &&
            mYCube.getParamMinFreqCentroid() < mYCube.getParamMaxFreqCentroid() &&
            mYCube.getParamRangeCentroid() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidZ() {
        if (mZCube.getParamDescriptorComboBoxIndex() == 4 &&
            mZCube.getParamMinFreqCentroid() < mZCube.getParamMaxFreqCentroid() &&
            mZCube.getParamRangeCentroid() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidHSpan() {
        if (mHSpanCube.getParamDescriptorComboBoxIndex() == 4 &&
            mHSpanCube.getParamMinFreqCentroid() < mHSpanCube.getParamMaxFreqCentroid() &&
            mHSpanCube.getParamRangeCentroid() != 0
            ) {
            return true;
        }
        return false;
    }
    bool checkConditionCentroidVSpan() {
        if (mVSpanCube.getParamDescriptorComboBoxIndex() == 4 &&
            mVSpanCube.getParamMinFreqCentroid() < mVSpanCube.getParamMaxFreqCentroid() &&
            mVSpanCube.getParamRangeCentroid() != 0
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

    //=================================================================================
    void updateCubeParametersState() {
        mXCube.updateParameterState();
        mYCube.updateParameterState();
        mZCube.updateParameterState();
        mHSpanCube.updateParameterState();
        mVSpanCube.updateParameterState();
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

    //==============================================================================
    JUCE_LEAK_DETECTOR(CubeSettings)
};