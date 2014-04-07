
#pragma once

#include "PassiveMonster.h"





class cSquid :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cSquid();

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	CLASS_PROTODEF(cSquid);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	// Squids do not drown (or float)
	virtual void HandleAir(void) override {}
	virtual void SetSwimState(cChunk & a_Chunk) override {}
} ;




