#pragma once

#include "PassiveAggressiveMonster.h"
#include "Ageable.h"




class cZombiePigman :
	public cPassiveAggressiveMonster, public Ageable
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cZombiePigman(void);

	CLASS_PROTODEF(cZombiePigman)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;
	
	virtual bool IsUndead(void) override { return true; }
} ;




