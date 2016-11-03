
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombie.h"
#include "../World.h"
#include "../LineBlockTracer.h"





cZombie::cZombie(bool a_IsVillagerZombie) :
	super("Zombie", mtZombie, "mob.zombie.hurt", "mob.zombie.death", 0.6, 1.8),
	m_IsVillagerZombie(a_IsVillagerZombie),
	m_IsConverting(false)
{
	SetBurnsInDaylight(true);
}





void cZombie::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_ROTTEN_FLESH);
	cItems RareDrops;
	RareDrops.Add(cItem(E_ITEM_IRON));
	RareDrops.Add(cItem(E_ITEM_CARROT));
	RareDrops.Add(cItem(E_ITEM_POTATO));
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}





void cZombie::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBool(IsBaby());  // Is baby
	// TODO: This actually encodes the zombie villager profession, but that isn't implemented yet.
	a_Writer.WriteInt(IsVillagerZombie() ? 1 : 0);  // Profession
	a_Writer.WriteBool(IsConverting());  // Is converting
	a_Writer.SkipMeta();  // Are hands held up
}
