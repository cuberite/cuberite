
#pragma once

#include "NewMonster.h"





class cNewSpider :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewSpider(void);

	CLASS_PROTODEF(cNewSpider)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




