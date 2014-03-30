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

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobCensus.h"





void cMobCensus::CollectMob(cMonster & a_Monster, cChunk & a_Chunk, double a_Distance)
{
	m_ProximityCounter.CollectMob(a_Monster, a_Chunk, a_Distance);
	m_MobFamilyCollecter.CollectMob(a_Monster);
}





bool cMobCensus::IsCapped(cMonster::eFamily a_MobFamily)
{
	const int ratio = 319; // this should be 256 as we are only supposed to take account from chunks that are in 17x17 from a player
	// but for now, we use all chunks loaded by players. that means 19 x 19 chunks. That's why we use 256 * (19*19) / (17*17) = 319
	// MG TODO : code the correct count	
	if ((GetCapMultiplier(a_MobFamily) * GetNumChunks()) / ratio >= m_MobFamilyCollecter.GetNumberOfCollectedMobs(a_MobFamily))
	{
		return false;
	}
	return true;
}





int cMobCensus::GetCapMultiplier(cMonster::eFamily a_MobFamily)
{
	switch (a_MobFamily)
	{
		case cMonster::mfHostile: return 79;
		case cMonster::mfPassive: return 11;
		case cMonster::mfAmbient: return 16;
		case cMonster::mfWater:   return 5;
		default:
		{
			ASSERT(!"Unhandled mob family");
			return -1;
		}
	}
}





void cMobCensus::CollectSpawnableChunk(cChunk & a_Chunk)
{
	m_EligibleForSpawnChunks.insert(&a_Chunk);
}





int cMobCensus::GetNumChunks(void)
{
	return m_EligibleForSpawnChunks.size();
}





cMobProximityCounter & cMobCensus::GetProximityCounter(void)
{
	return m_ProximityCounter;
}





void cMobCensus::Logd()
{
	LOGD("Hostile mobs : %d %s", m_MobFamilyCollecter.GetNumberOfCollectedMobs(cMonster::mfHostile), IsCapped(cMonster::mfHostile) ? "(capped)" : "");
	LOGD("Ambient mobs : %d %s", m_MobFamilyCollecter.GetNumberOfCollectedMobs(cMonster::mfAmbient), IsCapped(cMonster::mfAmbient) ? "(capped)" : "");
	LOGD("Water mobs   : %d %s", m_MobFamilyCollecter.GetNumberOfCollectedMobs(cMonster::mfWater),   IsCapped(cMonster::mfWater)   ? "(capped)" : "");
	LOGD("Passive mobs : %d %s", m_MobFamilyCollecter.GetNumberOfCollectedMobs(cMonster::mfPassive), IsCapped(cMonster::mfPassive) ? "(capped)" : "");
}





