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

#include "MobProximityCounter.h"

#include "Entities/Entity.h"
#include "Chunk.h"

void cMobProximityCounter::CollectMob(cEntity& a_Monster, cChunk& a_Chunk, double a_Distance)
{
//	LOGD("Collecting monster %s, with distance %f",a_Monster->GetClass(),a_Distance);
	tMonsterToDistance::iterator it = m_MonsterToDistance.find(&a_Monster);
	if (it == m_MonsterToDistance.end())
	{
		sDistanceAndChunk newDistanceAndChunk(a_Distance,a_Chunk);
		std::pair<tMonsterToDistance::iterator,bool> result = m_MonsterToDistance.insert(tMonsterToDistance::value_type(&a_Monster,newDistanceAndChunk));
		if (!result.second)
		{
			ASSERT(!"A collected Monster was not found inside distance map using find(), but insert() said there already is a key for it");
		}
	}
	else
	{
		if (a_Distance < it->second.m_Distance)
		{
			it->second.m_Distance = a_Distance;
			it->second.m_Chunk = a_Chunk;
		}
	}

	m_EligibleForSpawnChunks.insert(&a_Chunk);

}

void cMobProximityCounter::convertMaps()
{
	for(tMonsterToDistance::const_iterator itr = m_MonsterToDistance.begin(); itr != m_MonsterToDistance.end(); itr++)
	{
		m_DistanceToMonster.insert(tDistanceToMonster::value_type(itr->second.m_Distance,sMonsterAndChunk(*itr->first,itr->second.m_Chunk)));
	}
}

cMobProximityCounter::sIterablePair cMobProximityCounter::getMobWithinThosesDistances(double a_DistanceMin, double a_DistanceMax)
{
	sIterablePair toReturn;
	toReturn.m_Count = 0;
	toReturn.m_Begin = m_DistanceToMonster.end();
	toReturn.m_End = m_DistanceToMonster.end();

	a_DistanceMin *= a_DistanceMin;// this is because is use square distance
	a_DistanceMax *= a_DistanceMax;

	if (m_DistanceToMonster.size() <= 0)
	{
		convertMaps();
	}

	for(tDistanceToMonster::const_iterator itr = m_DistanceToMonster.begin(); itr != m_DistanceToMonster.end(); itr++)
	{
		if (toReturn.m_Begin == m_DistanceToMonster.end())
		{
			if ((a_DistanceMin == 1) || (itr->first > a_DistanceMin))
			{
				toReturn.m_Begin = itr; // this is the first one with distance > a_DistanceMin;
			}
		}

		if (toReturn.m_Begin != m_DistanceToMonster.end())
		{
			if ((a_DistanceMax != 1) && (itr->first > a_DistanceMax))
			{
				toReturn.m_End = itr; // this is just after the last one with distance < a_DistanceMax
				// Note : if we are not going through this, it's ok, toReturn.m_End will be end();
				break;
			}
			else
			{
				toReturn.m_Count ++;
			}
		}
	}
	return toReturn;
}
