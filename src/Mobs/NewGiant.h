
#pragma once

#include "NewMonster.h"





class cNewGiant :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewGiant(void);

	CLASS_PROTODEF(cNewGiant)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
} ;




