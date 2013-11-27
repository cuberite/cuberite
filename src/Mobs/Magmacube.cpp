
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Magmacube.h"





cMagmaCube::cMagmaCube(int a_Size) :
	super("MagmaCube", mtMagmaCube, "mob.MagmaCube.big", "mob.MagmaCube.big", 0.6 * a_Size, 0.6 * a_Size),
	m_Size(a_Size)
{
}





void cMagmaCube::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_MAGMA_CREAM);
}




