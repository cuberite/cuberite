
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cow.h"
#include "../Entities/Player.h"





cCow::cCow(void) :
	Super("Cow", mtCow, "entity.cow.hurt", "entity.cow.death", "entity.cow.ambient", 0.9f, 1.4f)
{
}





void cCow::GetDrops(cItems & a_Drops, cEntity * a_Killer)
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





void cCow::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	if (a_Player.GetEquippedItem().m_ItemType == Item::Bucket)
	{
		// Milk the cow.
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::MilkBucket));
		}
	}
}
