
#pragma once

#include "PassiveMonster.h"
#include "Ageable.h"




class cRabbit :
	public cPassiveMonster, public Ageable
{
	typedef cPassiveMonster super;
	
public:
	cRabbit();

	CLASS_PROTODEF(cRabbit)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;

	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_CARROT); }

} ;
