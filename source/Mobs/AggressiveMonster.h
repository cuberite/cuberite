
#pragma once

#include "Monster.h"





class cAggressiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:
	cAggressiveMonster(const AString & a_ConfigName, char a_ProtocolMobType, const AString & a_SoundHurt, const AString & a_SoundDeath);

	virtual void Tick(float a_Dt) override;
	virtual void InStateChasing(float a_Dt) override;
	
	virtual void EventSeePlayer(cEntity *) override;
	
protected:
	float m_ChaseTime;
} ;




