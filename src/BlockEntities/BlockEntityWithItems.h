
// BlockEntityWithItems.h

// Declares the cBlockEntityWithItems class representing a common ancestor for all block entities that have an ItemGrid





#pragma once

#include "BlockEntity.h"
#include "../ItemGrid.h"
#include "../UI/WindowOwner.h"





// tolua_begin
class cBlockEntityWithItems :
	public cBlockEntity
	// tolua_end
	// tolua doesn't seem to support multiple inheritance?
	, public cItemGrid::cListener
	, public cBlockEntityWindowOwner
	// tolua_begin
{
	typedef cBlockEntity super;
	
public:
	// tolua_end
	
	cBlockEntityWithItems(
		BLOCKTYPE a_BlockType,                      // Type of the block that the entity represents
		int a_BlockX, int a_BlockY, int a_BlockZ,   // Position of the block entity
		int a_ItemGridWidth, int a_ItemGridHeight,  // Dimensions of the ItemGrid
		cWorld * a_World                            // Optional world to assign to the entity
	) :
		super(a_BlockType, a_BlockX, a_BlockY, a_BlockZ, a_World),
		m_Contents(a_ItemGridWidth, a_ItemGridHeight)
	{
		m_Contents.AddListener(*this);
	}
	
	virtual void Destroy(void) override
	{
		// Drop the contents as pickups:
		ASSERT(m_World != NULL);
		cItems Pickups;
		m_Contents.CopyToItems(Pickups);
		m_Contents.Clear();
		m_World->SpawnItemPickups(Pickups, m_PosX + 0.5, m_PosY + 0.5, m_PosZ + 0.5);  // Spawn in centre of block
	}
	
	// tolua_begin
	
	const cItem & GetSlot(int a_SlotNum)    const { return m_Contents.GetSlot(a_SlotNum); }
	const cItem & GetSlot(int a_X, int a_Y) const { return m_Contents.GetSlot(a_X, a_Y); }
	
	void SetSlot(int a_SlotNum,    const cItem & a_Item) { m_Contents.SetSlot(a_SlotNum, a_Item); }
	void SetSlot(int a_X, int a_Y, const cItem & a_Item) { m_Contents.SetSlot(a_X, a_Y, a_Item); }

	/// Returns the ItemGrid used for storing the contents
	cItemGrid & GetContents(void) { return m_Contents; }

	// tolua_end
	
	/// Const version of the GetContents() function for C++ type-safety
	const cItemGrid & GetContents(void) const { return m_Contents; }

protected:
	cItemGrid m_Contents;
	
	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum)
	{
		UNUSED(a_SlotNum);
		ASSERT(a_Grid == &m_Contents);
		if (m_World != NULL)
		{
			if (GetWindow() != NULL)
			{
				GetWindow()->BroadcastWholeWindow();
			}

			m_World->MarkChunkDirty(GetChunkX(), GetChunkZ());
		}
	}
} ;  // tolua_export




