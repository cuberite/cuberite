#pragma once

#include "AggressiveMonster.h"





class cCaveSpider :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cCaveSpider(void);

	CLASS_PROTODEF(cCaveSpider)

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
} ;




