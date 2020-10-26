
#pragma once

#include "PassiveMonster.h"





class cPig:
	public cPassiveMonster
{
	using Super = cPassiveMonster;

public:

	cPig();

	CLASS_PROTODEF(cPig)

	// cEntity overrides
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_CARROT);
	}

	bool IsSaddled(void) const { return m_bIsSaddled; }

private:

	bool m_bIsSaddled;

} ;




