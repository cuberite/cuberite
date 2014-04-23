#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MagmaCube.h"





cMagmaCube::cMagmaCube(int a_Size) :
	super("MagmaCube", mtMagmaCube, "mob.MagmaCube.big", "mob.MagmaCube.big", 0.6 * a_Size, 0.6 * a_Size),
	m_Size(a_Size)
{
}





void cMagmaCube::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	if (GetSize() > 1)
	{
		AddRandomUncommonDropItem(a_Drops, 25.0f, E_ITEM_MAGMA_CREAM);
	}
}




