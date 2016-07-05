
#pragma once

#include "PassiveAggressiveMonster.h"





class cSnowGolem :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;

public:
	cSnowGolem(void);

	CLASS_PROTODEF(cSnowGolem)

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
} ;




