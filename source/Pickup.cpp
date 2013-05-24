
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifndef _WIN32
#include <cstdlib>
#endif

#include "Pickup.h"
#include "ClientHandle.h"
#include "Inventory.h"
#include "World.h"
#include "Simulator/FluidSimulator.h"
#include "Server.h"
#include "Player.h"
#include "PluginManager.h"
#include "Item.h"
#include "Root.h"
#include "Tracer.h"
#include "Chunk.h"

#include "Vector3d.h"
#include "Vector3f.h"





cPickup::cPickup(int a_MicroPosX, int a_MicroPosY, int a_MicroPosZ, const cItem & a_Item, float a_SpeedX /* = 0.f */, float a_SpeedY /* = 0.f */, float a_SpeedZ /* = 0.f */)
	:	cEntity(etPickup, ((double)(a_MicroPosX)) / 32, ((double)(a_MicroPosY)) / 32, ((double)(a_MicroPosZ)) / 32)
	, m_Health(5)
	, m_Timer( 0.f )
	, m_Item(a_Item)
	, m_bCollected( false )
{
	SetSpeed(a_SpeedX, a_SpeedY, a_SpeedZ);
	m_Gravity = -3.0;
}





void cPickup::Initialize(cWorld * a_World)
{
	super::Initialize(a_World);
	a_World->BroadcastSpawn(*this);
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
		if (BlockY < cChunkDef::Height)  // Don't do anything except for falling when above the world
		{
			int BlockX = (int) floor(GetPosX());
			int BlockZ = (int) floor(GetPosZ());
			//Position might have changed due to physics. So we have to make sure we have the correct chunk.
			cChunk * CurrentChunk = a_Chunk.GetNeighborChunk(BlockX, BlockZ);
			if (CurrentChunk != NULL)  // Make sure the chunk is loaded
			{
				int RelBlockX = BlockX - (CurrentChunk->GetPosX() * cChunkDef::Width);
				int RelBlockZ = BlockZ - (CurrentChunk->GetPosZ() * cChunkDef::Width);
				
				BLOCKTYPE BlockBelow = CurrentChunk->GetBlock(RelBlockX, BlockY - 1, RelBlockZ);
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
						Destroy();
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
			Destroy();
			return;
		}
	}

	if (m_Timer > 1000 * 60 * 5)  // 5 minutes
	{
		Destroy();
		return;
	}

	if (GetPosY() < -8) // Out of this world and no more visible!
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




