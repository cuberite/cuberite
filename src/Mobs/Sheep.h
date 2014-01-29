
#pragma once

#include "PassiveMonster.h"





class cSheep :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cSheep(int a_Color);
	
	CLASS_PROTODEF(cSheep);
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_WHEAT); }

	bool IsSheared(void) const { return m_IsSheared; }
	int GetFurColor(void) const { return m_WoolColor; }

private:
	
	bool m_IsSheared;
	int m_WoolColor;
	int m_TimeToStopEating;

} ;




