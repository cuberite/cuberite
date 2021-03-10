#pragma once

#include "PassiveMonster.h"





class cChicken:
	public cPassiveMonster
{
	using Super = cPassiveMonster;

public:

	cChicken();

	CLASS_PROTODEF(cChicken)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(Item::BeetrootSeeds);
		a_Items.Add(Item::MelonSeeds);
		a_Items.Add(Item::PumpkinSeeds);
		a_Items.Add(Item::WheatSeeds);
	}

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

private:

	int m_EggDropTimer;
} ;



