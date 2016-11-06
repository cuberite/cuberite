
#pragma once

#include "PassiveMonster.h"





class cOcelot :
	public cPassiveMonster
{
	typedef cPassiveMonster super;

public:
	cOcelot(void) :
		super("Ocelot", mtOcelot, "entity.cat.hurt", "entity.cat.death", 0.6, 0.8)
	{
	}

	virtual void GetBreedingItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_RAW_FISH);
	}

	CLASS_PROTODEF(cOcelot)
} ;




