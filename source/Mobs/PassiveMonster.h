
#pragma once

#include "Monster.h"





class cPassiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:
	cPassiveMonster(void);

	virtual void Tick(float a_Dt) override;

	/// When hit by someone, run away
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
} ;




