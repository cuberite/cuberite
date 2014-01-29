
#pragma once

#include "PassiveMonster.h"





class cMooshroom :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cMooshroom(void);

	CLASS_PROTODEF(cMooshroom);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_WHEAT); }
} ;




