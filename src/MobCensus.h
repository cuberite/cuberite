/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include "MobProximityCounter.h"
#include "MobFamilyCollecter.h"




// fwd:
class cChunk;
class cMonster;





/** This class is used to collect information, for each Mob, what is the distance of the closest player
it was first being designed in order to make mobs spawn / despawn / act
as the behaviour and even life of mobs depends on the distance to closest player

as side effect : it also collect the chunks that are elligible for spawning
as side effect 2 : it also know the caps for mobs number and can compare census to this numbers
*/
class cMobCensus
{
public:
	/// Returns the nested proximity counter
	cMobProximityCounter & GetProximityCounter(void);

	// collect an elligible Chunk for Mob Spawning
	// MG TODO : code the correct rule (not loaded chunk but short distant from players)
	void CollectSpawnableChunk(cChunk & a_Chunk);
	
	/// Collect a mob - it's distance to player, it's family ...
	void CollectMob(cMonster& a_Monster, cChunk& a_Chunk, double a_Distance);

	/// Returns true if the family is capped (i.e. there are more mobs of this family than max)
	bool IsCapped(cMonster::eFamily a_MobFamily);
	
	/// log the results of census to server console
	void Logd(void);
	
protected :
	cMobProximityCounter m_ProximityCounter;
	cMobFamilyCollecter m_MobFamilyCollecter;

	std::set<cChunk *> m_EligibleForSpawnChunks;

	/// Returns the number of chunks that are elligible for spawning (for now, the loaded, valid chunks)
	int GetNumChunks();

	/// Returns the cap multiplier value of the given monster family
	static int GetCapMultiplier(cMonster::eFamily a_MobFamily);
} ;




