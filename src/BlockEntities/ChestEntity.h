
#pragma once

#include "BlockEntityWithItems.h"





class cClientHandle;





// tolua_begin
class cChestEntity :
	public cBlockEntityWithItems
{
	typedef cBlockEntityWithItems super;

public:
	enum
	{
		ContentsHeight = 3,
		ContentsWidth  = 9,
	} ;

	// tolua_end

	BLOCKENTITY_PROTODEF(cChestEntity)

	/** Constructor used for normal operation */
	cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World, BLOCKTYPE a_Type);

	virtual ~cChestEntity() override;

	// cBlockEntity overrides:
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;

	/** Search horizontally adjacent blocks for neighbouring chests and links them together. */
	void ScanNeighbours();

	/** Opens a new chest window where this is the primary chest and any neighbour is the secondary. */
	void OpenNewWindow();

	/** Forces any players to close the owned window. */
	void DestroyWindow();

	/** Returns true if the chest should not be accessible by players. */
	bool IsBlocked();

	/** Gets the number of players who currently have this chest open */
	int GetNumberOfPlayers(void) const { return m_NumActivePlayers; }

	/** Sets the number of players who currently have this chest open */
	void SetNumberOfPlayers(int a_NumActivePlayers) { m_NumActivePlayers = a_NumActivePlayers; }

private:

	/** Number of players who currently have this chest open */
	int m_NumActivePlayers;

	/** Neighbouring chest that links to form a double chest */
	cChestEntity * m_Neighbour;

	/** cItemGrid::cListener overrides: */
	virtual void OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum) override
	{
		UNUSED(a_SlotNum);
		ASSERT(a_Grid == &m_Contents);
		if (m_World != nullptr)
		{
			if (GetWindow() != nullptr)
			{
				GetWindow()->BroadcastWholeWindow();
			}

			m_World->MarkChunkDirty(GetChunkX(), GetChunkZ());
		}
	}

} ;  // tolua_export




