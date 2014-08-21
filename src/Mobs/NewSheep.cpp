
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewSheep.h"
#include "../BlockID.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "FastRandom.h"





cNewSheep::cNewSheep(int a_Color) :
	super("Sheep", mtSheep, "mob.sheep.say", "mob.sheep.say", 0.6, 1.3),
	m_IsSheared(false),
	m_WoolColor(a_Color)
	// m_TimeToStopEating(-1)
{
	// Generate random wool color.
	if (m_WoolColor == -1)
	{
		m_WoolColor = GenerateNaturalRandomColor();
	}

	if ((m_WoolColor < 0) || (m_WoolColor > 15))
	{
		m_WoolColor = 0;
	}
}





void cNewSheep::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (!m_IsSheared)
	{
		a_Drops.push_back(cItem(E_BLOCK_WOOL, 1, m_WoolColor));
	}
}





NIBBLETYPE cNewSheep::GenerateNaturalRandomColor(void)
{
	cFastRandom Random;
	int Chance = Random.NextInt(101);

	if (Chance <= 81)
	{
		return E_META_WOOL_WHITE;
	}
	else if (Chance <= 86)
	{
		return E_META_WOOL_BLACK;
	}
	else if (Chance <= 91)
	{
		return E_META_WOOL_GRAY;
	}
	else if (Chance <= 96)
	{
		return E_META_WOOL_LIGHTGRAY;
	}
	else if (Chance <= 99)
	{
		return E_META_WOOL_BROWN;
	}
	else
	{
		return E_META_WOOL_PINK;
	}
}

