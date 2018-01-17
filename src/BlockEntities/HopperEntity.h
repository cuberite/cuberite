
// HopperEntity.h

// Declares the cHopperEntity representing a hopper block entity





#pragma once

#include "BlockEntityWithItems.h"





// tolua_begin
class cHopperEntity :
	public cBlockEntityWithItems
{
	typedef cBlockEntityWithItems Super;

public:
	enum
	{
		ContentsHeight = 1,
		ContentsWidth  = 5,
		TICKS_PER_TRANSFER = 8,  ///< How many ticks at minimum between two item transfers to or from the hopper
	} ;

	// tolua_end

	BLOCKENTITY_PROTODEF(cHopperEntity)

	/** Constructor used for normal operation */
	cHopperEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	/** Returns the block coords of the block receiving the output items, based on the meta
	Returns false if unattached.
	Exported in ManualBindings.cpp. */
	bool GetOutputBlockPos(NIBBLETYPE a_BlockMeta, int & a_OutputX, int & a_OutputY, int & a_OutputZ);

protected:

	Int64 m_LastMoveItemsInTick;
	Int64 m_LastMoveItemsOutTick;

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;

	/** Opens a new chest window for this chest. Scans for neighbors to open a double chest window, if appropriate. */
	void OpenNewWindow(void);

	/** Moves items from the container above it into this hopper. Returns true if the contents have changed. */
	bool MoveItemsIn(cChunk & a_Chunk, Int64 a_CurrentTick);

	/** Moves pickups from above this hopper into it. Returns true if the contents have changed. */
	bool MovePickupsIn(cChunk & a_Chunk, Int64 a_CurrentTick);

	/** Moves items out from this hopper into the destination. Returns true if the contents have changed. */
	bool MoveItemsOut(cChunk & a_Chunk, Int64 a_CurrentTick);

	/** Moves items from a chest (dblchest) above the hopper into this hopper. Returns true if contents have changed. */
	bool MoveItemsFromChest(cChunk & a_Chunk);

	/** Moves items from a furnace above the hopper into this hopper. Returns true if contents have changed. */
	bool MoveItemsFromFurnace(cChunk & a_Chunk);

	/** Moves items from the specified a_Entity's Contents into this hopper. Returns true if contents have changed. */
	bool MoveItemsFromGrid(cBlockEntityWithItems & a_Entity);

	/** Moves one piece from the specified itemstack into this hopper. Returns true if contents have changed. Doesn't change the itemstack. */
	bool MoveItemsFromSlot(cBlockEntityWithItems & a_Entity, int a_SrcSlotNum);

	/** Moves items to the chest at the specified coords. Returns true if contents have changed */
	bool MoveItemsToChest(cChunk & a_Chunk, int a_BlockX, int a_BlockY, int a_BlockZ);

	/** Moves items to the furnace at the specified coords. Returns true if contents have changed */
	bool MoveItemsToFurnace(cChunk & a_Chunk, int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_HopperMeta);

	/** Moves items to the specified ItemGrid. Returns true if contents have changed */
	bool MoveItemsToGrid(cBlockEntityWithItems & a_Entity);

	/** Moves one piece to the specified entity's contents' slot. Returns true if contents have changed. */
	bool MoveItemsToSlot(cBlockEntityWithItems & a_Entity, int a_DstSlotNum);
} ;  // tolua_export




