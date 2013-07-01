
#pragma once

#include "PassiveAggressiveMonster.h"





class cZombiepigman :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cZombiepigman(void);

	CLASS_PROTODEF(cZombiepigman);

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void KilledBy(cEntity * a_Killer) override;
} ;




