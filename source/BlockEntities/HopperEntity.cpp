
// HopperEntity.cpp

// Implements the cHopperEntity representing a hopper block entity

#include "Globals.h"
#include "HopperEntity.h"
#include "../Chunk.h"
#include "../Player.h"
#include "ChestEntity.h"
#include "DropSpenserEntity.h"
#include "FurnaceEntity.h"





cHopperEntity::cHopperEntity(int a_BlockX, int a_BlockY, int a_BlockZ) :
	super(E_BLOCK_HOPPER, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, NULL),
	m_LastMoveItemsInTick(0),
	m_LastMoveItemsOutTick(0)
{
}





cHopperEntity::cHopperEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_HOPPER, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World),
	m_LastMoveItemsInTick(0),
	m_LastMoveItemsOutTick(0)
{
}





/** Returns the block coords of the block receiving the output items, based on the meta
Returns false if unattached
*/
bool cHopperEntity::GetOutputBlockPos(NIBBLETYPE a_BlockMeta, int & a_OutputX, int & a_OutputY, int & a_OutputZ)
{
	a_OutputX = m_PosX;
	a_OutputY = m_PosY;
	a_OutputZ = m_PosZ;
	switch (a_BlockMeta)
	{
		case E_META_HOPPER_FACING_XM: a_OutputX--; return true;
		case E_META_HOPPER_FACING_XP: a_OutputX++; return true;
		case E_META_HOPPER_FACING_YM: a_OutputY--; return true;
		case E_META_HOPPER_FACING_ZM: a_OutputZ--; return true;
		case E_META_HOPPER_FACING_ZP: a_OutputZ++; return true;
		default:
		{
			// Not attached
			return false;
		}
	}
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
		case E_BLOCK_CHEST:       res = MoveItemsFromChest(a_Chunk); break;
		case E_BLOCK_FURNACE:     res = MoveItemsFromFurnace(a_Chunk); break;
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:     res = MoveItemsFromGrid(((cDropSpenserEntity *)a_Chunk.GetBlockEntity(m_PosX, m_PosY + 1, m_PosZ))->GetContents()); break;
		case E_BLOCK_HOPPER:      res = MoveItemsFromGrid(((cHopperEntity *)     a_Chunk.GetBlockEntity(m_PosX, m_PosY + 1, m_PosZ))->GetContents()); break;
		case E_BLOCK_LIT_FURNACE: res = MoveItemsFromFurnace(a_Chunk); break;
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
	
	int bx, by, bz;
	NIBBLETYPE Meta = a_Chunk.GetMeta(m_RelX, m_PosY, m_RelZ);
	if (!GetOutputBlockPos(Meta, bx, by, bz))
	{
		// Not attached to another container
		return false;
	}
	if (by < 0)
	{
		// Cannot output below the zero-th block level
		return false;
	}
	
	// Convert coords to relative:
	int rx = bx - a_Chunk.GetPosX() * cChunkDef::Width;
	int rz = bz - a_Chunk.GetPosZ() * cChunkDef::Width;
	cChunk * DestChunk = a_Chunk.GetRelNeighborChunkAdjustCoords(rx, rz);
	if (DestChunk == NULL)
	{
		// The destination chunk has been unloaded, don't tick
		return false;
	}
	
	// Call proper moving function, based on the blocktype present at the coords:
	bool res = false;
	switch (DestChunk->GetBlock(rx, by, rz))
	{
		case E_BLOCK_CHEST:       res = MoveItemsToChest(*DestChunk, bx, by, bz); break;
		case E_BLOCK_FURNACE:     res = MoveItemsToFurnace(*DestChunk, bx, by, bz, Meta); break;
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:     res = MoveItemsToGrid(((cDropSpenserEntity *)DestChunk->GetBlockEntity(bx, by, bz))->GetContents()); break;
		case E_BLOCK_HOPPER:      res = MoveItemsToGrid(((cHopperEntity *)     DestChunk->GetBlockEntity(bx, by, bz))->GetContents()); break;
		case E_BLOCK_LIT_FURNACE: res = MoveItemsToFurnace(*DestChunk, bx, by, bz, Meta); break;
	}
	
	// If the item has been moved, reset the last tick:
	if (res)
	{
		m_LastMoveItemsOutTick = a_CurrentTick;
	}
	
	return res;
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
	cFurnaceEntity * Furnace = (cFurnaceEntity *)a_Chunk.GetBlockEntity(m_PosX, m_PosY + 1, m_PosZ);
	ASSERT(Furnace != NULL);
	
	// Try move from the output slot:
	if (MoveItemsFromSlot(Furnace->GetOutputSlot(), true))
	{
		cItem NewOutput(Furnace->GetOutputSlot());
		Furnace->SetOutputSlot(NewOutput.AddCount(-1));
		return true;
	}
	
	// No output moved, check if we can move an empty bucket out of the fuel slot:
	if (Furnace->GetFuelSlot().m_ItemType == E_ITEM_BUCKET)
	{
		if (MoveItemsFromSlot(Furnace->GetFuelSlot(), true))
		{
			Furnace->SetFuelSlot(cItem());
			return true;
		}
	}
	
	// Nothing can be moved
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
	cItem One(a_ItemStack.CopyOne());
	if (m_Contents.AddItem(One, a_AllowNewStacks) > 0)
	{
		return true;
	}
	return false;
}





/// Moves items to the chest at the specified coords. Returns true if contents have changed
bool cHopperEntity::MoveItemsToChest(cChunk & a_Chunk, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Try the chest directly connected to the hopper:
	if (MoveItemsToGrid(((cChestEntity *)a_Chunk.GetBlockEntity(a_BlockX, a_BlockY, a_BlockZ))->GetContents()))
	{
		return true;
	}

	// Check if the chest is a double-chest, if so, try to move into the other half:
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
		if (MoveItemsToGrid(((cChestEntity *)Neighbor->GetBlockEntity(a_BlockX, a_BlockY, a_BlockZ))->GetContents()))
		{
			return true;
		}
		return false;
	}
	
	// The chest was single and nothing could be moved
	return false;
}





/// Moves items to the furnace at the specified coords. Returns true if contents have changed
bool cHopperEntity::MoveItemsToFurnace(cChunk & a_Chunk, int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_HopperMeta)
{
	cFurnaceEntity * Furnace = (cFurnaceEntity *)a_Chunk.GetBlockEntity(a_BlockX, a_BlockY, a_BlockZ);
	if (a_HopperMeta == E_META_HOPPER_FACING_YM)
	{
		// Feed the input slot of the furnace
		return MoveItemsToSlot(Furnace->GetContents(), cFurnaceEntity::fsInput);
	}
	else
	{
		// Feed the fuel slot of the furnace
		return MoveItemsToSlot(Furnace->GetContents(), cFurnaceEntity::fsFuel);
	}
	return false;
}





/// Moves items to the specified ItemGrid. Returns true if contents have changed
bool cHopperEntity::MoveItemsToGrid(cItemGrid & a_ItemGrid)
{
	// Iterate through our slots, try to move from each one:
	for (int i = 0; i < ContentsWidth * ContentsHeight; i++)
	{
		const cItem & SrcItem = m_Contents.GetSlot(i);
		if (SrcItem.IsEmpty())
		{
			continue;
		}
		
		cItem ToAdd = SrcItem.CopyOne();
		if (a_ItemGrid.AddItem(ToAdd) > 0)
		{
			m_Contents.ChangeSlotCount(i, -1);
			return true;
		}
	}
	return false;
}





/// Moves one piece to the specified ItemGrid's slot. Returns true if contents have changed.
bool cHopperEntity::MoveItemsToSlot(cItemGrid & a_ItemGrid, int a_DestSlotNum)
{
	if (a_ItemGrid.IsSlotEmpty(a_DestSlotNum))
	{
		// The slot is empty, move the first non-empty slot from our contents:
		for (int i = 0; i < ContentsWidth * ContentsHeight; i++)
		{
			if (!m_Contents.IsSlotEmpty(i))
			{
				a_ItemGrid.SetSlot(a_DestSlotNum, m_Contents.GetSlot(i).CopyOne());
				m_Contents.ChangeSlotCount(i, -1);
				return true;
			}
		}
		return false;
	}
	else
	{
		// The slot is taken, try to top it up:
		const cItem & DestSlot = a_ItemGrid.GetSlot(a_DestSlotNum);
		if (DestSlot.IsFullStack())
		{
			return false;
		}
		for (int i = 0; i < ContentsWidth * ContentsHeight; i++)
		{
			if (m_Contents.GetSlot(i).IsStackableWith(DestSlot))
			{
				a_ItemGrid.ChangeSlotCount(a_DestSlotNum, 1);
				m_Contents.ChangeSlotCount(i, -1);
				return true;
			}
		}
		return false;
	}
}




