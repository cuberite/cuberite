
#pragma once

#include "PassiveMonster.h"





class cVillager :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:

	enum eVillagerType
	{
		VILLAGER_TYPE_FARMER     = 0,
		VILLAGER_TYPE_LIBRARIAN  = 1,
		VILLAGER_TYPE_PRIEST     = 2,
		VILLAGER_TYPE_BLACKSMITH = 3,
		VILLAGER_TYPE_BUTCHER    = 4,
		VILLAGER_TYPE_GENERIC    = 5
	} ;

	cVillager(eVillagerType VillagerType);

	CLASS_PROTODEF(cVillager);

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	int GetVilType(void) const { return m_Type; }	

private:

	int m_Type;

} ;




