
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobProximityCounter.h"

#include "Entities/Entity.h"
#include "Chunk.h"





void cMobProximityCounter::CollectMob(cEntity & a_Monster, cChunk & a_Chunk, double a_Distance)
{
	// LOGD("Collecting monster %s, with distance %f", a_Monster->GetClass(), a_Distance);
	tMonsterToDistance::iterator it = m_MonsterToDistance.find(&a_Monster);
	if (it == m_MonsterToDistance.end())
	{
		sDistanceAndChunk newDistanceAndChunk(a_Distance, a_Chunk);
		std::pair<tMonsterToDistance::iterator, bool> result = m_MonsterToDistance.insert(tMonsterToDistance::value_type(&a_Monster, newDistanceAndChunk));
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
			it->second.m_Chunk = &a_Chunk;
		}
	}

	m_EligibleForSpawnChunks.insert(&a_Chunk);

}

void cMobProximityCounter::convertMaps()
{
	for (tMonsterToDistance::const_iterator itr = m_MonsterToDistance.begin(); itr != m_MonsterToDistance.end(); ++itr)
	{
		m_DistanceToMonster.insert(tDistanceToMonster::value_type(itr->second.m_Distance, sMonsterAndChunk(*itr->first, *itr->second.m_Chunk)));
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

	if (m_DistanceToMonster.empty())
	{
		convertMaps();
	}

	for (tDistanceToMonster::const_iterator itr = m_DistanceToMonster.begin(); itr != m_DistanceToMonster.end(); ++itr)
	{
		if (toReturn.m_Begin == m_DistanceToMonster.end())
		{
			if ((a_DistanceMin == 1.0) || (itr->first > a_DistanceMin))
			{
				toReturn.m_Begin = itr;  // This is the first one with distance > a_DistanceMin;
			}
		}

		if (toReturn.m_Begin != m_DistanceToMonster.end())
		{
			if ((a_DistanceMax != 1.0) && (itr->first > a_DistanceMax))
			{
				toReturn.m_End = itr;  // This is just after the last one with distance < a_DistanceMax
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
