
#pragma once

#include "AggressiveMonster.h"





class cSpider :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cSpider(void);

	CLASS_PROTODEF(cSpider)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void EventSeePlayer(cPlayer *, cChunk & a_Chunk) override;
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
} ;




