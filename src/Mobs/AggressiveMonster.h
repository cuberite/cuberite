
#pragma once

#include "Monster.h"





class cAggressiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:

	cAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual void Tick          (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void InStateChasing(std::chrono::milliseconds a_Dt) override;
	
	virtual void EventSeePlayer(cEntity *) override;
	virtual void Attack(std::chrono::milliseconds a_Dt);
} ;




