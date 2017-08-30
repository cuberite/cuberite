
#pragma once

#include "Monster.h"





class cSpider :
	public cMonster
{
	typedef cMonster super;

public:
	cSpider(void);

	CLASS_PROTODEF(cSpider)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
} ;




