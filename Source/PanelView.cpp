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

#include "PanelView.h"

PanelView::PanelView(AudioDescriptorsAudioProcessor& processor, Parameters& parameter)
    : mAudioProcessor(processor)
    , mParameter(parameter)
{
    setSize(1000, 1000);

    if (parameter.getParameterID() == ParameterID::azimuth) {
        addAndMakeVisible(mDescriptorLapLabel);
        mDescriptorLapLabel.setText("Lap", juce::dontSendNotification);
        mDescriptorLap.setRange(1, 4, 1);
        mDescriptorLap.setSliderStyle(juce::Slider::LinearHorizontal);
        mDescriptorLap.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
        addAndMakeVisible(mDescriptorLap);
        isAzimuth = true;
    }

    if (parameter.getParameterID() == ParameterID::z ||
        parameter.getParameterID() == ParameterID::elevation ||
        parameter.getParameterID() == ParameterID::elevationspan) {
        addAndMakeVisible(mDescriptorOffsetLabel);
        mDescriptorOffsetLabel.setText("Offset", juce::dontSendNotification);
        if (parameter.getParameterID() == ParameterID::elevation) {
            mDescriptorOffset.setRange(0, 90);
        }
        else {
            mDescriptorOffset.setRange(0, 1);
        }
        mDescriptorOffset.setSliderStyle(juce::Slider::LinearHorizontal);
        mDescriptorOffset.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
        addAndMakeVisible(mDescriptorOffset);
        isOffset = true;
    }
    addAndMakeVisible(mDescriptorLabel);
    mDescriptorLabel.setText(mParameter.getParameterName(), juce::dontSendNotification);

    addAndMakeVisible(mDescriptorFactorLabel);
    mDescriptorFactorLabel.setText("Factor", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorSmoothLabel);
    mDescriptorSmoothLabel.setText("Smooth", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorMoreSmoothLabel);
    mDescriptorMoreSmoothLabel.setText("More Smooth", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorRangeLabel);
    mDescriptorRangeLabel.setText("Range", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorMaxFreqLabel);
    mDescriptorMaxFreqLabel.setText("Max Freq", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorMinFreqLabel);
    mDescriptorMinFreqLabel.setText("Min Freq", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorThresholdLabel);
    mDescriptorThresholdLabel.setText("Threshold", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorMinTimeLabel);
    mDescriptorMinTimeLabel.setText("Min. Time", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorMaxTimeLabel);
    mDescriptorMaxTimeLabel.setText("Max. Time", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorMetricComboBox);
    mDescriptorMetricComboBox.addItemList(metricBoxItems, 1);
    mDescriptorMetricComboBox.setSelectedId(parameter.getParameterMetricComboBoxIndex());

    mDescriptorMetricComboBox.addListener(this);
    mDescriptorMetricComboBox.onChange = [this, &parameter] {
        parameter.setParameterMetricComboBoxIndex(mDescriptorMetricComboBox.getSelectedId());
        mAudioProcessor.setOnsetDetectionMetric(mParameter.getParameterID(), mDescriptorMetricComboBox.getSelectedId());
        };

    mDescriptorComboBox.addItemList(boxItems, 1);
    addAndMakeVisible(mDescriptorComboBox);
    mDescriptorComboBox.setSelectedId(mParameter.getParameterComboBoxIndex());
    mDescriptorComboBox.addListener(this);
    mDescriptorComboBox.onChange = [this] {
        mParameter.setParameterComboBoxIndex(mDescriptorComboBox.getSelectedId());
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mDescriptorFactor.setValue(mParameter.getParamArrayValue(indexFactorLoudness, mParameter.getParamFactorArray()));
            mDescriptorSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeLoudness, mParameter.getParamSmoothArray()));
            mDescriptorRange.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeLoudness, mParameter.getParamRangeArray()));
            mDescriptorMoreSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeLoudness, mParameter.getParamMoreSmoothArray()));
            if (isAzimuth) {
                mDescriptorLap.setValue(mParameter.getParamArrayValue(indexLapLoudness, mParameter.getParamLapArray()));
            }
            if (isOffset) {
                mDescriptorOffset.setValue(mParameter.getParamArrayValue(indexOffsetLoudness, mParameter.getParamOffsetArray()));
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 3) {
            mDescriptorSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangePitch,mParameter.getParamSmoothArray()));
            mDescriptorRange.setValue(mParameter.getParamArrayValue(indexSmoothAndRangePitch,mParameter.getParamRangeArray()));
            mDescriptorMinFreq.setValue(mParameter.getParamArrayValue(indexMinFreqPitch,mParameter.getParamMinFreqArray()));
            mDescriptorMaxFreq.setValue(mParameter.getParamArrayValue(indexMaxFreqPitch,mParameter.getParamMaxFreqArray()));
            mDescriptorMoreSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangePitch, mParameter.getParamMoreSmoothArray()));
            if (isAzimuth) {
                mDescriptorLap.setValue(mParameter.getParamArrayValue(indexLapPitch, mParameter.getParamLapArray()));
            }
            if (isOffset) {
                mDescriptorOffset.setValue(mParameter.getParamArrayValue(indexOffsetPitch, mParameter.getParamOffsetArray()));
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mDescriptorSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeCentroid, mParameter.getParamSmoothArray()));
            mDescriptorRange.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeCentroid, mParameter.getParamRangeArray()));
            mDescriptorMinFreq.setValue(mParameter.getParamArrayValue(indexMinFreqCentroid, mParameter.getParamMinFreqArray()));
            mDescriptorMaxFreq.setValue(mParameter.getParamArrayValue(indexMaxFreqCentroid, mParameter.getParamMaxFreqArray()));
            mDescriptorMoreSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeCentroid, mParameter.getParamMoreSmoothArray()));
            if (isAzimuth) {
                mDescriptorLap.setValue(mParameter.getParamArrayValue(indexLapCentroid, mParameter.getParamLapArray()));
            }
            if (isOffset) {
                mDescriptorOffset.setValue(mParameter.getParamArrayValue(indexOffsetCentroid, mParameter.getParamOffsetArray()));
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mDescriptorFactor.setValue(mParameter.getParamArrayValue(indexFactorSpread, mParameter.getParamFactorArray()));
            mDescriptorSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeSpread, mParameter.getParamSmoothArray()));
            mDescriptorRange.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeSpread, mParameter.getParamRangeArray()));
            mDescriptorMoreSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeSpread, mParameter.getParamMoreSmoothArray()));
            if (isAzimuth) {
                mDescriptorLap.setValue(mParameter.getParamArrayValue(indexLapSpread, mParameter.getParamLapArray()));
            }
            if (isOffset) {
                mDescriptorOffset.setValue(mParameter.getParamArrayValue(indexOffsetSpread, mParameter.getParamOffsetArray()));
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mDescriptorFactor.setValue(mParameter.getParamArrayValue(indexFactorNoise, mParameter.getParamFactorArray()));
            mDescriptorSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeNoise, mParameter.getParamSmoothArray()));
            mDescriptorRange.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeNoise, mParameter.getParamRangeArray()));
            mDescriptorMoreSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeNoise, mParameter.getParamMoreSmoothArray()));
            if (isAzimuth) {
                mDescriptorLap.setValue(mParameter.getParamArrayValue(indexLapNoise, mParameter.getParamLapArray()));
            }
            if (isOffset) {
                mDescriptorOffset.setValue(mParameter.getParamArrayValue(indexOffsetNoise, mParameter.getParamOffsetArray()));
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 7) {
            mDescriptorMetricComboBox.setSelectedId(mParameter.getParameterMetricComboBoxIndex());
            mDescriptorThreshold.setValue(mParameter.getParameterThreshold());
            mDescriptorMinTime.setValue(mParameter.getParameterMinTime());
            mDescriptorMaxTime.setValue(mParameter.getParameterMaxTime());
            mDescriptorSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeOnsetDetection, mParameter.getParamSmoothArray()));
            mDescriptorRange.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeOnsetDetection, mParameter.getParamRangeArray()));
            mDescriptorMoreSmooth.setValue(mParameter.getParamArrayValue(indexSmoothAndRangeOnsetDetection, mParameter.getParamMoreSmoothArray()));
            if (isAzimuth) {
                mDescriptorLap.setValue(mParameter.getParamArrayValue(indexLapOnsetDetection, mParameter.getParamLapArray()));
            }
            if (isOffset) {
                mDescriptorOffset.setValue(mParameter.getParamArrayValue(indexOffsetOnsetDetection, mParameter.getParamOffsetArray()));
            }
        }
        };

    mDescriptorFactor.setRange(0, 500);
    mDescriptorFactor.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorFactor.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorFactor);
    mDescriptorFactor.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mParameter.setParamArrayValue(mDescriptorFactor.getValue(), indexFactorLoudness, mParameter.getParamFactorArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mParameter.setParamArrayValue(mDescriptorFactor.getValue(), indexFactorSpread, mParameter.getParamFactorArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mParameter.setParamArrayValue(mDescriptorFactor.getValue(), indexFactorNoise, mParameter.getParamFactorArray());
        }
        };

    if (isAzimuth) {
        mDescriptorLap.onValueChange = [this] {
            if (mDescriptorComboBox.getSelectedId() == 2) {
                mParameter.setParamArrayValue(mDescriptorLap.getValue(), indexLapLoudness, mParameter.getParamLapArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 3) {
                mParameter.setParamArrayValue(mDescriptorLap.getValue(), indexLapPitch, mParameter.getParamLapArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 4) {
                mParameter.setParamArrayValue(mDescriptorLap.getValue(), indexLapCentroid, mParameter.getParamLapArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 5) {
                mParameter.setParamArrayValue(mDescriptorLap.getValue(), indexLapSpread, mParameter.getParamLapArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 6) {
                mParameter.setParamArrayValue(mDescriptorLap.getValue(), indexLapNoise, mParameter.getParamLapArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 7) {
                mParameter.setParamArrayValue(mDescriptorLap.getValue(), indexLapOnsetDetection, mParameter.getParamLapArray());
            }
        };
    }

    if (isOffset) {
        mDescriptorOffset.onValueChange = [this] {
            if (mDescriptorComboBox.getSelectedId() == 2) {
                DBG("VALEUR ! " << mDescriptorOffset.getValue());
                mParameter.setParamArrayValue(mDescriptorOffset.getValue(), indexOffsetLoudness, mParameter.getParamOffsetArray());
                DBG("VALEYR APRES " << mParameter.getParamArrayValue(indexOffsetLoudness, mParameter.getParamOffsetArray()));
            }
            else if (mDescriptorComboBox.getSelectedId() == 3) {
                mParameter.setParamArrayValue(mDescriptorOffset.getValue(), indexOffsetPitch, mParameter.getParamOffsetArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 4) {
                mParameter.setParamArrayValue(mDescriptorOffset.getValue(), indexOffsetCentroid, mParameter.getParamOffsetArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 5) {
                mParameter.setParamArrayValue(mDescriptorOffset.getValue(), indexOffsetSpread, mParameter.getParamOffsetArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 6) {
                mParameter.setParamArrayValue(mDescriptorOffset.getValue(), indexOffsetNoise, mParameter.getParamOffsetArray());
            }
            else if (mDescriptorComboBox.getSelectedId() == 7) {
                mParameter.setParamArrayValue(mDescriptorOffset.getValue(), indexOffsetOnsetDetection, mParameter.getParamOffsetArray());
            }
        };
    }

    mDescriptorSmooth.setRange(0, 100);
    mDescriptorSmooth.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorSmooth.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorSmooth);
    mDescriptorSmooth.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mParameter.setParamArrayValue(mDescriptorSmooth.getValue(), indexSmoothAndRangeLoudness, mParameter.getParamSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamArrayValue(mDescriptorSmooth.getValue(), indexSmoothAndRangePitch, mParameter.getParamSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamArrayValue(mDescriptorSmooth.getValue(), indexSmoothAndRangeCentroid, mParameter.getParamSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mParameter.setParamArrayValue(mDescriptorSmooth.getValue(), indexSmoothAndRangeSpread, mParameter.getParamSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mParameter.setParamArrayValue(mDescriptorSmooth.getValue(), indexSmoothAndRangeNoise, mParameter.getParamSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParamArrayValue(mDescriptorSmooth.getValue(), indexSmoothAndRangeOnsetDetection, mParameter.getParamSmoothArray());
        }
        };

    mDescriptorMoreSmooth.setRange(0, 100);
    mDescriptorMoreSmooth.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMoreSmooth.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMoreSmooth);
    mDescriptorMoreSmooth.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mParameter.setParamArrayValue(mDescriptorMoreSmooth.getValue(), indexSmoothAndRangeLoudness, mParameter.getParamMoreSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamArrayValue(mDescriptorMoreSmooth.getValue(), indexSmoothAndRangePitch, mParameter.getParamMoreSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamArrayValue(mDescriptorMoreSmooth.getValue(), indexSmoothAndRangeCentroid, mParameter.getParamMoreSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mParameter.setParamArrayValue(mDescriptorMoreSmooth.getValue(), indexSmoothAndRangeSpread, mParameter.getParamMoreSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mParameter.setParamArrayValue(mDescriptorMoreSmooth.getValue(), indexSmoothAndRangeNoise, mParameter.getParamMoreSmoothArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParamArrayValue(mDescriptorMoreSmooth.getValue(), indexSmoothAndRangeOnsetDetection, mParameter.getParamMoreSmoothArray());
        }
    };

    mDescriptorRange.setRange(-100, 100);
    mDescriptorRange.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorRange.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorRange);
    mDescriptorRange.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mParameter.setParamArrayValue(mDescriptorRange.getValue(), indexSmoothAndRangeLoudness, mParameter.getParamRangeArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamArrayValue(mDescriptorRange.getValue(), indexSmoothAndRangePitch, mParameter.getParamRangeArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamArrayValue(mDescriptorRange.getValue(), indexSmoothAndRangeCentroid, mParameter.getParamRangeArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mParameter.setParamArrayValue(mDescriptorRange.getValue(), indexSmoothAndRangeSpread, mParameter.getParamRangeArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mParameter.setParamArrayValue(mDescriptorRange.getValue(), indexSmoothAndRangeNoise, mParameter.getParamRangeArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParamArrayValue(mDescriptorRange.getValue(), indexSmoothAndRangeOnsetDetection, mParameter.getParamRangeArray());
        }
        };

    mDescriptorMinFreq.setRange(20, 20000);
    mDescriptorMinFreq.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMinFreq.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMinFreq);
    mDescriptorMinFreq.onValueChange = [this] {
        auto minVal{ mDescriptorMinFreq.getValue() };
        auto maxVal{ mDescriptorMaxFreq.getValue() };
        if (minVal > maxVal) {
            minVal = maxVal;
            mDescriptorMinFreq.setValue(minVal);
        }
        if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamArrayValue(minVal, indexMinFreqPitch, mParameter.getParamMinFreqArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamArrayValue(minVal, indexMinFreqCentroid, mParameter.getParamMinFreqArray());
        }
        };

    mDescriptorMaxFreq.setRange(20, 20000);
    mDescriptorMaxFreq.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMaxFreq.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMaxFreq);
    mDescriptorMaxFreq.onValueChange = [this] {
        auto minVal{ mDescriptorMinFreq.getValue() };
        auto maxVal{ mDescriptorMaxFreq.getValue() };
        if (maxVal < minVal) {
            maxVal = minVal;
            mDescriptorMaxFreq.setValue(maxVal);
        }
        if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamArrayValue(maxVal, indexMaxFreqPitch, mParameter.getParamMaxFreqArray());
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamArrayValue(maxVal, indexMaxFreqCentroid, mParameter.getParamMaxFreqArray());
        }
        };

    mDescriptorThreshold.setRange(0.0, 1.0);
    mDescriptorThreshold.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorThreshold.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorThreshold);
    mDescriptorThreshold.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 7) {
            auto newVal{ mDescriptorThreshold.getValue() };
            mParameter.setParameterThreshold(newVal);
            mAudioProcessor.setOnsetDetectionThreshold(mParameter.getParameterID(), static_cast<float>(newVal));
        }
        };

    mDescriptorMinTime.setRange(0.0, 30.0 + ALMOST_ZERO);
    mDescriptorMinTime.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMinTime.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMinTime);
    mDescriptorMinTime.onValueChange = [this] {
        auto minVal{ mDescriptorMinTime.getValue() };
        auto maxVal{ mDescriptorMaxTime.getValue() };
        if (minVal > maxVal) {
            minVal = maxVal - ALMOST_ZERO;
            mDescriptorMinTime.setValue(minVal);
        }
        if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParameterMinTime(minVal);
            mAudioProcessor.setOnsetDetectionMinTime(mParameter.getParameterID(), minVal);
        }
        };

    mDescriptorMaxTime.setRange(ALMOST_ZERO, 30.0);
    mDescriptorMaxTime.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMaxTime.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMaxTime);
    mDescriptorMaxTime.onValueChange = [this] {
        auto minVal{ mDescriptorMinTime.getValue() };
        auto maxVal{ mDescriptorMaxTime.getValue() };
        if (maxVal < minVal) {
            maxVal = minVal + ALMOST_ZERO;
            mDescriptorMaxTime.setValue(maxVal);
        }
        if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParameterMaxTime(maxVal);
            mAudioProcessor.setOnsetDetectionMaxTime(mParameter.getParameterID(), maxVal);
        }
        };
}

PanelView::~PanelView()
{
    //mDescriptor = nullptr;
}

void PanelView::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &mDescriptorComboBox) {
        if (mDescriptorComboBox.getSelectedId() == 1) {

            mDescriptorFactor.setVisible(false);
            mDescriptorFactorLabel.setVisible(false);

            mDescriptorLap.setVisible(false);
            mDescriptorLapLabel.setVisible(false);

            mDescriptorOffset.setVisible(false);
            mDescriptorOffsetLabel.setVisible(false);

            mDescriptorSmooth.setVisible(false);
            mDescriptorSmoothLabel.setVisible(false);

            mDescriptorMoreSmooth.setVisible(false);
            mDescriptorMoreSmoothLabel.setVisible(false);

            mDescriptorRange.setVisible(false);
            mDescriptorRangeLabel.setVisible(false);

            mDescriptorMinFreq.setVisible(false);
            mDescriptorMinFreqLabel.setVisible(false);

            mDescriptorMaxFreq.setVisible(false);
            mDescriptorMaxFreqLabel.setVisible(false);

            mDescriptorMetricComboBox.setVisible(false);

            mDescriptorThreshold.setVisible(false);
            mDescriptorThresholdLabel.setVisible(false);

            mDescriptorMinTime.setVisible(false);
            mDescriptorMinTimeLabel.setVisible(false);

            mDescriptorMaxTime.setVisible(false);
            mDescriptorMaxTimeLabel.setVisible(false);
        }
        else if (mDescriptorComboBox.getSelectedId() != 1) {
            if (isAzimuth) {
                mDescriptorLap.setVisible(true);
                mDescriptorLapLabel.setVisible(true);
            }

            if (isOffset) {
                mDescriptorOffset.setVisible(true);
                mDescriptorOffsetLabel.setVisible(true);
            }

            mDescriptorFactor.setVisible(true);
            mDescriptorFactorLabel.setVisible(true);

            mDescriptorSmooth.setVisible(true);
            mDescriptorSmoothLabel.setVisible(true);

            mDescriptorMoreSmooth.setVisible(true);
            mDescriptorMoreSmoothLabel.setVisible(true);

            mDescriptorRange.setVisible(true);
            mDescriptorRangeLabel.setVisible(true);

            mDescriptorMetricComboBox.setVisible(false);

            mDescriptorThreshold.setVisible(false);
            mDescriptorThresholdLabel.setVisible(false);

            mDescriptorMinTime.setVisible(false);
            mDescriptorMinTimeLabel.setVisible(false);

            mDescriptorMaxTime.setVisible(false);
            mDescriptorMaxTimeLabel.setVisible(false);

            if (mDescriptorComboBox.getSelectedId() == 3 || mDescriptorComboBox.getSelectedId() == 4) {
                mDescriptorMinFreq.setVisible(true);
                mDescriptorMinFreqLabel.setVisible(true);

                mDescriptorMaxFreq.setVisible(true);
                mDescriptorMaxFreqLabel.setVisible(true);

                mDescriptorFactor.setVisible(false);
                mDescriptorFactorLabel.setVisible(false);
            }
            else if (mDescriptorComboBox.getSelectedId() == 7) {
                mDescriptorMetricComboBox.setVisible(true);

                mDescriptorThreshold.setVisible(true);
                mDescriptorThresholdLabel.setVisible(true);

                mDescriptorMinTime.setVisible(true);
                mDescriptorMinTimeLabel.setVisible(true);

                mDescriptorMaxTime.setVisible(true);
                mDescriptorMaxTimeLabel.setVisible(true);

                mDescriptorFactor.setVisible(false);
                mDescriptorFactorLabel.setVisible(false);

                mDescriptorMinFreq.setVisible(false);
                mDescriptorMinFreqLabel.setVisible(false);

                mDescriptorMaxFreq.setVisible(false);
                mDescriptorMaxFreqLabel.setVisible(false);
            }
            else {
                mDescriptorMinFreq.setVisible(false);
                mDescriptorMinFreqLabel.setVisible(false);

                mDescriptorMaxFreq.setVisible(false);
                mDescriptorMaxFreqLabel.setVisible(false);
            }
        }
    }
}

void PanelView::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::white);
}

void PanelView::resized()
{
    auto area = getLocalBounds().reduced(10);
    const juce::FlexItem::Margin sliderMargin(0, 200, 0, 0);
    juce::FlexItem::Margin margin(0, 10, 0, 0);
    juce::FlexBox generalFlexBoxConfig;
    generalFlexBoxConfig.flexDirection = juce::FlexBox::Direction::row;
    generalFlexBoxConfig.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    mDescriptorFactorBox = generalFlexBoxConfig;
    mDescriptorFactorBox.items.add(juce::FlexItem(mDescriptorFactorLabel).withMinWidth(10).withFlex(1));
    mDescriptorFactorBox.items.add(juce::FlexItem(mDescriptorFactor).withFlex(3).withMargin(sliderMargin));

    if (isAzimuth) {
        mDescriptorLapBox = generalFlexBoxConfig;
        mDescriptorLapBox.items.add(juce::FlexItem(mDescriptorLapLabel).withMinWidth(10).withFlex(1));
        mDescriptorLapBox.items.add(juce::FlexItem(mDescriptorLap).withFlex(3).withMargin(sliderMargin));
    }


    if (isOffset) {
        mDescriptorOffsetBox = generalFlexBoxConfig;
        mDescriptorOffsetBox.items.add(juce::FlexItem(mDescriptorOffsetLabel).withMinWidth(10).withFlex(1));
        mDescriptorOffsetBox.items.add(juce::FlexItem(mDescriptorOffset).withFlex(3).withMargin(sliderMargin));
    }

    mDescriptorSmoothBox = generalFlexBoxConfig;
    mDescriptorSmoothBox.items.add(juce::FlexItem(mDescriptorSmoothLabel).withMinWidth(10).withFlex(1));
    mDescriptorSmoothBox.items.add(juce::FlexItem(mDescriptorSmooth).withFlex(3).withMargin(sliderMargin));

    mDescriptorMoreSmoothBox = generalFlexBoxConfig;
    mDescriptorMoreSmoothBox.items.add(juce::FlexItem(mDescriptorMoreSmoothLabel).withMinWidth(10).withFlex(1));
    mDescriptorMoreSmoothBox.items.add(juce::FlexItem(mDescriptorMoreSmooth).withFlex(3).withMargin(sliderMargin));

    mDescriptorRangeBox = generalFlexBoxConfig;
    mDescriptorRangeBox.items.add(juce::FlexItem(mDescriptorRangeLabel).withMinWidth(10).withFlex(1));
    mDescriptorRangeBox.items.add(juce::FlexItem(mDescriptorRange).withFlex(3).withMargin(sliderMargin));

    mDescriptorMinFreqBox = generalFlexBoxConfig;
    mDescriptorMinFreqBox.items.add(juce::FlexItem(mDescriptorMinFreqLabel).withMinWidth(10).withFlex(1));
    mDescriptorMinFreqBox.items.add(juce::FlexItem(mDescriptorMinFreq).withFlex(3).withMargin(sliderMargin));

    mDescriptorMaxFreqBox = generalFlexBoxConfig;
    mDescriptorMaxFreqBox.items.add(juce::FlexItem(mDescriptorMaxFreqLabel).withMinWidth(10).withFlex(1));
    mDescriptorMaxFreqBox.items.add(juce::FlexItem(mDescriptorMaxFreq).withFlex(3).withMargin(sliderMargin));

    mDescriptorMetricBox = generalFlexBoxConfig;
    //azimuthMetricBox.items.add(juce::FlexItem(mAzimuthMetricLabel).withMinWidth(10).withFlex(1));
    mDescriptorMetricBox.items.add(juce::FlexItem(mDescriptorMetricComboBox).withMinHeight(20).withFlex(3).withMargin(sliderMargin));

    mDescriptorThresholdBox = generalFlexBoxConfig;
    mDescriptorThresholdBox.items.add(juce::FlexItem(mDescriptorThresholdLabel).withMinWidth(10).withFlex(1));
    mDescriptorThresholdBox.items.add(juce::FlexItem(mDescriptorThreshold).withFlex(3).withMargin(sliderMargin));

    mDescriptorMinTimeBox = generalFlexBoxConfig;
    mDescriptorMinTimeBox.items.add(juce::FlexItem(mDescriptorMinTimeLabel).withMinWidth(10).withFlex(1));
    mDescriptorMinTimeBox.items.add(juce::FlexItem(mDescriptorMinTime).withFlex(3).withMargin(sliderMargin));

    mDescriptorMaxTimeBox = generalFlexBoxConfig;
    mDescriptorMaxTimeBox.items.add(juce::FlexItem(mDescriptorMaxTimeLabel).withMinWidth(10).withFlex(1));
    mDescriptorMaxTimeBox.items.add(juce::FlexItem(mDescriptorMaxTime).withFlex(3).withMargin(sliderMargin));

    juce::FlexBox descriptorFlexBox;
    descriptorFlexBox.flexDirection = juce::FlexBox::Direction::column;
    descriptorFlexBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    descriptorFlexBox.alignItems = juce::FlexBox::AlignItems::stretch;
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorLabel).withHeight(50).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorComboBox).withHeight(40).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorFactorBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorSmoothBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorMoreSmoothBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    if (isAzimuth) {
        descriptorFlexBox.items.add(juce::FlexItem(mDescriptorLapBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    }
    if (isOffset) {
        descriptorFlexBox.items.add(juce::FlexItem(mDescriptorOffsetBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    }
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorRangeBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorMinFreqBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorMaxFreqBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorMetricBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorThresholdBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorMinTimeBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBox.items.add(juce::FlexItem(mDescriptorMaxTimeBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));

    //////////
    juce::FlexBox parentFlexBox;
    parentFlexBox.flexDirection = juce::FlexBox::Direction::row; // Disposition horizontale
    parentFlexBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parentFlexBox.alignItems = juce::FlexBox::AlignItems::stretch;
    parentFlexBox.items.add(juce::FlexItem(descriptorFlexBox).withFlex(1));
    parentFlexBox.performLayout(area); // Utilise topHalf pour le layout
}
