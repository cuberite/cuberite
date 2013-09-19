
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
#include <cstdlib>
#endif

#include "Pickup.h"
#include "../ClientHandle.h"
#include "../Inventory.h"
#include "../World.h"
#include "../Simulator/FluidSimulator.h"
#include "../Server.h"
#include "Player.h"
#include "../PluginManager.h"
#include "../Item.h"
#include "../Root.h"
#include "../Chunk.h"

#include "../Vector3d.h"
#include "../Vector3f.h"





cPickup::cPickup(double a_X, double a_Y, double a_Z, const cItem & a_Item, float a_SpeedX /* = 0.f */, float a_SpeedY /* = 0.f */, float a_SpeedZ /* = 0.f */)
	:	cEntity(etPickup, a_X, a_Y, a_Z, 0.2, 0.2)
	, m_Timer( 0.f )
	, m_Item(a_Item)
	, m_bCollected( false )
{
	m_MaxHealth = 5;
	m_Health = 5;
	SetSpeed(a_SpeedX, a_SpeedY, a_SpeedZ);
}





void cPickup::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendPickupSpawn(*this);	
}





void cPickup::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	BroadcastMovementUpdate(); //Notify clients of position

	m_Timer += a_Dt;
	
	if (!m_bCollected)
	{
		int BlockY = (int) floor(GetPosY());
		if ((BlockY >= 0) && (BlockY < cChunkDef::Height))  // Don't do anything except for falling when outside the world
		{
			int BlockX = (int) floor(GetPosX());
			int BlockZ = (int) floor(GetPosZ());
			// Position might have changed due to physics. So we have to make sure we have the correct chunk.
			cChunk * CurrentChunk = a_Chunk.GetNeighborChunk(BlockX, BlockZ);
			if (CurrentChunk != NULL)  // Make sure the chunk is loaded
			{
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
					m_Timer = 0;  // We have to reset the timer.
					m_Timer += a_Dt;  // In case we have to destroy the pickup in the same tick.
					if (m_Timer > 500.f)  
					{
						Destroy(true);
						return;
					}
				}
			}
		}
	}
	else
	{
		if (m_Timer > 500.f)  // 0.5 second
		{
			Destroy(true);
			return;
		}
	}

	if (m_Timer > 1000 * 60 * 5)  // 5 minutes
	{
		Destroy(true);
		return;
	}

	if (GetPosY() < -8) // Out of this world and no more visible!
	{
		Destroy(true);
		return;
	}
}





bool cPickup::CollectedBy(cPlayer * a_Dest)
{
	ASSERT(a_Dest != NULL);
	
	if (m_bCollected)
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because it has already been collected.", m_UniqueID, a_Dest->GetName().c_str());
		return false; // It's already collected!
	}
	
	// 800 is to long
	if (m_Timer < 500.f)
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because it is not old enough.", m_UniqueID, a_Dest->GetName().c_str());
		return false; // Not old enough
	}

	if (cRoot::Get()->GetPluginManager()->CallHookCollectingPickup(a_Dest, *this))
	{
		// LOG("Pickup %d cannot be collected by \"%s\", because a plugin has said no.", m_UniqueID, a_Dest->GetName().c_str());
		return false;
	}

	int NumAdded = a_Dest->GetInventory().AddItem(m_Item);
	if (NumAdded > 0)
	{
		m_Item.m_ItemCount -= NumAdded;
		m_World->BroadcastCollectPickup(*this, *a_Dest);
		if (m_Item.m_ItemCount == 0)
		{
			// All of the pickup has been collected, schedule the pickup for destroying
			m_bCollected = true;
		}
		m_Timer = 0;
		return true;
	}

	// LOG("Pickup %d cannot be collected by \"%s\", because there's no space in the inventory.", a_Dest->GetName().c_str(), m_UniqueID);
	return false;
}




