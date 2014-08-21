
#pragma once

#include "NewMonster.h"





class cNewVillager :
	public cNewMonster
{
	typedef cNewMonster super;
	
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

	cNewVillager(eVillagerType VillagerType);

	CLASS_PROTODEF(cNewVillager)

	// Get and set functions.
	int      GetVilType(void)              const { return m_Type; }

private:

	int m_Type;

} ;




