#pragma once

#include "PassiveMonster.h"





class cChicken :
	public cPassiveMonster
{
	typedef cPassiveMonster super;

public:
	cChicken(void);

	CLASS_PROTODEF(cChicken)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_SEEDS);
	}

	virtual void HandleFalling(void) override;

	virtual void HandleFalling(void) override;

private:

	int m_EggDropTimer;
} ;



