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

#include "algorithms/public/RunningStats.hpp"
#include "algorithms/public/MultiStats.hpp"
#include "algorithms/public/MelBands.hpp"
#include "algorithms/public/DCT.hpp"
#include "algorithms/public/Loudness.hpp"
#include "algorithms/public/OnsetDetectionFunctions.hpp"
#include "algorithms/public/YINFFT.hpp"
#include "algorithms/public/SpectralShape.hpp"
#include "algorithms/public/STFT.hpp"

#include "Eigen/src/Core/Transpose.h"
#include "Eigen/src/Core/ArrayBase.h"
#include "Eigen/src/Core/AssignEvaluator.h"

