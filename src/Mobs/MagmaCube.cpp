#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MagmaCube.h"





cMagmaCube::cMagmaCube(int a_Size) :
	Super("MagmaCube", mtMagmaCube, Printf("entity.%smagmacube.hurt", GetSizeName(a_Size).c_str()), Printf("entity.%smagmacube.death", GetSizeName(a_Size).c_str()), "", 0.6 * a_Size, 0.6 * a_Size),
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





AString cMagmaCube::GetSizeName(int a_Size)
{
	if (a_Size == 1)
	{
		return "small_";
	}
	return "";
}
