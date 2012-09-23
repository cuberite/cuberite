#pragma once

#include "Monster.h"

class cAggressiveMonster : public cMonster
{
public:
	cAggressiveMonster();
	~cAggressiveMonster();

	virtual void Tick(float a_Dt);
	virtual void InStateChasing(float a_Dt);
	
	virtual void EventSeePlayer(cEntity *);
protected:
	float m_ChaseTime;
};
