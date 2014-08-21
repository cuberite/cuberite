
#pragma once

#include "Monster.h"





class cAggressiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:

	cAggressiveMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual void Tick          (float a_Dt, cChunk & a_Chunk) override;
	virtual void InStateChasing(float a_Dt) override;
	
	virtual void EventSeePlayer(cEntity *) override;
	virtual void Attack(float a_Dt);

protected:
	/** Whether this mob's destination is the same as its target's position. */
	bool IsMovingToTargetPosition();

} ;




