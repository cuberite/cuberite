
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Sheep.h"
#include "../BlockID.h"





cSheep::cSheep(void) :
	super("Sheep", 91, "mob.sheep.say", "mob.sheep.say"),
	m_IsSheared(false),
	m_WoolColor(E_META_WOOL_WHITE)
{
}





void cSheep::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	if (!m_IsSheared)
	{
		a_Drops.push_back(cItem(E_BLOCK_WOOL, 1, m_WoolColor));
	}
}




