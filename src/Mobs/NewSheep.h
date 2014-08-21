
#pragma once

#include "NewMonster.h"





class cNewSheep :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:

	/** The number is the color of the sheep.
	Use E_META_WOOL_* constants for the wool color.
	If you type -1, the server will generate a random color
	with the GenerateNaturalRandomColor() function. */
	cNewSheep(int a_Color = -1);

	CLASS_PROTODEF(cNewSheep)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	/** Generates a random color for the sheep like the vanilla server.
	The percent's where used are from the wiki: http://minecraft.gamepedia.com/Sheep#Breeding */
	static NIBBLETYPE GenerateNaturalRandomColor(void);

	bool IsSheared(void) const { return m_IsSheared; }
	void SetSheared(bool a_IsSheared) { m_IsSheared = a_IsSheared; }

	int GetFurColor(void) const { return m_WoolColor; }
	void SetFurColor(int a_WoolColor) { m_WoolColor = a_WoolColor; }

private:
	bool m_IsSheared;
	int m_WoolColor;
	// int m_TimeToStopEating;

} ;




