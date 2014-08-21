
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewSlime.h"
#include "FastRandom.h"
#include "World.h"





cNewSlime::cNewSlime(int a_Size) :
	super("Slime",
		mtSlime,
		Printf("mob.slime.%s", GetSizeName(a_Size).c_str()),
		Printf("mob.slime.%s", GetSizeName(a_Size).c_str()),
		0.6 * a_Size,
		0.6 * a_Size
	),
	m_Size(a_Size)
{
}





void cNewSlime::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}

	// Only slimes with the size 1 can drop slimeballs.
	if (m_Size == 1)
	{
		AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_SLIMEBALL);
	}
}





const AString cNewSlime::GetSizeName(int a_Size) const
{
	if (a_Size > 1)
	{
		return "big";
	}
	else
	{
		return "small";
	}
}




