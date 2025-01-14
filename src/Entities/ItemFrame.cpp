
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ItemFrame.h"
#include "Player.h"
#include "../ClientHandle.h"
#include "Chunk.h"





cItemFrame::cItemFrame(eBlockFace a_BlockFace, Vector3d a_Pos):
	Super(etItemFrame, a_BlockFace, a_Pos),
	m_Item(Item::Air),
	m_ItemRotation(0)
{
}





bool cItemFrame::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	// Take environmental or non-player damage normally:
	if (m_Item.IsEmpty() || (a_TDI.Attacker == nullptr) || !a_TDI.Attacker->IsPlayer())
	{
		return Super::DoTakeDamage(a_TDI);
	}

	// Only pop out a pickup if attacked by a non-creative player:
	if (!static_cast<cPlayer *>(a_TDI.Attacker)->IsGameModeCreative())
	{
		// Where the pickup spawns, offset by half cPickup height to centre in the block.
		const auto SpawnPosition = GetPosition().addedY(-0.125);

		// The direction the pickup travels to simulate a pop-out effect.
		const auto FlyOutSpeed = AddFaceDirection(Vector3i(), ProtocolFaceToBlockFace(m_Facing)) * 2;

		// Spawn the frame's held item:
		GetWorld()->SpawnItemPickup(SpawnPosition, m_Item, FlyOutSpeed);
	}

	// In any case we have a held item and were hit by a player, so clear it:
	m_Item.Empty();
	m_ItemRotation = 0;
	a_TDI.FinalDamage = 0;
	SetInvulnerableTicks(0);
	GetWorld()->BroadcastEntityMetadata(*this);
	return false;
}





void cItemFrame::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if (!m_Item.IsEmpty())
	{
		a_Items.push_back(m_Item);
	}

	a_Items.emplace_back(Item::ItemFrame);
}





void cItemFrame::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	if (!m_Item.IsEmpty())
	{
		// Item not empty, rotate, clipping values to zero to seven inclusive
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
	GetParentChunk()->MarkDirty();               // Mark chunk dirty to save rotation or item
}





void cItemFrame::SpawnOn(cClientHandle & a_ClientHandle)
{
	Super::SpawnOn(a_ClientHandle);
	a_ClientHandle.SendSpawnEntity(*this);
	a_ClientHandle.SendEntityMetadata(*this);

	if (m_Item.m_ItemType == Item::FilledMap)
	{
		cMap * Map = GetWorld()->GetMapManager().GetMapData(static_cast<unsigned>(m_Item.m_ItemDamage));
		if (Map != nullptr)
		{
			a_ClientHandle.SendMapData(*Map, 0, 0);
		}
	}
}
