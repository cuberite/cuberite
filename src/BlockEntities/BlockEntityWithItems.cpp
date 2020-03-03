// BlockEntityWithItems.cpp

#include "Globals.h"
#include "BlockEntityWithItems.h"
#include "../Simulator/RedstoneSimulator.h"





cBlockEntityWithItems::cBlockEntityWithItems(
	BLOCKTYPE a_BlockType,
	NIBBLETYPE a_BlockMeta,
	Vector3i a_Pos,
	int a_ItemGridWidth, int a_ItemGridHeight,
	cWorld * a_World
):
	super(a_BlockType, a_BlockMeta, a_Pos, a_World),
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
	m_World->SpawnItemPickups(Pickups, m_Pos.x + 0.5, m_Pos.y + 0.5, m_Pos.z + 0.5);  // Spawn in centre of block
}





void cBlockEntityWithItems::CopyFrom(const cBlockEntity & a_Src)
{
	super::CopyFrom(a_Src);
	auto & src = static_cast<const cBlockEntityWithItems &>(a_Src);
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
		m_World->DoWithChunkAt(m_Pos, [&](cChunk & a_Chunk)
			{
				m_World->GetRedstoneSimulator()->WakeUp(m_Pos, &a_Chunk);
				return true;
			}
		);
	}
}
