
#pragma once

#include "AggressiveMonster.h"
#include "../MersenneTwister.h"





class cWitch :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cWitch();

	CLASS_PROTODEF(cWitch)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsAngry(void) const {return ((m_EMState == ATTACKING) || (m_EMState == CHASING)); }
} ;




