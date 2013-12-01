
#pragma once

#include "AggressiveMonster.h"





// tolua_begin
class cGhast :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	// tolua_end
	cGhast(void);

	CLASS_PROTODEF(cGhast);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Attack(float a_Dt) override;

	// tolua_begin
	bool IsCharging(void) const {return false; }
} ;
// tolua_end



