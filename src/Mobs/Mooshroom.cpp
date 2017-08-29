
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Mooshroom.h"
#include "../Entities/Player.h"










cMooshroom::cMooshroom(void) :
	super("Mooshroom", mtMooshroom, "entity.cow.hurt", "entity.cow.death", 0.9, 1.3)
{
	m_EMPersonality = PASSIVE;
	m_BehaviorBreeder.AttachToMonster(*this);
	m_BehaviorCoward.AttachToMonster(*this);
	m_BehaviorItemFollower.AttachToMonster(*this);
	m_BehaviorWanderer.AttachToMonster(*this);
}





void cMooshroom::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}





void cMooshroom::OnRightClicked(cPlayer & a_Player)
{
	// mobTodo Behaviors
	switch (a_Player.GetEquippedItem().m_ItemType)
	{
		case E_ITEM_BUCKET:
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
				a_Player.GetInventory().AddItem(E_ITEM_MILK);
			}
		} break;
		case E_ITEM_BOWL:
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
				a_Player.GetInventory().AddItem(E_ITEM_MUSHROOM_SOUP);
			}
		} break;
		case E_ITEM_SHEARS:
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.UseEquippedItem();
			}

			cItems Drops;
			Drops.push_back(cItem(E_BLOCK_RED_MUSHROOM, 5, 0));
			m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
			m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), mtCow, false);
			Destroy();
		} break;
	}
}





cBehaviorBreeder * cMooshroom::GetBehaviorBreeder()
{
	return &m_BehaviorBreeder;
}





const cBehaviorBreeder * cMooshroom::GetBehaviorBreeder() const
{
	return static_cast<const cBehaviorBreeder *>(&m_BehaviorBreeder);
}
