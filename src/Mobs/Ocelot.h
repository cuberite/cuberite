
#pragma once

#include "PassiveMonster.h"





class cOcelot :
	public cPassiveMonster
{
	typedef cPassiveMonster super;

public:
	cOcelot(void) :
		super("Ocelot", mtOcelot, "mob.cat.hitt", "mob.cat.hitt", 0.6, 0.8)
	{
	}

	virtual void GetBreedingItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_RAW_FISH);
	}

	CLASS_PROTODEF(cOcelot)

	// TODO: When ocelots are tameable (or have types) metadata will be needed.
	// virtual void WriteMetadata(cMetadataWriter & a_Writer) const override;
} ;




