
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Magmacube.h"





cMagmacube::cMagmacube(int a_Size) :
	super("Magmacube", 62, "mob.magmacube.big", "mob.magmacube.big", 0.6 * a_Size, 0.6 * a_Size),
	m_Size(a_Size)
{
}





void cMagmacube::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_MAGMA_CREAM);
}




