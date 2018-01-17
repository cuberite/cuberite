
#pragma once

#include "PassiveMonster.h"





class cMooshroom :
	public cPassiveMonster
{
	typedef cPassiveMonster super;

public:
	cMooshroom(void);

	CLASS_PROTODEF(cMooshroom)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_WHEAT);
	}
} ;




