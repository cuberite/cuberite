
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Mooshroom.h"
#include "../Entities/Player.h"





cMooshroom::cMooshroom(void) :
	Super("Mooshroom", mtMooshroom, "entity.cow.hurt", "entity.cow.death", "entity.cow.ambient", 0.9f, 1.4f)
{
}





void cMooshroom::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (IsBaby())
	{
		return;  // Babies don't drop items
	}

	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, Item::Leather);
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? Item::CookedBeef : Item::Beef);
}





void cMooshroom::OnRightClicked(cPlayer & a_Player)
{
	switch (a_Player.GetEquippedItem().m_ItemType)
	{
		case Item::Bucket:
		{
			// Milk the cow.
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::MilkBucket));
			}
		} break;
		case Item::Bowl:
		{
			// Soup the cow.
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::MushroomStem));
			}
		} break;
		case Item::Shears:
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.UseEquippedItem();
			}

			cItems Drops;
			Drops.emplace_back(Item::RedMushroom, 5);
			m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
			m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), mtCow, false);
			Destroy();
		} break;
		default:
		{
			FLOGWARNING("{}: Item type not handled {}.", __FUNCTION__, a_Player.GetEquippedItem().m_ItemType);
		}
	}
}
