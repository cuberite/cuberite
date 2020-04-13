
#pragma once

#include "AggressiveMonster.h"





class cWitch:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cWitch();

	CLASS_PROTODEF(cWitch)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	bool IsAngry(void) const {return ((m_EMState == ATTACKING) || (m_EMState == CHASING)); }
} ;




