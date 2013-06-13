
// HopperEntity.h

// Declares the cHopperEntity representing a hopper block entity





#pragma once

#include "BlockEntityWithItems.h"
#include "../UI/WindowOwner.h"





class cHopperEntity :  // tolua_export
	public cBlockEntityWindowOwner,
	// tolua_begin
	public cBlockEntityWithItems
{
	typedef cBlockEntityWithItems super;

public:
	enum {
		ContentsHeight = 1,
		ContentsWidth  = 5,
		TICKS_PER_TRANSFER = 8,  ///< How many ticks at minimum between two item transfers to or from the hopper
	} ;

	/// Constructor used while generating a chunk; sets m_World to NULL
	cHopperEntity(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	// tolua_end
	
	/// Constructor used for normal operation
	cHopperEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	
	static const char * GetClassStatic(void) { return "cHopperEntity"; }
	
protected:

	Int64 m_LastMoveItemsInTick;
	Int64 m_LastMoveItemsOutTick;

	// cBlockEntity overrides:
	virtual bool Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void SaveToJson(Json::Value & a_Value) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	
	/// Opens a new chest window for this chest. Scans for neighbors to open a double chest window, if appropriate.
	void OpenNewWindow(void);

	/// Moves items from the container above it into this hopper. Returns true if the contents have changed.
	bool MoveItemsIn(cChunk & a_Chunk, Int64 a_CurrentTick);
	
	/// Moves pickups from above this hopper into it. Returns true if the contents have changed.
	bool MovePickupsIn(cChunk & a_Chunk, Int64 a_CurrentTick);
	
	/// Moves items out from this hopper into the destination. Returns true if the contents have changed.
	bool MoveItemsOut(cChunk & a_Chunk, Int64 a_CurrentTick);
	
	/// Moves items from a chest (dblchest) above the hopper into this hopper. Returns true if contents have changed.
	bool MoveItemsFromChest(cChunk & a_Chunk);
	
	/// Moves items from a furnace above the hopper into this hopper. Returns true if contents have changed.
	bool MoveItemsFromFurnace(cChunk & a_Chunk);
	
	/// Moves items from the specified ItemGrid into this hopper. Returns true if contents have changed.
	bool MoveItemsFromGrid(cItemGrid & a_Grid);
	
	/// Moves one of the specified itemstack into this hopper. Returns true if contents have changed. Doesn't change the itemstack.
	bool MoveItemsFromSlot(const cItem & a_ItemStack, bool a_AllowNewStacks);
} ;




