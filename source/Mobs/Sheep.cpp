
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Sheep.h"
#include "../BlockID.h"
#include "../Entities/Player.h"
#include "../World.h"





cSheep::cSheep(int a_Color) :
	super("Sheep", mtSheep, "mob.sheep.say", "mob.sheep.say", 0.6, 1.3),
	m_IsSheared(false),
	m_WoolColor(a_Color)
{
}





void cSheep::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (!m_IsSheared)
	{
		a_Drops.push_back(cItem(E_BLOCK_WOOL, 1, m_WoolColor));
	}
}





void cSheep::OnRightClicked(cPlayer & a_Player)
{
	if ((a_Player.GetEquippedItem().m_ItemType == E_ITEM_SHEARS) && (!m_IsSheared))
	{
		m_IsSheared = true;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.UseEquippedItem();
		}

		cItems Drops;
		int NumDrops = m_World->GetTickRandomNumber(2) + 1;
		Drops.push_back(cItem(E_BLOCK_WOOL, NumDrops, m_WoolColor));
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_BLACK)
	{
		m_WoolColor = 15;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_RED)
	{
		m_WoolColor = 14;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_GREEN)
	{
		m_WoolColor = 13;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_BROWN)
	{
		m_WoolColor = 12;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_BLUE)
	{
		m_WoolColor = 11;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_PURPLE)
	{
		m_WoolColor = 10;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_CYAN)
	{
		m_WoolColor = 9;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_LIGHTGRAY)
	{
		m_WoolColor = 8;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_GRAY)
	{
		m_WoolColor = 7;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_PINK)
	{
		m_WoolColor = 6;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_LIGHTGREEN)
	{
		m_WoolColor = 5;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_YELLOW)
	{
		m_WoolColor = 4;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_LIGHTBLUE)
	{
		m_WoolColor = 3;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_MAGENTA)
	{
		m_WoolColor = 2;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_ORANGE)
	{
		m_WoolColor = 1;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
	if (a_Player.GetEquippedItem().m_ItemDamage == E_META_DYE_WHITE)
	{
		m_WoolColor = 0;
		m_World->BroadcastEntityMetadata(*this);

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
}
