#pragma once

#include "NewMonster.h"





class cNewCaveSpider :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewCaveSpider(void);

	CLASS_PROTODEF(cNewCaveSpider)

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




