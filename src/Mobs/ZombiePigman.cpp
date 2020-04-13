#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ZombiePigman.h"
#include "../World.h"
#include "../ClientHandle.h"





cZombiePigman::cZombiePigman(void) :
	Super("ZombiePigman", mtZombiePigman, "entity.zombie_pig.hurt", "entity.zombie_pig.death", "entity.zombie_pig.ambient", 0.6, 1.8)
{
}





void cZombiePigman::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_ROTTEN_FLESH);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GOLD_NUGGET);

	cItems RareDrops;
	RareDrops.Add(cItem(E_ITEM_GOLD));
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}





void cZombiePigman::SpawnOn(cClientHandle & a_ClientHandle)
{
		Super::SpawnOn(a_ClientHandle);
		a_ClientHandle.SendEntityEquipment(*this, 0, cItem(E_ITEM_GOLD_SWORD));
}





void cZombiePigman::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);

	if ((a_TDI.Attacker != nullptr) && (a_TDI.Attacker->IsPlayer()))
	{
		// TODO: Anger all nearby zombie pigmen
		// TODO: In vanilla, if one player angers ZPs, do they attack any nearby player, or only that one attacker?
	}
}




