
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
	
	virtual ~cChestEntity();

	// cBlockEntity overrides:
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	
	/** Opens a new chest window for this chest.
	Scans for neighbors to open a double chest window, if appropriate. */
	void OpenNewWindow(void);

	/** Gets the number of players who currently have this chest open */
	int GetNumberOfPlayers(void) const { return m_NumActivePlayers; }

	/** Sets the number of players who currently have this chest open */
	void SetNumberOfPlayers(int a_NumActivePlayers) { m_NumActivePlayers = a_NumActivePlayers; }

private:

	/** Number of players who currently have this chest open */
	int m_NumActivePlayers;

	/** cItemGrid::cListener overrides: */
	virtual void OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum)
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
			m_World->MarkRedstoneDirty(GetChunkX(), GetChunkZ());
		}
	}

} ;  // tolua_export




