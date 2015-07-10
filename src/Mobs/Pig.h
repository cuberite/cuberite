
#pragma once

#include "PassiveMonster.h"
#include "Ageable.h"





class cPig :
	public cPassiveMonster, public Ageable
{
	typedef cPassiveMonster super;
	
public:
	cPig(void);

	CLASS_PROTODEF(cPig)

	// cEntity overrides
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_CARROT); }

	bool IsSaddled(void) const { return m_bIsSaddled; }

private:

	bool m_bIsSaddled;

} ;




