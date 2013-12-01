
#pragma once

#include "PassiveMonster.h"





// tolua_begin
class cBat :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	// tolua_end
	cBat(void);
	
	CLASS_PROTODEF(cBat);
	// tolua_begin
	bool IsHanging(void) const {return false; }
	
} ;

// tolua_end


