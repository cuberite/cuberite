
// HopperEntity.cpp

// Implements the cHopperEntity representing a hopper block entity

#include "Globals.h"
#include "HopperEntity.h"
#include "../Chunk.h"
#include "../Player.h"
#include "ChestEntity.h"
#include "DropSpenserEntity.h"





cHopperEntity::cHopperEntity(int a_BlockX, int a_BlockY, int a_BlockZ) :
	super(E_BLOCK_HOPPER, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, NULL)
{
}





cHopperEntity::cHopperEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_HOPPER, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World)
{
}





bool cHopperEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	Int64 CurrentTick = a_Chunk.GetWorld()->GetWorldAge();
	
	bool res = false;
	res = MoveItemsIn  (a_Chunk, CurrentTick) || res;
	res = MovePickupsIn(a_Chunk, CurrentTick) || res;
	res = MoveItemsOut (a_Chunk, CurrentTick) || res;
	return res;
}





void cHopperEntity::SaveToJson(Json::Value & a_Value)
{
	// TODO
	LOGWARNING("%s: Not implemented yet", __FUNCTION__);
}





void cHopperEntity::SendTo(cClientHandle & a_Client)
{
	// The hopper entity doesn't need anything sent to the client when it's created / gets in the viewdistance
	// All the actual handling is in the cWindow UI code that gets called when the hopper is rclked
	
	UNUSED(a_Client);
}





void cHopperEntity::UsedBy(cPlayer * a_Player)
{
	// If the window is not created, open it anew:
	cWindow * Window = GetWindow();
	if (Window == NULL)
	{
		OpenNewWindow();
		Window = GetWindow();
	}
	
	// Open the window for the player:
	if (Window != NULL)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(Window);
		}
	}

	// This is rather a hack
	// Instead of marking the chunk as dirty upon chest contents change, we mark it dirty now
	// We cannot properly detect contents change, but such a change doesn't happen without a player opening the chest first.
	// The few false positives aren't much to worry about
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(m_PosX, m_PosY, m_PosZ, ChunkX, ChunkZ);
	m_World->MarkChunkDirty(ChunkX, ChunkZ);
}





/// Opens a new window UI for this hopper
void cHopperEntity::OpenNewWindow(void)
{
	OpenWindow(new cHopperWindow(m_PosX, m_PosY, m_PosZ, this));
}





/// Moves items from the container above it into this hopper. Returns true if the contents have changed.
bool cHopperEntity::MoveItemsIn(cChunk & a_Chunk, Int64 a_CurrentTick)
{
	if (m_PosY >= cChunkDef::Height)
	{
		// This hopper is at the top of the world, no more blocks above
		return false;
	}

	if (a_CurrentTick - m_LastMoveItemsInTick < TICKS_PER_TRANSFER)
	{
		// Too early after the previous transfer
		return false;
	}
	
	// Try moving an item in:
	bool res = false;
	switch (a_Chunk.GetBlock(m_RelX, m_PosY + 1, m_RelZ))
	{
		case E_BLOCK_CHEST:     res = MoveItemsFromChest(a_Chunk); break;
		case E_BLOCK_FURNACE:   res = MoveItemsFromFurnace(a_Chunk); break;
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:   res = MoveItemsFromGrid(((cDropSpenserEntity *)a_Chunk.GetBlockEntity(m_PosX, m_PosY + 1, m_PosZ))->GetContents()); break;
		case E_BLOCK_HOPPER:    res = MoveItemsFromGrid(((cHopperEntity *)     a_Chunk.GetBlockEntity(m_PosX, m_PosY + 1, m_PosZ))->GetContents()); break;
	}
	
	// If the item has been moved, reset the last tick:
	if (res)
	{
		m_LastMoveItemsInTick = a_CurrentTick;
	}
	
	return res;
}





/// Moves pickups from above this hopper into it. Returns true if the contents have changed.
bool cHopperEntity::MovePickupsIn(cChunk & a_Chunk, Int64 a_CurrentTick)
{
	// TODO
	return false;
}





/// Moves items out from this hopper into the destination. Returns true if the contents have changed.
bool cHopperEntity::MoveItemsOut(cChunk & a_Chunk, Int64 a_CurrentTick)
{
	if (a_CurrentTick - m_LastMoveItemsOutTick < TICKS_PER_TRANSFER)
	{
		// Too early after the previous transfer
		return false;
	}
	
	// TODO
	return false;
}





/// Moves items from a chest (dblchest) above the hopper into this hopper. Returns true if contents have changed.
bool cHopperEntity::MoveItemsFromChest(cChunk & a_Chunk)
{
	if (MoveItemsFromGrid(((cChestEntity *)a_Chunk.GetBlockEntity(m_PosX, m_PosY + 1, m_PosZ))->GetContents()))
	{
		// Moved the item from the chest directly above the hopper
		return true;
	}
	
	// Check if the chest is a double-chest, if so, try to move from there:
	static const struct
	{
		int x, z;
	}
	Coords [] =
	{
		{1, 0},
		{-1, 0},
		{0, 1},
		{0, -1},
	} ;
	for (int i = 0; i < ARRAYCOUNT(Coords); i++)
	{
		int x = m_RelX + Coords[i].x;
		int z = m_RelZ + Coords[i].z;
		cChunk * Neighbor = a_Chunk.GetRelNeighborChunkAdjustCoords(x, z);
		if (
			(Neighbor == NULL) ||
			(Neighbor->GetBlock(x, m_PosY + 1, z) != E_BLOCK_CHEST)
		)
		{
			continue;
		}
		if (MoveItemsFromGrid(((cChestEntity *)Neighbor->GetBlockEntity(x, m_PosY, z))->GetContents()))
		{
			return true;
		}
		return false;
	}
	
	// The chest was single and nothing could be moved
	return false;
}





/// Moves items from a furnace above the hopper into this hopper. Returns true if contents have changed.
bool cHopperEntity::MoveItemsFromFurnace(cChunk & a_Chunk)
{
	// TODO
	return false;
}





/// Moves items from the specified ItemGrid into this hopper. Returns true if contents have changed.
bool cHopperEntity::MoveItemsFromGrid(cItemGrid & a_Grid)
{
	int NumSlots = a_Grid.GetNumSlots();
	
	// First try adding items of types already in the hopper:
	for (int i = 0; i < NumSlots; i++)
	{
		if (a_Grid.IsSlotEmpty(i))
		{
			continue;
		}
		if (MoveItemsFromSlot(a_Grid.GetSlot(i), false))
		{
			a_Grid.ChangeSlotCount(i, -1);
			return true;
		}
	}

	// No already existing stack can be topped up, try again with allowing new stacks:
	for (int i = 0; i < NumSlots; i++)
	{
		if (a_Grid.IsSlotEmpty(i))
		{
			continue;
		}
		if (MoveItemsFromSlot(a_Grid.GetSlot(i), true))
		{
			a_Grid.ChangeSlotCount(i, -1);
			return true;
		}
	}
	return false;
}





/// Moves one of the specified itemstack into this hopper. Returns true if contents have changed. Doesn't change the itemstack.
bool cHopperEntity::MoveItemsFromSlot(const cItem & a_ItemStack, bool a_AllowNewStacks)
{
	if (m_Contents.AddItem(a_ItemStack.CopyOne(), a_AllowNewStacks) > 0)
	{
		return true;
	}
	return false;
}




