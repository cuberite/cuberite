
#pragma once

#include "PassiveMonster.h"





class cVillager :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
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

	CLASS_PROTODEF(cVillager);

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	int GetVilType(void) const { return m_Type; }	

private:

	int m_Type;

} ;




