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

#include "algorithms/public/MultiStats.hpp"
#include "Descriptors.hpp"

class StatsD : public Descriptors
{
public:

	void resetStats() {
		mStats.reset(new fluid::algorithm::MultiStats());
	}

	void initStats() {
		mStats->init(0, 0, 50, 100);
	}

	fluid::algorithm::MultiStats* getStats() const {
		return mStats.get();
	}


private:
	std::unique_ptr<fluid::algorithm::MultiStats>     mStats;

	//==============================================================================
	JUCE_LEAK_DETECTOR(StatsD)
};
