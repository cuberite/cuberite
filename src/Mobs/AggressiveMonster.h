
#pragma once

#include "Monster.h"





class cAggressiveMonster:
	public cMonster
{
	using Super = cMonster;

public:

	cAggressiveMonster(
		const AString & a_ConfigName,
		eMonsterType a_MobType,
		const AString & a_SoundHurt,
		const AString & a_SoundDeath,
		const AString & a_SoundAmbient,
		float a_Width,
		float a_Height
	);

	virtual void Tick          (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void InStateChasing(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;


	virtual void EventSeePlayer(cPlayer * a_Player, cChunk & a_Chunk) override;

	/**
	* Check if a a_MobType is around the monster at a SightDistance
	* return true if it found one and set a_Monster as the Found Monster
	* return false if not found. In this case a_Monster is set to nullptr
	* a_SightDistance is 16 by default.
	*/
	virtual bool CanSeeMobType(eMonsterType a_MobType, cAggressiveMonster * a_Monster, int SightDistance = 16);

	/** Try to perform attack
	returns true if attack was deemed successful (hit player, fired projectile, creeper exploded, etc.) even if it didn't actually do damage
	return false if e.g. the mob is still in cooldown from a previous attack */
	virtual bool Attack(std::chrono::milliseconds a_Dt);
} ;




