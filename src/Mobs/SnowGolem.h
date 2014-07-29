
#pragma once

#include "AggressiveMonster.h"





class cSnowGolem :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSnowGolem(void);

	CLASS_PROTODEF(cSnowGolem)
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




