#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MagmaCube.h"





cMagmaCube::cMagmaCube(int a_Size) :
	Super(
		"MagmaCube",
		mtMagmaCube,
		fmt::format(FMT_STRING("entity.{}magmacube.hurt"),  GetSizeName(a_Size)),
		fmt::format(FMT_STRING("entity.{}magmacube.death"), GetSizeName(a_Size)),
		"",
		0.51f * a_Size,
		0.51f * a_Size
	),
	m_Size(a_Size)
{
}





AString cMagmaCube::GetSizeName(int a_Size)
{
	if (a_Size == 1)
	{
		return "small_";
	}
	return "";
}





void cMagmaCube::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	if (GetSize() > 1)
	{
		AddRandomUncommonDropItem(a_Drops, 25.0f, Item::MagmaCream);
	}
}
