
#pragma once

#include "PassiveMonster.h"





class cSheep :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:

	/** The number is the color of the sheep.
	0-15 are the normal colors, if you type -1 the server
	automatically chooses the right color for the sheep when spawned. */
	cSheep(int a_Color = -1);

	CLASS_PROTODEF(cSheep);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_WHEAT); }

	/** Generates a random color for the sheep, like Mojang it does. */
	static NIBBLETYPE GenerateNaturalRandomColor(void);

	bool IsSheared(void) const { return m_IsSheared; }
	void SetSheared(bool a_IsSheared) { m_IsSheared = a_IsSheared; }

	int GetFurColor(void) const { return m_WoolColor; }
	void SetFurColor(int a_WoolColor) { m_WoolColor = a_WoolColor; }

private:
	bool m_IsSheared;
	int m_WoolColor;
	int m_TimeToStopEating;

} ;




