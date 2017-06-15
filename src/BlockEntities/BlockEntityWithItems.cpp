// BlockEntityWithItems.cpp

// Implements the cBlockEntityWithItems class representing a common ancestor for all block entities that have an ItemGrid





#include "Globals.h"
#include "BlockEntityWithItems.h"





cBlockEntityWithItems::cBlockEntityWithItems(
	BLOCKTYPE a_BlockType,
	NIBBLETYPE a_BlockMeta,
	int a_BlockX, int a_BlockY, int a_BlockZ,
	int a_ItemGridWidth, int a_ItemGridHeight,
	cWorld * a_World
):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World),
	cBlockEntityWindowOwner(this),
	m_Contents(a_ItemGridWidth, a_ItemGridHeight)
{
	m_Contents.AddListener(*this);
}





void cBlockEntityWithItems::Destroy(void)
{
	// Drop the contents as pickups:
	ASSERT(m_World != nullptr);
	cItems Pickups;
	m_Contents.CopyToItems(Pickups);
	m_Contents.Clear();
	m_World->SpawnItemPickups(Pickups, m_PosX + 0.5, m_PosY + 0.5, m_PosZ + 0.5);  // Spawn in centre of block
}





void cBlockEntityWithItems::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = reinterpret_cast<const cBlockEntityWithItems &>(a_Src);
	m_Contents.CopyFrom(src.m_Contents);
}





void cBlockEntityWithItems::OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum)
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
