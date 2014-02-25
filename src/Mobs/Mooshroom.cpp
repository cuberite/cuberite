
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Mooshroom.h"
#include "../World.h"
#include "../Entities/Player.h"





cMooshroom::cMooshroom(void) :
	super("Mooshroom", mtMooshroom, "mob.cow.hurt", "mob.cow.hurt", 0.9, 1.3)
{
}





void cMooshroom::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3, IsOnFire() ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}

void cMooshroom::OnRightClicked(cPlayer & a_Player)
{
	int equippeditem = a_Player.GetEquippedItem().m_ItemType;
	if ((equippeditem == E_ITEM_BUCKET))
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
			a_Player.GetInventory().AddItem(E_ITEM_MILK);
		}
	}
	else if ((equippeditem == E_ITEM_BOWL))
	{
		a_Player.GetInventory().RemoveOneEquippedItem();
		a_Player.GetInventory().AddItem(E_ITEM_MUSHROOM_SOUP);
	}
	else if ((equippeditem == E_ITEM_SHEARS))
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.UseEquippedItem();
		}
		cItems Drops;
		Drops.push_back(cItem(E_BLOCK_RED_MUSHROOM, 5));
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), cMonster::mtCow);
		Destroy();
	}
}

