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
    , mDataGraph(parameter)
{
    mParameter.addObserver(this);

    setSize(1000, 1000);

    // default values
    mDescriptorFactorSlider.setDoubleClickReturnValue(true, 100.0);
    mDescriptorSmoothSlider.setDoubleClickReturnValue(true, 5.0);
    mDescriptorSmoothCoefSlider.setDoubleClickReturnValue(true, 0.0);
    mDescriptorRangeSlider.setDoubleClickReturnValue(true, 100.0);
    mDescriptorMinFreqSlider.setDoubleClickReturnValue(true, 20.0);
    mDescriptorMaxFreqSlider.setDoubleClickReturnValue(true, 10000.0);
    mDescriptorThresholdSlider.setDoubleClickReturnValue(true, 0.1);
    mDescriptorMinTimeSlider.setDoubleClickReturnValue(true, 0.1);
    mDescriptorMaxTimeSlider.setDoubleClickReturnValue(true, 10.0);
    mDescriptorLapSlider.setDoubleClickReturnValue(true, 1.0);
    mDescriptorOffsetSlider.setDoubleClickReturnValue(true, 0.0);


    if (parameter.getParameterID() == ParameterID::azimuth) {
        addAndMakeVisible(mDescriptorLapLabel);
        mDescriptorLapLabel.setText("Lap", juce::dontSendNotification);
        mDescriptorLapSlider.setRange(1, 4, 1);
        mDescriptorLapSlider.setSliderStyle(juce::Slider::LinearHorizontal);
        mDescriptorLapSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
        addAndMakeVisible(mDescriptorLapSlider);
        isAzimuth = true;
    }

    if (parameter.getParameterID() == ParameterID::z ||
        parameter.getParameterID() == ParameterID::elevation ||
        parameter.getParameterID() == ParameterID::elevationspan) {
        addAndMakeVisible(mDescriptorOffsetLabel);
        mDescriptorOffsetLabel.setText("Offset", juce::dontSendNotification);
        if (parameter.getParameterID() == ParameterID::elevation) {
            mDescriptorOffsetSlider.setRange(0, 90);
        }
        else {
            mDescriptorOffsetSlider.setRange(0, 1);
        }
        mDescriptorOffsetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
        mDescriptorOffsetSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
        addAndMakeVisible(mDescriptorOffsetSlider);
        isOffset = true;
    }

    addAndMakeVisible(mClickTimerButton);
    mClickTimerButton.setButtonText("click me!");
    mClickTimerButton.onClick = [this] {
        mAudioProcessor.setOnsetDetectionFromClick(mParameter.getParameterID(), static_cast<double>(mOnsetDetectiontimerCounter));
        mOnsetDetectiontimerCounter = 0;
        startTimer(5);
    };

    addAndMakeVisible(mDescriptorLabel);
    mDescriptorLabel.setText(mParameter.getParameterName(), juce::dontSendNotification);

    addAndMakeVisible(mDescriptorFactorLabel);
    mDescriptorFactorLabel.setText("Factor", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorSmoothLabel);
    mDescriptorSmoothLabel.setText("Smooth", juce::dontSendNotification);

    addAndMakeVisible(mDescriptorSmoothCoefLabel);
    mDescriptorSmoothCoefLabel.setText("Smooth Coef.", juce::dontSendNotification);

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
    mDescriptorMetricComboBox.setSelectedId(parameter.getParamMetricComboBoxIndex());

    mDescriptorMetricComboBox.addListener(this);
    mDescriptorMetricComboBox.onChange = [this, &parameter] {
        parameter.setParamMetricComboboxIndex(mDescriptorMetricComboBox.getSelectedId());
        mAudioProcessor.setOnsetDetectionMetric(mParameter.getParameterID(), mDescriptorMetricComboBox.getSelectedId());
        };

    mDescriptorComboBox.addItemList(boxItems, 1);
    addAndMakeVisible(mDescriptorComboBox);
    mDescriptorComboBox.setSelectedId(mParameter.getParamDescriptorComboBoxIndex());
    mDescriptorComboBox.addListener(this);
    mDescriptorComboBox.onChange = [this] {
        mParameter.setParamDescriptorComboBoxIndex(mDescriptorComboBox.getSelectedId());
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mDescriptorFactorSlider.setValue(mParameter.getParamFactorLoudness());
            mDescriptorSmoothSlider.setValue(mParameter.getParamSmoothLoudness());
            mDescriptorRangeSlider.setValue(mParameter.getParamRangeLoudness());
            mDescriptorSmoothCoefSlider.setValue(mParameter.getParamSmoothCoefLoudness());
            if (isAzimuth) {
                mDescriptorLapSlider.setValue(mParameter.getParamLapLoudness());
            }
            if (isOffset) {
                mDescriptorOffsetSlider.setValue(mParameter.getParamOffsetLoudness());
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 3) {
            mDescriptorSmoothSlider.setValue(mParameter.getParamSmoothPitch());
            mDescriptorRangeSlider.setValue(mParameter.getParamRangePitch());
            mDescriptorMinFreqSlider.setValue(mParameter.getParamMinFreqPitch());
            mDescriptorMaxFreqSlider.setValue(mParameter.getParamMaxFreqPitch());
            mDescriptorSmoothCoefSlider.setValue(mParameter.getParamSmoothCoefPitch());
            if (isAzimuth) {
                mDescriptorLapSlider.setValue(mParameter.getParamLapPitch());
            }
            if (isOffset) {
                mDescriptorOffsetSlider.setValue(mParameter.getParamOffsetPitch());
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mDescriptorSmoothSlider.setValue(mParameter.getParamSmoothCentroid());
            mDescriptorRangeSlider.setValue(mParameter.getParamRangeCentroid());
            mDescriptorMinFreqSlider.setValue(mParameter.getParamMinFreqCentroid());
            mDescriptorMaxFreqSlider.setValue(mParameter.getParamMaxFreqCentroid());
            mDescriptorSmoothCoefSlider.setValue(mParameter.getParamSmoothCoefCentroid());
            if (isAzimuth) {
                mDescriptorLapSlider.setValue(mParameter.getParamLapCentroid());
            }
            if (isOffset) {
                mDescriptorOffsetSlider.setValue(mParameter.getParamOffsetCentroid());
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mDescriptorFactorSlider.setValue(mParameter.getParamFactorSpread());
            mDescriptorSmoothSlider.setValue(mParameter.getParamSmoothSpread());
            mDescriptorRangeSlider.setValue(mParameter.getParamRangeSpread());
            mDescriptorSmoothCoefSlider.setValue(mParameter.getParamSmoothCoefSpread());
            if (isAzimuth) {
                mDescriptorLapSlider.setValue(mParameter.getParamLapSpread());
            }
            if (isOffset) {
                mDescriptorOffsetSlider.setValue(mParameter.getParamOffsetSpread());
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mDescriptorFactorSlider.setValue(mParameter.getParamFactorNoise());
            mDescriptorSmoothSlider.setValue(mParameter.getParamSmoothNoise());
            mDescriptorRangeSlider.setValue(mParameter.getParamRangeNoise());
            mDescriptorSmoothCoefSlider.setValue(mParameter.getParamSmoothCoefNoise());
            if (isAzimuth) {
                mDescriptorLapSlider.setValue(mParameter.getParamLapNoise());
            }
            if (isOffset) {
                mDescriptorOffsetSlider.setValue(mParameter.getParamOffsetNoise());
            }
        }
        else if (mDescriptorComboBox.getSelectedId() == 7) {
            mDescriptorMetricComboBox.setSelectedId(mParameter.getParamMetricComboBoxIndex());
            mDescriptorThresholdSlider.setValue(mParameter.getParamThreshold());
            mDescriptorMinTimeSlider.setValue(mParameter.getParamMinTime());
            mDescriptorMaxTimeSlider.setValue(mParameter.getParamMaxTime());
            mDescriptorSmoothSlider.setValue(mParameter.getParamSmoothOnsetDetection());
            mDescriptorRangeSlider.setValue(mParameter.getParamRangeOnsetDetection());
            mDescriptorSmoothCoefSlider.setValue(mParameter.getParamSmoothCoefOnsetDetection());
            if (isAzimuth) {
                mDescriptorLapSlider.setValue(mParameter.getParamLapOnsetDetection());
            }
            if (isOffset) {
                mDescriptorOffsetSlider.setValue(mParameter.getParamOffsetOnsetDetection());
            }
        }
        };

    mDescriptorFactorSlider.setRange(0, 500);
    mDescriptorFactorSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorFactorSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorFactorSlider);
    mDescriptorFactorSlider.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mParameter.setParamFactorLoudness(mDescriptorFactorSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mParameter.setParamFactorSpread(mDescriptorFactorSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mParameter.setParamFactorNoise(mDescriptorFactorSlider.getValue());
        }
        };

    if (isAzimuth) {
        mDescriptorLapSlider.onValueChange = [this] {
            if (mDescriptorComboBox.getSelectedId() == 2) {
                mParameter.setParamLapLoudness(mDescriptorLapSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 3) {
                mParameter.setParamLapPitch(mDescriptorLapSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 4) {
                mParameter.setParamLapCentroid(mDescriptorLapSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 5) {
                mParameter.setParamLapSpread(mDescriptorLapSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 6) {
                mParameter.setParamLapNoise(mDescriptorLapSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 7) {
                mParameter.setParamLapOnsetDetection(mDescriptorLapSlider.getValue());
            }
        };
    }

    if (isOffset) {
        mDescriptorOffsetSlider.onValueChange = [this] {
            if (mDescriptorComboBox.getSelectedId() == 2) {
                mParameter.setParamOffsetLoudness(mDescriptorOffsetSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 3) {
                mParameter.setParamOffsetPitch(mDescriptorOffsetSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 4) {
                mParameter.setParamOffsetCentroid(mDescriptorOffsetSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 5) {
                mParameter.setParamOffsetSpread(mDescriptorOffsetSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 6) {
                mParameter.setParamOffsetNoise(mDescriptorOffsetSlider.getValue());
            }
            else if (mDescriptorComboBox.getSelectedId() == 7) {
                mParameter.setParamOffsetOnsetDetection(mDescriptorOffsetSlider.getValue());
            }
        };
    }

    mDescriptorSmoothSlider.setRange(0, 100);
    mDescriptorSmoothSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorSmoothSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorSmoothSlider);
    mDescriptorSmoothSlider.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mParameter.setParamSmoothLoudness(mDescriptorSmoothSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamSmoothPitch(mDescriptorSmoothSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamSmoothCentroid(mDescriptorSmoothSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mParameter.setParamSmoothSpread(mDescriptorSmoothSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mParameter.setParamSmoothNoise(mDescriptorSmoothSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParamSmoothOnsetDetection(mDescriptorSmoothSlider.getValue());
        }
        };

    mDescriptorSmoothCoefSlider.setRange(0, 100);
    mDescriptorSmoothCoefSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorSmoothCoefSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorSmoothCoefSlider);
    mDescriptorSmoothCoefSlider.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mParameter.setParamSmoothCoefLoudness(mDescriptorSmoothCoefSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamSmoothCoefPitch(mDescriptorSmoothCoefSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamSmoothCoefCentroid(mDescriptorSmoothCoefSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mParameter.setParamSmoothCoefSpread(mDescriptorSmoothCoefSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mParameter.setParamSmoothCoefNoise(mDescriptorSmoothCoefSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParamSmoothCoefOnsetDetection(mDescriptorSmoothCoefSlider.getValue());
        }
    };

    mDescriptorRangeSlider.setRange(-100, 100);
    mDescriptorRangeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorRangeSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorRangeSlider);
    mDescriptorRangeSlider.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 2) {
            mParameter.setParamRangeLoudness(mDescriptorRangeSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamRangePitch(mDescriptorRangeSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamRangeCentroid(mDescriptorRangeSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 5) {
            mParameter.setParamRangeSpread(mDescriptorRangeSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 6) {
            mParameter.setParamRangeNoise(mDescriptorRangeSlider.getValue());
        }
        else if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParamRangeOnsetDetection(mDescriptorRangeSlider.getValue());
        }
        };

    mDescriptorMinFreqSlider.setRange(20, 20000);
    mDescriptorMinFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMinFreqSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMinFreqSlider);
    mDescriptorMinFreqSlider.onValueChange = [this] {
        auto minVal{ mDescriptorMinFreqSlider.getValue() };
        auto maxVal{ mDescriptorMaxFreqSlider.getValue() };
        if (minVal > maxVal) {
            minVal = maxVal;
            mDescriptorMinFreqSlider.setValue(minVal);
        }
        if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamMinFreqPitch(minVal);
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamMinFreqCentroid(minVal);
        }
        };

    mDescriptorMaxFreqSlider.setRange(20, 20000);
    mDescriptorMaxFreqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMaxFreqSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMaxFreqSlider);
    mDescriptorMaxFreqSlider.onValueChange = [this] {
        auto minVal{ mDescriptorMinFreqSlider.getValue() };
        auto maxVal{ mDescriptorMaxFreqSlider.getValue() };
        if (maxVal < minVal) {
            maxVal = minVal;
            mDescriptorMaxFreqSlider.setValue(maxVal);
        }
        if (mDescriptorComboBox.getSelectedId() == 3) {
            mParameter.setParamMaxFreqPitch(maxVal);
        }
        else if (mDescriptorComboBox.getSelectedId() == 4) {
            mParameter.setParamMaxFreqCentroid(maxVal);
        }
        };

    mDescriptorThresholdSlider.setRange(0.0, 1.0);
    mDescriptorThresholdSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorThresholdSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorThresholdSlider);
    mDescriptorThresholdSlider.onValueChange = [this] {
        if (mDescriptorComboBox.getSelectedId() == 7) {
            auto newVal{ mDescriptorThresholdSlider.getValue() };
            mParameter.setParamThreshold(newVal);
            mAudioProcessor.setOnsetDetectionThreshold(mParameter.getParameterID(), static_cast<float>(newVal));
        }
        };

    mDescriptorMinTimeSlider.setRange(0.0, 30.0 + ALMOST_ZERO);
    mDescriptorMinTimeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMinTimeSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMinTimeSlider);
    mDescriptorMinTimeSlider.onValueChange = [this] {
        auto minVal{ mDescriptorMinTimeSlider.getValue() };
        auto maxVal{ mDescriptorMaxTimeSlider.getValue() };
        if (minVal > maxVal) {
            minVal = maxVal - ALMOST_ZERO;
            mDescriptorMinTimeSlider.setValue(minVal);
        }
        if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParamMinTime(minVal);
            mAudioProcessor.setOnsetDetectionMinTime(mParameter.getParameterID(), minVal);
        }
        };

    mDescriptorMaxTimeSlider.setRange(ALMOST_ZERO, 30.0);
    mDescriptorMaxTimeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mDescriptorMaxTimeSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    addAndMakeVisible(mDescriptorMaxTimeSlider);
    mDescriptorMaxTimeSlider.onValueChange = [this] {
        auto minVal{ mDescriptorMinTimeSlider.getValue() };
        auto maxVal{ mDescriptorMaxTimeSlider.getValue() };
        if (maxVal < minVal) {
            maxVal = minVal + ALMOST_ZERO;
            mDescriptorMaxTimeSlider.setValue(maxVal);
        }
        if (mDescriptorComboBox.getSelectedId() == 7) {
            mParameter.setParamMaxTime(maxVal);
            mAudioProcessor.setOnsetDetectionMaxTime(mParameter.getParameterID(), maxVal);
        }
        };

    addAndMakeVisible(mDataGraph);
}

PanelView::~PanelView()
{
    mParameter.removeObserver(this);
}

void PanelView::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &mDescriptorComboBox) {
        if (mDescriptorComboBox.getSelectedId() == 1) {

            mDescriptorFactorSlider.setVisible(false);
            mDescriptorFactorLabel.setVisible(false);

            mDescriptorLapSlider.setVisible(false);
            mDescriptorLapLabel.setVisible(false);

            mDescriptorOffsetSlider.setVisible(false);
            mDescriptorOffsetLabel.setVisible(false);

            mDescriptorSmoothSlider.setVisible(false);
            mDescriptorSmoothLabel.setVisible(false);

            mDescriptorSmoothCoefSlider.setVisible(false);
            mDescriptorSmoothCoefLabel.setVisible(false);

            mDescriptorRangeSlider.setVisible(false);
            mDescriptorRangeLabel.setVisible(false);

            mDescriptorMinFreqSlider.setVisible(false);
            mDescriptorMinFreqLabel.setVisible(false);

            mDescriptorMaxFreqSlider.setVisible(false);
            mDescriptorMaxFreqLabel.setVisible(false);

            mDescriptorMetricComboBox.setVisible(false);

            mDescriptorThresholdSlider.setVisible(false);
            mDescriptorThresholdLabel.setVisible(false);

            mDescriptorMinTimeSlider.setVisible(false);
            mDescriptorMinTimeLabel.setVisible(false);

            mDescriptorMaxTimeSlider.setVisible(false);
            mDescriptorMaxTimeLabel.setVisible(false);

            mDataGraph.setVisible(false);
            mClickTimerButton.setVisible(false);
        }
        else if (mDescriptorComboBox.getSelectedId() != 1) {
            mDataGraph.setVisible(true);
            mClickTimerButton.setVisible(false);

            if (isAzimuth) {
                mDescriptorLapSlider.setVisible(true);
                mDescriptorLapLabel.setVisible(true);
            }

            if (isOffset) {
                mDescriptorOffsetSlider.setVisible(true);
                mDescriptorOffsetLabel.setVisible(true);
            }

            mDescriptorFactorSlider.setVisible(true);
            mDescriptorFactorLabel.setVisible(true);

            mDescriptorSmoothSlider.setVisible(true);
            mDescriptorSmoothLabel.setVisible(true);

            mDescriptorSmoothCoefSlider.setVisible(true);
            mDescriptorSmoothCoefLabel.setVisible(true);

            mDescriptorRangeSlider.setVisible(true);
            mDescriptorRangeLabel.setVisible(true);

            mDescriptorMetricComboBox.setVisible(false);

            mDescriptorThresholdSlider.setVisible(false);
            mDescriptorThresholdLabel.setVisible(false);

            mDescriptorMinTimeSlider.setVisible(false);
            mDescriptorMinTimeLabel.setVisible(false);

            mDescriptorMaxTimeSlider.setVisible(false);
            mDescriptorMaxTimeLabel.setVisible(false);

            if (mDescriptorComboBox.getSelectedId() == 3 || mDescriptorComboBox.getSelectedId() == 4) {
                mDescriptorMinFreqSlider.setVisible(true);
                mDescriptorMinFreqLabel.setVisible(true);

                mDescriptorMaxFreqSlider.setVisible(true);
                mDescriptorMaxFreqLabel.setVisible(true);

                mDescriptorFactorSlider.setVisible(false);
                mDescriptorFactorLabel.setVisible(false);
            }
            else if (mDescriptorComboBox.getSelectedId() == 7) {
                mDescriptorMetricComboBox.setVisible(true);

                mDescriptorThresholdSlider.setVisible(true);
                mDescriptorThresholdLabel.setVisible(true);

                mDescriptorMinTimeSlider.setVisible(true);
                mDescriptorMinTimeLabel.setVisible(true);

                mDescriptorMaxTimeSlider.setVisible(true);
                mDescriptorMaxTimeLabel.setVisible(true);

                mDescriptorFactorSlider.setVisible(false);
                mDescriptorFactorLabel.setVisible(false);

                mDescriptorMinFreqSlider.setVisible(false);
                mDescriptorMinFreqLabel.setVisible(false);

                mDescriptorMaxFreqSlider.setVisible(false);
                mDescriptorMaxFreqLabel.setVisible(false);

                mClickTimerButton.setVisible(true);
            }
            else {
                mDescriptorMinFreqSlider.setVisible(false);
                mDescriptorMinFreqLabel.setVisible(false);

                mDescriptorMaxFreqSlider.setVisible(false);
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

    juce::FlexItem::Margin margin(0, 10, 0, 0);
    juce::FlexBox generalFlexBoxConfig;
    generalFlexBoxConfig.flexDirection = juce::FlexBox::Direction::row;
    generalFlexBoxConfig.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    mDescriptorFactorBox = generalFlexBoxConfig;
    mDescriptorFactorBox.items.add(juce::FlexItem(mDescriptorFactorLabel).withMinWidth(10).withFlex(1));
    mDescriptorFactorBox.items.add(juce::FlexItem(mDescriptorFactorSlider).withFlex(3));

    if (isAzimuth) {
        mDescriptorLapBox = generalFlexBoxConfig;
        mDescriptorLapBox.items.add(juce::FlexItem(mDescriptorLapLabel).withMinWidth(10).withFlex(1));
        mDescriptorLapBox.items.add(juce::FlexItem(mDescriptorLapSlider).withFlex(3));
    }

    if (isOffset) {
        mDescriptorOffsetBox = generalFlexBoxConfig;
        mDescriptorOffsetBox.items.add(juce::FlexItem(mDescriptorOffsetLabel).withMinWidth(10).withFlex(1));
        mDescriptorOffsetBox.items.add(juce::FlexItem(mDescriptorOffsetSlider).withFlex(3));
    }

    mDescriptorSmoothBox = generalFlexBoxConfig;
    mDescriptorSmoothBox.items.add(juce::FlexItem(mDescriptorSmoothLabel).withMinWidth(10).withFlex(1));
    mDescriptorSmoothBox.items.add(juce::FlexItem(mDescriptorSmoothSlider).withFlex(3));

    mDescriptorSmoothCoefBox = generalFlexBoxConfig;
    mDescriptorSmoothCoefBox.items.add(juce::FlexItem(mDescriptorSmoothCoefLabel).withMinWidth(10).withFlex(1));
    mDescriptorSmoothCoefBox.items.add(juce::FlexItem(mDescriptorSmoothCoefSlider).withFlex(3));

    mDescriptorRangeBox = generalFlexBoxConfig;
    mDescriptorRangeBox.items.add(juce::FlexItem(mDescriptorRangeLabel).withMinWidth(10).withFlex(1));
    mDescriptorRangeBox.items.add(juce::FlexItem(mDescriptorRangeSlider).withFlex(3));

    mDescriptorMinFreqBox = generalFlexBoxConfig;
    mDescriptorMinFreqBox.items.add(juce::FlexItem(mDescriptorMinFreqLabel).withMinWidth(10).withFlex(1));
    mDescriptorMinFreqBox.items.add(juce::FlexItem(mDescriptorMinFreqSlider).withFlex(3));

    mDescriptorMaxFreqBox = generalFlexBoxConfig;
    mDescriptorMaxFreqBox.items.add(juce::FlexItem(mDescriptorMaxFreqLabel).withMinWidth(10).withFlex(1));
    mDescriptorMaxFreqBox.items.add(juce::FlexItem(mDescriptorMaxFreqSlider).withFlex(3));

    mDescriptorMetricBox = generalFlexBoxConfig;
    mDescriptorMetricBox.items.add(juce::FlexItem(mDescriptorMetricComboBox).withMinHeight(20).withFlex(3));

    mDescriptorThresholdBox = generalFlexBoxConfig;
    mDescriptorThresholdBox.items.add(juce::FlexItem(mDescriptorThresholdLabel).withMinWidth(10).withFlex(1));
    mDescriptorThresholdBox.items.add(juce::FlexItem(mDescriptorThresholdSlider).withFlex(3));

    mDescriptorMinTimeBox = generalFlexBoxConfig;
    mDescriptorMinTimeBox.items.add(juce::FlexItem(mDescriptorMinTimeLabel).withMinWidth(10).withFlex(1));
    mDescriptorMinTimeBox.items.add(juce::FlexItem(mDescriptorMinTimeSlider).withFlex(3));

    mDescriptorMaxTimeBox = generalFlexBoxConfig;
    mDescriptorMaxTimeBox.items.add(juce::FlexItem(mDescriptorMaxTimeLabel).withMinWidth(10).withFlex(1));
    mDescriptorMaxTimeBox.items.add(juce::FlexItem(mDescriptorMaxTimeSlider).withFlex(3));

    mGraphBox = generalFlexBoxConfig;
    mGraphBox.items.add(juce::FlexItem(mDataGraph).withFlex(3));

    mClickTimerBox = generalFlexBoxConfig;
    mClickTimerBox.items.add(juce::FlexItem(mClickTimerButton).withFlex(1));

    juce::FlexBox descriptorFlexBoxTopPart;
    juce::FlexBox descriptorFlexBoxBottomPart;
    juce::FlexBox descriptorFlexBoxBottomLeftPart;
    juce::FlexBox descriptorFlexBoxBottomRightPart;

    descriptorFlexBoxTopPart.flexDirection = juce::FlexBox::Direction::column;
    descriptorFlexBoxTopPart.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    descriptorFlexBoxTopPart.alignItems = juce::FlexBox::AlignItems::stretch;

    descriptorFlexBoxBottomPart.flexDirection = juce::FlexBox::Direction::row;
    descriptorFlexBoxBottomPart.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    descriptorFlexBoxBottomPart.alignItems = juce::FlexBox::AlignItems::stretch;

    descriptorFlexBoxBottomLeftPart.flexDirection = juce::FlexBox::Direction::column;
    descriptorFlexBoxBottomLeftPart.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    descriptorFlexBoxBottomLeftPart.alignItems = juce::FlexBox::AlignItems::stretch;

    descriptorFlexBoxBottomRightPart.flexDirection = juce::FlexBox::Direction::column;
    descriptorFlexBoxBottomRightPart.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    descriptorFlexBoxBottomRightPart.alignItems = juce::FlexBox::AlignItems::stretch;

    descriptorFlexBoxTopPart.items.add(juce::FlexItem(mDescriptorLabel).withHeight(40).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxTopPart.items.add(juce::FlexItem(mDescriptorComboBox).withHeight(40).withMargin(juce::FlexItem::Margin(5)));

    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorFactorBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorSmoothBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorSmoothCoefBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    if (isAzimuth) {
        descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorLapBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    }
    if (isOffset) {
        descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorOffsetBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    }
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorRangeBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorMinFreqBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorMaxFreqBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorMetricBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorThresholdBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorMinTimeBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));
    descriptorFlexBoxBottomLeftPart.items.add(juce::FlexItem(mDescriptorMaxTimeBox).withHeight(30).withMargin(juce::FlexItem::Margin(5)));

    descriptorFlexBoxBottomRightPart.items.add(juce::FlexItem(mGraphBox)
        .withMinHeight(80)
        .withMinWidth(80)
        //.withMaxHeight(80)
        //.withMaxWidth(80)
        .withMargin(juce::FlexItem::Margin(5))
        .withAlignSelf(juce::FlexItem::AlignSelf::center));
    descriptorFlexBoxBottomRightPart.items.add(juce::FlexItem(mClickTimerButton)
        .withMinHeight(20)
        .withMinWidth(80)
        .withAlignSelf(juce::FlexItem::AlignSelf::center));

    //////////

    descriptorFlexBoxBottomPart.items.add(juce::FlexItem(descriptorFlexBoxBottomLeftPart).withFlex(3));
    descriptorFlexBoxBottomPart.items.add(juce::FlexItem(descriptorFlexBoxBottomRightPart).withFlex(1));

    juce::FlexBox parentFlexBox;
    parentFlexBox.flexDirection = juce::FlexBox::Direction::column; // Disposition horizontale
    parentFlexBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parentFlexBox.alignItems = juce::FlexBox::AlignItems::stretch;

    parentFlexBox.items.add(juce::FlexItem(descriptorFlexBoxTopPart).withFlex(1));
    parentFlexBox.items.add(juce::FlexItem(descriptorFlexBoxBottomPart).withFlex(4));
    parentFlexBox.performLayout(area); // Utilise topHalf pour le layout
}

void PanelView::timerCallback()
{
    mOnsetDetectiontimerCounter += getTimerInterval();

    if (mOnsetDetectiontimerCounter >= 1000 * 120) {
        stopTimer();
        mOnsetDetectiontimerCounter = 0;
        mClickTimerButton.setButtonText(juce::String("click me!"));
    }
    else {
        if (mOnsetDetectiontimerCounter < 1000) {
            mClickTimerButton.setButtonText(juce::String(mOnsetDetectiontimerCounter) + juce::String(" ms"));
        }
        else {
            auto buttonTimerValue = static_cast<double>(mOnsetDetectiontimerCounter) / 1000;
            auto formattedTimerValue = juce::String(buttonTimerValue, 2);
            mClickTimerButton.setButtonText(formattedTimerValue + juce::String(" s"));
        }
    }
}

void PanelView::addNewParamValue(double value)
{
    // normalize value
    auto lap = static_cast<int>(mDescriptorLapSlider.getValue());
    switch (mParameter.getParameterID())
    {
    case ParameterID::azimuth:
        value = juce::jmap(std::abs(value), 0.0, 360.0 * lap, 0.0, 1.0);
        break;
    case ParameterID::elevation:
        value = juce::jmap(std::abs(value), 0.0, 90.0, 0.0, 1.0);
        break;
    case ParameterID::x:
    case ParameterID::y:
        value = juce::jmap(std::abs(value), 0.0, 1.66, 0.0, 1.0);
        break;
    case ParameterID::z:
        value = juce::jmap(std::abs(value), 0.0, 1.0, 0.0, 1.0);
        break;
    case ParameterID::azimuthspan:
    case ParameterID::elevationspan:
        value = juce::jmap(std::abs(value), 0.0, 100.0, 0.0, 1.0);
        break;
    case ParameterID::invalid:
    default:
        break;
    }

    mDataGraph.addToBuffer(value);
}

//==============================================================================
DataGraph::DataGraph(Parameters& parameter)
    : param(parameter)
{
    mGUIBuffer.resize(100);
    std::fill(mGUIBuffer.begin(), mGUIBuffer.end(), 0.0);

    startTimer(50);
}

DataGraph::~DataGraph()
{
}

void DataGraph::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::yellow);

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::black);

    auto area = getLocalBounds().reduced(1);
    juce::RectangleList<float> rectList{};

    for (int i{}; i < mGUIBuffer.size(); ++i) {
        auto initialX = (static_cast<float>(area.getWidth()) / static_cast<float>(mGUIBuffer.size())) * (i + 1);
        auto initialY = static_cast<float>(area.getHeight() - (area.getHeight() * mGUIBuffer.at(i)) + 1);
        auto width = static_cast<float>(area.getWidth()) / mGUIBuffer.size();
        auto height = static_cast<float>(area.getHeight() * mGUIBuffer.at(i));
        rectList.add(initialX, initialY, width, height);
    }
    g.fillRectList(rectList);
}

void DataGraph::resized()
{
}

void DataGraph::timerCallback()
{
    if (isVisible()) {
        mGUIBuffer.push_back(readBufferMean());
        if (mGUIBuffer.size() > 100) {
            mGUIBuffer.pop_front();
        }
        repaint();
    }
}

void DataGraph::addToBuffer(double value)
{
    mBuffer += value;
    mBufferCount++;
}

double DataGraph::readBufferMean()
{
    double mean{};

    if (mBufferCount > 0) {
        mean = mBuffer / mBufferCount;
        mBuffer = 0.0;
        mBufferCount = 0;
    }

    return mean;
}
