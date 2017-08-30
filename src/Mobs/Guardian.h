
#pragma once

#include "Monster.h"





class cGuardian :
	public cMonster
{
	typedef cMonster super;

public:
	cGuardian();

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	CLASS_PROTODEF(cGuardian)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	// Guardians do not drown (or float)
	virtual void HandleAir(void) override {}
	virtual void SetSwimState(cChunk & a_Chunk) override {}
} ;




