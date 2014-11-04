
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




class cPickupCombiningCallback :
	public cEntityCallback
{
public:
	cPickupCombiningCallback(Vector3d a_Position, cPickup * a_Pickup) :
		m_FoundMatchingPickup(false),
		m_Position(a_Position),
		m_Pickup(a_Pickup)
	{
	}

	virtual bool Item(cEntity * a_Entity) override
	{
		if (!a_Entity->IsPickup() || (a_Entity->GetUniqueID() <= m_Pickup->GetUniqueID()) || a_Entity->IsDestroyed())
		{
			return false;
		}

		Vector3d EntityPos = a_Entity->GetPosition();
		double Distance = (EntityPos - m_Position).Length();

		cItem & Item = ((cPickup *)a_Entity)->GetItem();
		if ((Distance < 1.2) && Item.IsEqual(m_Pickup->GetItem()))
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

			m_Pickup->GetItem().AddCount((char)CombineCount);
			Item.m_ItemCount -= CombineCount;

			if (Item.m_ItemCount <= 0)
			{
				a_Entity->Destroy();
			}
			else
			{
				a_Entity->GetWorld()->BroadcastEntityMetadata(*a_Entity);
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





cPickup::cPickup(CreateEntityInfo a_Info, const cItem & a_Item, bool IsPlayerCreated)
	: cEntity(etPickup, a_Info, 0.2, 0.2, 5),
	m_DestroyTimer(0),
	m_Item(a_Item),
	m_bCollected(false),
	m_bIsPlayerCreated(IsPlayerCreated)
{
	SetGravity(-10.5f);
}





void cPickup::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendPickupSpawn(*this);
}





void cPickup::Tick(float a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);

	super::Tick(a_Dt, a_Chunk);
	BroadcastMovementUpdate();  // Notify clients of position
	
	if (!m_bCollected)
	{
		int BlockY = POSY_TOINT;
		int BlockX = POSX_TOINT;
		int BlockZ = POSZ_TOINT;

		if ((BlockY >= 0) && (BlockY < cChunkDef::Height))  // Don't do anything except for falling when outside the world
		{
			// Position might have changed due to physics. So we have to make sure we have the correct chunk.
			GET_AND_VERIFY_CURRENT_CHUNK(CurrentChunk, BlockX, BlockZ)
			
			int RelBlockX = BlockX - (CurrentChunk->GetPosX() * cChunkDef::Width);
			int RelBlockZ = BlockZ - (CurrentChunk->GetPosZ() * cChunkDef::Width);
				
			// If the pickup is on the bottommost block position, make it think the void is made of air: (#131)
			BLOCKTYPE BlockBelow = (BlockY > 0) ? CurrentChunk->GetBlock(RelBlockX, BlockY - 1, RelBlockZ) : E_BLOCK_AIR;
			BLOCKTYPE BlockIn = CurrentChunk->GetBlock(RelBlockX, BlockY, RelBlockZ);

			if (
				IsBlockLava(BlockBelow) || (BlockBelow == E_BLOCK_FIRE) ||
				IsBlockLava(BlockIn) || (BlockIn == E_BLOCK_FIRE)
			)
			{
				m_bCollected = true;
				Destroy();
				return;
			}

			// Try to combine the pickup with adjacent same-item pickups:
			if (!IsDestroyed() && (m_Item.m_ItemCount < m_Item.GetMaxStackSize()))  // Don't combine if already full
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
		// Playing collect animation, wait until it's complete
		m_DestroyTimer++;

		if (m_DestroyTimer == 10)  // Animation lasts around 0.5 seconds
		{
			Destroy();
			return;
		}
	}

	if (GetTicksAlive() >= 20 * 60 * 5)  // 5 minutes
	{
		Destroy();
		return;
	}

	if (GetPosY() < VOID_BOUNDARY)  // Out of this world and no more visible!
	{
		Destroy();
		return;
	}
}





bool cPickup::CollectedBy(cPlayer * a_Dest)
{
	ASSERT(a_Dest != NULL);
	
	if (m_bCollected)
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because it has already been collected.", m_UniqueID, a_Dest->GetName().c_str());
		return false;  // It's already collected!
	}
	
	// Two seconds if player created the pickup (vomiting), half a second if anything else
	if (GetTicksAlive() < (m_bIsPlayerCreated ? 40 : 10))
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because it is not old enough.", m_UniqueID, a_Dest->GetName().c_str());
		return false;  // Not old enough
	}

	if (cRoot::Get()->GetPluginManager()->CallHookCollectingPickup(a_Dest, *this))
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because a plugin has said no.", m_UniqueID, a_Dest->GetName().c_str());
		return false;
	}

	int NumAdded = a_Dest->GetInventory().AddItem(m_Item);
	if (NumAdded > 0)
	{
		// Check achievements
		switch (m_Item.m_ItemType)
		{
			case E_BLOCK_LOG:      a_Dest->AwardAchievement(achMineWood); break;
			case E_ITEM_LEATHER:   a_Dest->AwardAchievement(achKillCow);  break;
			case E_ITEM_DIAMOND:   a_Dest->AwardAchievement(achDiamonds); break;
			case E_ITEM_BLAZE_ROD: a_Dest->AwardAchievement(achBlazeRod); break;
			default: break;
		}

		m_Item.m_ItemCount -= NumAdded;
		m_World->BroadcastCollectEntity(*this, *a_Dest);
		// Also send the "pop" sound effect with a somewhat random pitch (fast-random using EntityID ;)
		m_World->BroadcastSoundEffect("random.pop", GetPosX(), GetPosY(), GetPosZ(), 0.5, (float)(0.75 + ((float)((GetUniqueID() * 23) % 32)) / 64));
		if (m_Item.m_ItemCount <= 0)
		{
			// All of the pickup has been collected, schedule the pickup for destroying
			m_bCollected = true;
		}
		return true;
	}

	// LOG("Pickup %d cannot be collected by \"%s\", because there's no space in the inventory.", a_Dest->GetName().c_str(), m_UniqueID);
	return false;
}




