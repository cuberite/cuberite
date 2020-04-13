
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ZombieVillager.h"
#include "../World.h"
#include "../LineBlockTracer.h"
#include "../Entities/Player.h"





cZombieVillager::cZombieVillager(cVillager::eVillagerType a_Profession) :
	Super("ZombieVillager", mtZombieVillager, "entity.zombie_villager.hurt", "entity.zombie_villager.death", "entity.ambient", 0.6, 1.8),
	m_ConversionTime(-1),
	m_Profession(a_Profession)
{
	SetBurnsInDaylight(true);
}





void cZombieVillager::GetDrops(cItems & a_Drops, cEntity * a_Killer)
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





void cZombieVillager::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	LOGD("Conversion time: %d", m_ConversionTime);

	if (m_ConversionTime == 0)
	{
		m_World->BroadcastSoundEffect("entity.zombie_villager.cure", GetPosition(), 1.0f, 1.0f);
		Destroy();
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), mtVillager, false);
	}
	else if (m_ConversionTime > 0)
	{
		m_ConversionTime--;
	}
}





void cZombieVillager::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	const cItem & EquippedItem = a_Player.GetEquippedItem();
	if ((EquippedItem.m_ItemType == E_ITEM_GOLDEN_APPLE) && GetEntityEffect(cEntityEffect::effWeakness) != nullptr)
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}

		m_ConversionTime = 6000;
	}
}
