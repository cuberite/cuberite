#pragma once

#include "Monster.h"





class cCaveSpider :
	public cMonster
{
	typedef cMonster super;

public:
	cCaveSpider(void);

	CLASS_PROTODEF(cCaveSpider)

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
} ;




