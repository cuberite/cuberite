
#pragma once

#include "PassiveAggressiveMonster.h"





class cSnowGolem:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cSnowGolem();

	CLASS_PROTODEF(cSnowGolem)

	virtual void Tick(const std::chrono::milliseconds & a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
} ;




