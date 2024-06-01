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

// Globals
const double ALMOST_ZERO{ 0.0000000000001 };

const int indexFactorLoudness = 0;
const int indexFactorSpread = 1;
const int indexFactorNoise = 2;

const int indexSmoothAndRangeLoudness = 0;
const int indexSmoothAndRangePitch = 1;
const int indexSmoothAndRangeCentroid = 2;
const int indexSmoothAndRangeSpread = 3;
const int indexSmoothAndRangeNoise = 4;
const int indexSmoothAndRangeOnsetDetection = 5;

const int indexLapLoudness = 0;
const int indexLapPitch = 1;
const int indexLapCentroid = 2;
const int indexLapSpread = 3;
const int indexLapNoise = 4;
const int indexLapOnsetDetection = 5;

const int indexOffsetLoudness = 0;
const int indexOffsetPitch = 1;
const int indexOffsetCentroid = 2;
const int indexOffsetSpread = 3;
const int indexOffsetNoise = 4;
const int indexOffsetOnsetDetection = 5;


const int indexMinFreqPitch = 0;
const int indexMinFreqCentroid = 1;

const int indexMaxFreqPitch = 0;
const int indexMaxFreqCentroid = 1;
