
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
#include <cstdlib>
#endif

#include "Pickup.h"
#include "Player.h"
#include "../ClientHandle.h"
#include "../World.h"
#include "../Server.h"
#include "../Bindings/PluginManager.h"
#include "../Root.h"
#include "../Chunk.h"




class cPickupCombiningCallback
{
public:
	cPickupCombiningCallback(Vector3d a_Position, cPickup * a_Pickup) :
		m_FoundMatchingPickup(false),
		m_Position(a_Position),
		m_Pickup(a_Pickup)
	{
	}

	bool operator () (cEntity & a_Entity)
	{
		ASSERT(a_Entity.IsTicking());
		if (!a_Entity.IsPickup() || (a_Entity.GetUniqueID() <= m_Pickup->GetUniqueID()) || !a_Entity.IsOnGround())
		{
			return false;
		}


		Vector3d EntityPos = a_Entity.GetPosition();
		double Distance = (EntityPos - m_Position).Length();

		auto & OtherPickup = static_cast<cPickup &>(a_Entity);
		cItem & Item = OtherPickup.GetItem();
		if ((Distance < 1.2) && Item.IsEqual(m_Pickup->GetItem()) && OtherPickup.CanCombine())
		{
			short CombineCount = Item.m_ItemCount;
			if ((CombineCount + m_Pickup->GetItem().m_ItemCount) > Item.GetMaxStackSize())
			{
				CombineCount = Item.GetMaxStackSize() - m_Pickup->GetItem().m_ItemCount;
			}

			if (CombineCount <= 0)
			{
				return false;
			}

			m_Pickup->GetItem().AddCount(static_cast<char>(CombineCount));
			Item.m_ItemCount -= CombineCount;

			if (Item.m_ItemCount <= 0)
			{
				/* Experimental: show animation pickups getting together */
				auto Diff = (m_Pickup->GetPosition() * 32.0).Floor() - (EntityPos * 32.0).Floor();
				a_Entity.GetWorld()->BroadcastEntityRelMove(a_Entity, Vector3<char>(Diff));
				/* End of experimental animation */
				a_Entity.Destroy();

				// Reset the timer
				m_Pickup->SetAge(0);
			}
			else
			{
				a_Entity.GetWorld()->BroadcastEntityMetadata(a_Entity);
			}
			m_FoundMatchingPickup = true;
		}
		return false;
	}

	inline bool FoundMatchingPickup()
	{
		return m_FoundMatchingPickup;
	}

protected:
	bool m_FoundMatchingPickup;

	Vector3d m_Position;
	cPickup * m_Pickup;
};





////////////////////////////////////////////////////////////////////////////////
// cPickup:

cPickup::cPickup(Vector3d a_Pos, const cItem & a_Item, bool IsPlayerCreated, Vector3f a_Speed, int a_LifetimeTicks, bool a_CanCombine):
	Super(etPickup, a_Pos, 0.2, 0.2),
	m_Timer(0),
	m_Item(a_Item),
	m_bCollected(false),
	m_bIsPlayerCreated(IsPlayerCreated),
	m_bCanCombine(a_CanCombine),
	m_Lifetime(cTickTime(a_LifetimeTicks))
{
	SetGravity(-16.0f);
	SetAirDrag(0.02f);
	SetMaxHealth(5);
	SetHealth(5);
	SetSpeed(a_Speed);
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
		// The base class tick destroyed us
		return;
	}
	BroadcastMovementUpdate();  // Notify clients of position

	m_Timer += a_Dt;

	if (!m_bCollected)
	{
		int BlockY = POSY_TOINT;
		int BlockX = POSX_TOINT;
		int BlockZ = POSZ_TOINT;

		if ((BlockY >= 0) && (BlockY < cChunkDef::Height))  // Don't do anything except for falling when outside the world
		{
			// Position might have changed due to physics. So we have to make sure we have the correct chunk.
			GET_AND_VERIFY_CURRENT_CHUNK(CurrentChunk, BlockX, BlockZ);

			// Destroy the pickup if it is on fire:
			if (IsOnFire())
			{
				m_bCollected = true;
				m_Timer = std::chrono::milliseconds(0);  // We have to reset the timer.
				m_Timer += a_Dt;  // In case we have to destroy the pickup in the same tick.
				if (m_Timer > std::chrono::milliseconds(500))
				{
					Destroy();
					return;
				}
			}

			// Try to combine the pickup with adjacent same-item pickups:
			if ((m_Item.m_ItemCount < m_Item.GetMaxStackSize()) && IsOnGround() && CanCombine())  // Don't combine if already full or not on ground
			{
				// By using a_Chunk's ForEachEntity() instead of cWorld's, pickups don't combine across chunk boundaries.
				// That is a small price to pay for not having to traverse the entire world for each entity.
				// The speedup in the tick thread is quite considerable.
				cPickupCombiningCallback PickupCombiningCallback(GetPosition(), this);
				a_Chunk.ForEachEntity(PickupCombiningCallback);
				if (PickupCombiningCallback.FoundMatchingPickup())
				{
					m_World->BroadcastEntityMetadata(*this);
				}
			}
		}
	}
	else
	{
		if (m_Timer > std::chrono::milliseconds(500))  // 0.5 second
		{
			Destroy();
			return;
		}
	}

	if (m_Timer > m_Lifetime)
	{
		Destroy();
		return;
	}
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





bool cPickup::CollectedBy(cPlayer & a_Dest)
{
	if (m_bCollected)
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because it has already been collected.", m_UniqueID, a_Dest->GetName().c_str());
		return false;  // It's already collected!
	}

	// Two seconds if player created the pickup (vomiting), half a second if anything else
	if (m_Timer < (m_bIsPlayerCreated ? std::chrono::seconds(2) : std::chrono::milliseconds(500)))
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because it is not old enough.", m_UniqueID, a_Dest->GetName().c_str());
		return false;  // Not old enough
	}

	// If the player is a spectator, he cannot collect anything
	if (a_Dest.IsGameModeSpectator())
	{
		return false;
	}

	if (cRoot::Get()->GetPluginManager()->CallHookCollectingPickup(a_Dest, *this))
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because a plugin has said no.", m_UniqueID, a_Dest->GetName().c_str());
		return false;
	}

	int NumAdded = a_Dest.GetInventory().AddItem(m_Item);
	if (NumAdded > 0)
	{
		// Check achievements
		switch (m_Item.m_ItemType)
		{
			case E_BLOCK_LOG:      a_Dest.AwardAchievement(achMineWood); break;
			case E_ITEM_LEATHER:   a_Dest.AwardAchievement(achKillCow);  break;
			case E_ITEM_DIAMOND:   a_Dest.AwardAchievement(achDiamonds); break;
			case E_ITEM_BLAZE_ROD: a_Dest.AwardAchievement(achBlazeRod); break;
			default: break;
		}

		m_Item.m_ItemCount -= NumAdded;
		m_World->BroadcastCollectEntity(*this, a_Dest, NumAdded);

		// Also send the "pop" sound effect with a somewhat random pitch (fast-random using EntityID ;)
		m_World->BroadcastSoundEffect("entity.item.pickup", GetPosition(), 0.3f, (1.2f + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));
		if (m_Item.m_ItemCount <= 0)
		{
			// All of the pickup has been collected, schedule the pickup for destroying
			m_bCollected = true;
		}
		m_Timer = std::chrono::milliseconds(0);
		return true;
	}

	// LOG("Pickup %d cannot be collected by \"%s\", because there's no space in the inventory.", a_Dest->GetName().c_str(), m_UniqueID);
	return false;
}
