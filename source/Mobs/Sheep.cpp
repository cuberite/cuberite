
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Sheep.h"
#include "../BlockID.h"





cSheep::cSheep(void) :
	m_IsSheared(false),
	m_WoolColor(E_META_WOOL_WHITE)
{
	m_MobType = 91;
	GetMonsterConfig("Sheep");
}





void cSheep::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	if (!m_IsSheared)
	{
		a_Drops.push_back(cItem(E_ITEM_WHITE_CLOTH, 1, m_WoolColor));
	}
}




