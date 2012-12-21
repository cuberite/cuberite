
#pragma once

#include "Monster.h"





class cAggressiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:
	cAggressiveMonster(void);

	virtual void Tick(float a_Dt) override;
	virtual void InStateChasing(float a_Dt) override;
	
	virtual void EventSeePlayer(cEntity *) override;
	
protected:
	float m_ChaseTime;
} ;




