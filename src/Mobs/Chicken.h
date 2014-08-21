#pragma once

#include "Monster.h"





class cChicken :
	public cMonster
{
	typedef cMonster super;
	
public:
	cChicken(void);

	CLASS_PROTODEF(cChicken)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) /*override*/;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) /*override*/;

private:

	int m_EggDropTimer;
} ;



