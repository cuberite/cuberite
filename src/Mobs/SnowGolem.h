
#pragma once

#include "Monster.h"





class cSnowGolem :
	public cMonster
{
	typedef cMonster super;
	
public:
	cSnowGolem(void);

	CLASS_PROTODEF(cSnowGolem)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




