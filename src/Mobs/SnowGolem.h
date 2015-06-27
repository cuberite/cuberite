
#pragma once

#include "AggressiveMonster.h"





class cSnowGolem :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSnowGolem(void);

	CLASS_PROTODEF(cSnowGolem)
	
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) REQUIRES(WorldTick) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
} ;




