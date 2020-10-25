
// BlockEntityWithItems.h

// Declares the cBlockEntityWithItems class representing a common ancestor for all block entities that have an ItemGrid





#pragma once

#include "BlockEntity.h"
#include "../ItemGrid.h"
#include "../UI/WindowOwner.h"
#include "../World.h"





// tolua_begin
class cBlockEntityWithItems :
	public cBlockEntity,
	// tolua_end
	public cItemGrid::cListener,
	// tolua_begin
	public cBlockEntityWindowOwner
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	cBlockEntityWithItems(
		BLOCKTYPE a_BlockType,                      // Type of the block that the entity represents
		NIBBLETYPE a_BlockMeta,                     // Meta of the block that the entity represents
		Vector3i a_Pos,                             // Abs position of the block entity
		int a_ItemGridWidth, int a_ItemGridHeight,  // Dimensions of the ItemGrid
		cWorld * a_World                            // Optional world to assign to the entity
	);

	// cBlockEntity overrides:
	virtual cItems ConvertToPickups() const override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;

	// tolua_begin

	const cItem & GetSlot(int a_SlotNum)    const { return m_Contents.GetSlot(a_SlotNum); }
	const cItem & GetSlot(int a_X, int a_Y) const { return m_Contents.GetSlot(a_X, a_Y); }

	void SetSlot(int a_SlotNum,    const cItem & a_Item) { m_Contents.SetSlot(a_SlotNum, a_Item); }
	void SetSlot(int a_X, int a_Y, const cItem & a_Item) { m_Contents.SetSlot(a_X, a_Y, a_Item); }

	/** Returns the ItemGrid used for storing the contents */
	cItemGrid & GetContents(void) { return m_Contents; }

	// tolua_end

	/** Const version of the GetContents() function for C++ type-safety */
	const cItemGrid & GetContents(void) const { return m_Contents; }

protected:

	cItemGrid m_Contents;

	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum) override;
} ;  // tolua_export
