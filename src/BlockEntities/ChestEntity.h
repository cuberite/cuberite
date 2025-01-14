
#pragma once

#include "BlockEntityWithItems.h"
#include "../Simulator/RedstoneSimulator.h"





class cClientHandle;





// tolua_begin
class cChestEntity :
	public cBlockEntityWithItems
{
	// tolua_end

	using Super = cBlockEntityWithItems;

	// tolua_begin

public:

	enum
	{
		ContentsHeight = 3,
		ContentsWidth  = 9,
	} ;

	// tolua_end

	cChestEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World);

	/** Gets the number of players who currently have this chest open */
	int GetNumberOfPlayers(void) const { return m_NumActivePlayers; }

	/** Returns the associated primary chest. */
	cChestEntity & GetPrimaryChest();

	/** Returns the associated secondary chest, if any. */
	cChestEntity * GetSecondaryChest();

	/** Search the given horizontally adjacent relative position for a neighbouring chest of the same type.
	If found, links them together with ourselves and returns true. */
	bool ScanNeighbour(cChunk & a_Chunk, Vector3i a_Position);

	/** Sets the number of players who currently have this chest open */
	void SetNumberOfPlayers(int a_NumActivePlayers) { m_NumActivePlayers = a_NumActivePlayers; }

private:

	/** Number of players who currently have this chest open */
	int m_NumActivePlayers;

	/** Neighbouring chest that links to form a double chest */
	cChestEntity * m_Neighbour;

	/** Forces any players to close the owned window. */
	void DestroyWindow();

	/** Returns true if the chest should not be accessible by players. */
	bool IsBlocked();

	/** Opens a new chest window where this is the primary chest and any neighbour is the secondary. */
	void OpenNewWindow();

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual void OnAddToWorld(cWorld & a_World, cChunk & a_Chunk) override;
	virtual void OnRemoveFromWorld() override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;

	/** cItemGrid::cListener overrides: */
	virtual void OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum) override;
} ;  // tolua_export
