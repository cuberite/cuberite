
#pragma once

#include "PassiveMonster.h"





class cVillager :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cVillager(int Type);

	CLASS_PROTODEF(cVillager);

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	int GetVilType(void) const { return m_Type; }

private:

	int m_Type;

} ;




