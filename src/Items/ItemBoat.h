
// ItemBoat.h

// Declares the various boat ItemHandlers





#pragma once

#include "../Entities/Boat.h"





class cItemBoatHandler :
	public cItemHandler
{
	typedef cItemHandler super;
	
public:
	cItemBoatHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}
	
	
	
	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (a_Dir < 0)
		{
			return false;
		}
		
		double x = (double)a_BlockX + 0.5;
		double y = (double)a_BlockY + 0.5;
		double z = (double)a_BlockZ + 0.5;

		cBoat * Boat = NULL;

		Boat = new cBoat (x, y, z);
		Boat->Initialize(a_World);

		return true;
	}
	
} ;




