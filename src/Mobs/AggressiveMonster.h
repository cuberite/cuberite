
#pragma once

#include "Monster.h"





class cAggressiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:

	cAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual void Tick          (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) REQUIRES(WorldTick) override;
	virtual void InStateChasing(std::chrono::milliseconds a_Dt) override;
	
	virtual void EventSeePlayer(cEntity *) override;
	
	/** Try to perform attack
	returns true if attack was deemed successful (hit player, fired projectile, creeper exploded, etc.) even if it didn't actually do damage
	return false if e.g. the mob is still in cooldown from a previous attack */
	virtual bool Attack(std::chrono::milliseconds a_Dt);
} ;




