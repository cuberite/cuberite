
#pragma once

#include "PassiveMonster.h"





class cCow:
	public cPassiveMonster
{
	using Super = cPassiveMonster;

public:

	cCow();

	CLASS_PROTODEF(cCow)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_WHEAT);
	}

} ;




