
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ItemFrame.h"
#include "Player.h"
#include "../ClientHandle.h"





cItemFrame::cItemFrame(eBlockFace a_BlockFace, Vector3d a_Pos):
	Super(etItemFrame, a_BlockFace, a_Pos),
	m_Item(E_BLOCK_AIR),
	m_ItemRotation(0)
{
}





void cItemFrame::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	if (!m_Item.IsEmpty())
	{
		// Item not empty, rotate, clipping values to zero to three inclusive
		m_ItemRotation++;
		if (m_ItemRotation >= 8)
		{
			m_ItemRotation = 0;
		}
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

	GetWorld()->BroadcastEntityMetadata(*this);  // Update clients
}





void cItemFrame::KilledBy(TakeDamageInfo & a_TDI)
{
	if (m_Item.IsEmpty())
	{
		Super::KilledBy(a_TDI);
		Destroy();
		return;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPlayer() && !static_cast<cPlayer *>(a_TDI.Attacker)->IsGameModeCreative())
	{
		cItems Item;
		Item.push_back(m_Item);

		GetWorld()->SpawnItemPickups(Item, GetPosX(), GetPosY(), GetPosZ());
	}

	SetHealth(GetMaxHealth());
	m_Item.Empty();
	m_ItemRotation = 0;
	SetInvulnerableTicks(0);
	GetWorld()->BroadcastEntityMetadata(*this);
}





void cItemFrame::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if ((a_Killer != nullptr) && a_Killer->IsPlayer() && !static_cast<cPlayer *>(a_Killer)->IsGameModeCreative())
	{
		a_Items.push_back(cItem(E_ITEM_ITEM_FRAME));
	}
}





void cItemFrame::SpawnOn(cClientHandle & a_ClientHandle)
{
	Super::SpawnOn(a_ClientHandle);
	a_ClientHandle.SendSpawnEntity(*this);
	a_ClientHandle.SendEntityMetadata(*this);
}
