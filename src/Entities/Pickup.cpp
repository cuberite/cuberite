
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
#include <cstdlib>
#endif

#include "Pickup.h"
#include "Player.h"
#include "../Mobs/Villager.h"
#include "../ClientHandle.h"
#include "../World.h"
#include "../Server.h"
#include "../Bindings/PluginManager.h"
#include "../Registries/Items.h"
#include "../Root.h"
#include "../Chunk.h"




class PickupCombiningCallback
{
public:

	PickupCombiningCallback(cPickup * a_Pickup) :
		m_Pickup(a_Pickup),
		m_FoundMatchingPickup(false),
		m_MaxStackSize(a_Pickup->GetItem().GetMaxStackSize())
	{
	}

	~PickupCombiningCallback()
	{
		if (m_FoundMatchingPickup)
		{
			m_Pickup->GetWorld()->BroadcastEntityMetadata(*m_Pickup);
		}
	}

	bool operator()(cEntity & a_Entity)
	{
		ASSERT(a_Entity.IsTicking());

		if (!a_Entity.IsPickup() || (&a_Entity == m_Pickup))
		{
			return false;
		}

		auto & OtherPickup = static_cast<cPickup &>(a_Entity);
		cItem & OtherItem = OtherPickup.GetItem();
		cItem & Item = m_Pickup->GetItem();

		if (!Item.IsEqual(OtherItem) || !OtherPickup.CanCombine() || OtherPickup.IsCollected())
		{
			return false;
		}

		// The recipient pickup is the one with more items, and vice versa for the donor.
		auto [Recipient, Donor] = Item.m_ItemCount > OtherItem.m_ItemCount ? std::make_pair(m_Pickup, &OtherPickup) : std::make_pair(&OtherPickup, m_Pickup);

		// Try to combine, and stop if we're the one who is full:
		if (!CombineInto(Recipient->GetItem(), Donor->GetItem(), m_MaxStackSize))
		{
			return Recipient == m_Pickup;
		}

		if (Donor->GetItem().m_ItemCount == 0)
		{
			Donor->Destroy();
			m_Pickup = Recipient;
		}
		else
		{
			OtherPickup.GetWorld()->BroadcastEntityMetadata(OtherPickup);
		}

		m_FoundMatchingPickup = true;
		return false;
	}

	static bool CombineInto(cItem & a_Recipient, cItem & a_Donor, const char a_MaxStackSize)
	{
		// Check for plugin shenanigans:
		if (a_Recipient.m_ItemCount > a_MaxStackSize)
		{
			return false;
		}

		const auto Move = std::min(a_Donor.m_ItemCount, static_cast<char>(a_MaxStackSize - a_Recipient.m_ItemCount));

		// Stop if recipient full:
		if (Move == 0)
		{
			return false;
		}

		a_Donor.m_ItemCount -= Move;
		a_Recipient.m_ItemCount += Move;
		return true;
	}

private:

	cPickup * m_Pickup;

	bool m_FoundMatchingPickup;

	char m_MaxStackSize;
};





////////////////////////////////////////////////////////////////////////////////
// cPickup:

cPickup::cPickup(Vector3d a_Position, cItem && a_Item, Vector3d a_Speed, cTickTime a_CollectionDelay, cTickTime a_Lifetime) :
	Super(etPickup, a_Position, 0.25f, 0.25f),
	m_Item(std::move(a_Item)),
	m_RemainingCollectionDelay(a_CollectionDelay),
	m_RemainingLifetime(a_Lifetime),
	m_IsCollected(false)
{
	SetGravity(-16.0f);
	SetAirDrag(0.02f);
	SetMaxHealth(5);
	SetHealth(5);
	SetSpeed(a_Speed);
}





bool cPickup::CollectedBy(cEntity & a_Dest)
{
	if (m_IsCollected)
	{
		// It's already collected!
		return false;
	}

	if (!a_Dest.IsPawn())
	{
		// Pawns can't pick up items:
		return false;
	}

	if (m_RemainingCollectionDelay > m_RemainingCollectionDelay.zero())
	{
		// Not old enough to be collected!
		return false;
	}

	if (a_Dest.IsMob())
	{
		auto & Mob = static_cast<cMonster &>(a_Dest);
		if (Mob.GetMobType() == mtVillager)
		{
			// Villagers only pickup food
			if (!ItemCategory::IsVillagerFood(m_Item.m_ItemType))
			{
				return false;
			}

			auto & Villager = static_cast<cVillager &>(Mob);
			char NumAdded = Villager.GetInventory().AddItem(m_Item);
			if (NumAdded > 0)
			{
				OnCollectedBy(Mob, NumAdded);
				return true;
			}
		}
	}
	else if (a_Dest.IsPlayer())
	{
		auto & Player = static_cast<cPlayer &>(a_Dest);

		// If the player is a spectator, he cannot collect anything
		if (Player.IsGameModeSpectator())
		{
			return false;
		}

		if (cRoot::Get()->GetPluginManager()->CallHookCollectingPickup(Player, *this))
		{
			// Collection refused because a plugin has said no:
			return false;
		}

		char NumAdded = Player.GetInventory().AddItem(m_Item);
		if (NumAdded > 0)
		{
			// Check achievements
			switch (m_Item.m_ItemType)
			{
				case E_BLOCK_LOG:      Player.AwardAchievement(CustomStatistic::AchMineWood); break;
				case E_ITEM_LEATHER:   Player.AwardAchievement(CustomStatistic::AchKillCow);  break;
				case E_ITEM_DIAMOND:   Player.AwardAchievement(CustomStatistic::AchDiamonds); break;
				case E_ITEM_BLAZE_ROD: Player.AwardAchievement(CustomStatistic::AchBlazeRod); break;
				default: break;
			}

			OnCollectedBy(Player, NumAdded);
			return true;
		}
	}

	// Either destination cannot collect, or no space in inventory:
	return false;
}





bool cPickup::TryCombineWithQueuedEntity(cEntity & a_Entity, const cBoundingBox & a_CombineBounds, cItem & a_Item, const char a_MaxStackSize)
{
	if (!a_Entity.IsPickup())
	{
		return false;
	}

	auto & Pickup = static_cast<cPickup &>(a_Entity);
	auto & RecipientItem = Pickup.GetItem();

	if (!RecipientItem.IsEqual(a_Item) || !a_CombineBounds.DoesIntersect(Pickup.GetBoundingBox()))
	{
		return false;
	}

	PickupCombiningCallback::CombineInto(RecipientItem, a_Item, a_MaxStackSize);

	return a_Item.m_ItemCount == 0;
}





void cPickup::OnCollectedBy(cPawn & a_Collector, char a_CollectionCount)
{
	m_Item.m_ItemCount -= a_CollectionCount;

	if (m_Item.m_ItemCount <= 0)
	{
		// All of the pickup has been collected, schedule the pickup for destroying:
		m_IsCollected = true;

		// Play the collection animation (only when fully collected):
		m_World->BroadcastCollectEntity(*this, a_Collector, static_cast<unsigned>(a_CollectionCount));

		// Wait 0.5s for collection animation to play:
		m_RemainingLifetime = std::chrono::milliseconds(500);
	}
	else
	{
		// Our count changed, so try to combine again:
		TryCombineWithPickupsInWorld();
	}

	// Also send the "pop" sound effect with a somewhat random pitch (fast-random using EntityID ;)
	m_World->BroadcastSoundEffect("entity.item.pickup", GetPosition(), 0.3f, (1.2f + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));
}





void cPickup::TryCombineWithPickupsInWorld()
{
	if (!m_IsCombinable)
	{
		return;
	}

	PickupCombiningCallback PickupCombiningCallback(this);
	m_World->ForEachEntityInBox({ GetPosition(), 0.25 / 2, 0.25 }, PickupCombiningCallback);
}





bool cPickup::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType == dtCactusContact)
	{
		Destroy();
		return true;
	}

	return Super::DoTakeDamage(a_TDI);
}





void cPickup::OnAddToWorld(cWorld & a_World)
{
	Super::OnAddToWorld(a_World);

	// Say when going through a portal, try to combine:
	TryCombineWithPickupsInWorld();
}





void cPickup::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnEntity(*this);
	a_Client.SendEntityMetadata(*this);
}





void cPickup::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);

	if (!IsTicking())
	{
		// The base class tick destroyed us:
		return;
	}

	BroadcastMovementUpdate();  // Notify clients of position.

	m_RemainingCollectionDelay -= a_Dt;
	m_RemainingLifetime -= a_Dt;

	if (m_RemainingLifetime <= m_RemainingLifetime.zero())
	{
		Destroy();
		return;
	}

	if (m_IsCollected)
	{
		return;
	}

	// Don't combine if already full, we haven't moved, or combination is disabled:
	if ((m_LastPosition == GetPosition()) || (m_Item.m_ItemCount >= m_Item.GetMaxStackSize()))
	{
		return;
	}

	// Try to combine the pickup with adjacent same-item pickups:
	TryCombineWithPickupsInWorld();
}
