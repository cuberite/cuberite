
#pragma once

class cChunk;
class cEntity;


// This class is used to collect, for each Mob, what is the distance of the closest player
// it was first being designed in order to make mobs spawn / despawn / act
// as the behaviour and even life of mobs depends on the distance to closest player
class cMobProximityCounter
{
protected :
	// structs used for later maps (see m_MonsterToDistance and m_DistanceToMonster)
	struct sDistanceAndChunk
	{
		sDistanceAndChunk(double a_Distance, cChunk & a_Chunk) : m_Distance(a_Distance), m_Chunk(&a_Chunk) {}
		double m_Distance;
		cChunk * m_Chunk;
	};
	struct sMonsterAndChunk
	{
		sMonsterAndChunk(cEntity & a_Monster, cChunk & a_Chunk) : m_Monster(a_Monster), m_Chunk(a_Chunk) {}
		cEntity & m_Monster;
		cChunk & m_Chunk;
	};

public :
	typedef std::map<cEntity *, sDistanceAndChunk> tMonsterToDistance;
	typedef std::multimap<double, sMonsterAndChunk> tDistanceToMonster;

protected :
	// this map is filled during collection phase, it will be later transformed into DistanceToMonster
	tMonsterToDistance m_MonsterToDistance;

	// this map is generated after collection phase, in order to access monster by distance to player
	tDistanceToMonster m_DistanceToMonster;

	// this are the collected chunks. Used to determinate the number of elligible chunk for spawning.
	std::set<cChunk*> m_EligibleForSpawnChunks;

protected :
	// transform monsterToDistance map (that was usefull for collecting) into distanceToMonster
	// that will be usefull for picking up.
	void convertMaps();

public :
	// count a mob on a specified chunk with specified distance to an unkown player
	// if the distance is shortest than the one collected, this become the new closest
	// distance and the chunk become the "hosting" chunk (that is the one that will perform the action)
	void CollectMob(cEntity & a_Monster, cChunk & a_Chunk, double a_Distance);

	// return the mobs that are within the range of distance of the closest player they are
	// that means that if a mob is 30 m from a player and 150 m from another one. It will be
	// in the range [0..50] but not in [100..200]
	struct sIterablePair
	{
		tDistanceToMonster::const_iterator m_Begin;
		tDistanceToMonster::const_iterator m_End;
		int m_Count;
	};
	sIterablePair getMobWithinThosesDistances(double a_DistanceMin, double a_DistanceMax);

};
