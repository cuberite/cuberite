
#pragma once

#include "AggressiveMonster.h"






// tolua_begin
class cWitch :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	// tolua_end
	cWitch();

	CLASS_PROTODEF(cWitch);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	// tolua_begin
	bool IsAngry(void) const {return ((m_EMState == ATTACKING) || (m_EMState == CHASING)); }
} ;
// tolua_end




