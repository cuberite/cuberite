
#pragma once

#include "PassiveMonster.h"






// tolua_begin
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
	// tolua_end
	
	cVillager(eVillagerType VillagerType);

	CLASS_PROTODEF(cVillager);

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	
	// tolua_begin
	int GetVilType(void) const { return m_Type; }	
	// tolua_end

private:

	int m_Type;

} ; // tolua_export




