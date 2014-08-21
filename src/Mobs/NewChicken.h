#pragma once

#include "NewMonster.h"





class cNewChicken :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewChicken(void);

	CLASS_PROTODEF(cNewChicken)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

private:

	int m_EggDropTimer;
} ;



