
#pragma once

#include "PassiveMonster.h"





class cCow :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cCow();

	CLASS_PROTODEF(cCow)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_WHEAT); }

} ;




