
#pragma once

#include "Monster.h"





class cVillager :
	public cMonster
{
	typedef cMonster super;
	
public:

	enum eVillagerType
	{
		vtFarmer     = 0,
		vtLibrarian  = 1,
		vtPriest     = 2,
		vtBlacksmith = 3,
		vtButcher    = 4,
		vtGeneric    = 5,
		vtMax
	} ;

	cVillager(eVillagerType VillagerType);

	CLASS_PROTODEF(cVillager)

	// Get and set functions.
	int      GetVilType(void)              const { return m_Type; }

private:

	int m_Type;

} ;




