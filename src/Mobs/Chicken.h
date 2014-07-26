#pragma once

#include "PassiveMonster.h"





class cChicken :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cChicken(void);

	CLASS_PROTODEF(cChicken)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_SEEDS); }

private:

	int m_EggDropTimer;
} ;



