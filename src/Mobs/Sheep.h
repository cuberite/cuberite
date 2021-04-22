
#pragma once

#include "PassiveMonster.h"





class cSheep:
	public cPassiveMonster
{
	using Super = cPassiveMonster;

public:

	/** The number is the color of the sheep.
	Use E_META_WOOL_* constants for the wool color.
	If you type -1, the server will generate a random color
	with the GenerateNaturalRandomColor() function. */
	cSheep(int a_Color = -1);

	CLASS_PROTODEF(cSheep)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void InheritFromParents(cMonster * a_Parent1, cMonster * a_Parent2) override;

	virtual void GetFollowedItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_WHEAT);
	}

	/** Generates a random color for the sheep like the vanilla server.
	The percent's where used are from the wiki: https://minecraft.gamepedia.com/Sheep#Breeding */
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




