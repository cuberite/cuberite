
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ItemFrame.h"
#include "ClientHandle.h"
#include "Player.h"





cItemFrame::cItemFrame(int a_BlockFace, double a_X, double a_Y, double a_Z)
	: cEntity(etItemFrame, a_X, a_Y, a_Z, 0.8, 0.8),
	m_BlockFace(a_BlockFace),
	m_Item(E_BLOCK_AIR),
	m_Rotation(0)
{
	SetMaxHealth(1);
	SetHealth(1);

	if ((a_BlockFace == 0) || (a_BlockFace == 2)) // Probably a client bug, but two directions are flipped and contrary to the norm, so we do -180
	{
		SetYaw((a_BlockFace * 90) - 180);
	}
	else
	{
		SetYaw(a_BlockFace * 90);
	}
}





void cItemFrame::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnObject(*this, 71, m_BlockFace, (Byte)GetYaw(), (Byte)GetPitch());
}





void cItemFrame::OnRightClicked(cPlayer & a_Player)
{
	if (!m_Item.IsEmpty())
	{
		// Item not empty, rotate, clipping values to zero to three inclusive
		m_Rotation++;
		if (m_Rotation >= 4)
			m_Rotation = 0;
	}
	else if (!a_Player.GetEquippedItem().IsEmpty())
	{
		// Item empty, and player held item not empty - add this item to self
		m_Item = a_Player.GetEquippedItem();
		m_Item.m_ItemCount = 1;

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}

	GetWorld()->BroadcastEntityMetadata(*this); // Update clients
}






void cItemFrame::KilledBy(cEntity * a_Killer)
{
	if (m_Item.IsEmpty())
	{
		super::KilledBy(a_Killer);
		Destroy();
		return;
	}

	if ((a_Killer != NULL) && a_Killer->IsPlayer() && !((cPlayer *)a_Killer)->IsGameModeCreative())
	{
		cItems Item;
		Item.push_back(m_Item);

		GetWorld()->SpawnItemPickups(Item, GetPosX(), GetPosY(), GetPosZ());
	}

	SetHealth(GetMaxHealth());
	m_Item.Clear();
	GetWorld()->BroadcastEntityMetadata(*this);
}





void cItemFrame::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if ((a_Killer != NULL) && a_Killer->IsPlayer() && !((cPlayer *)a_Killer)->IsGameModeCreative())
	{
		a_Items.push_back(cItem(E_ITEM_ITEM_FRAME));
	}
}




