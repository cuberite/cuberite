
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
		auto Enchantments = a_Killer->GetEquippedWeapon().get<cEnchantments>();
		if (Enchantments.has_value())
		{
			LootingLevel = Enchantments.value().GetLevel(cEnchantments::enchLooting);
		}
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}





void cCow::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	short HeldItem = a_Player.GetEquippedItem().m_ItemType;
	if (HeldItem == E_ITEM_BUCKET)
	{
		// Milk the cow.
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_MILK));
		}
	}
}
