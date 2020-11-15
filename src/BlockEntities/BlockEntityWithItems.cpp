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
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World),
	cBlockEntityWindowOwner(this),
	m_Contents(a_ItemGridWidth, a_ItemGridHeight)
{
	m_Contents.AddListener(*this);
}





cItems cBlockEntityWithItems::ConvertToPickups() const
{
	cItems Pickups;
	Pickups.AddItemGrid(m_Contents);
	return Pickups;
}





void cBlockEntityWithItems::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cBlockEntityWithItems &>(a_Src);
	m_Contents.CopyFrom(src.m_Contents);
}





void cBlockEntityWithItems::OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum)
{
	UNUSED(a_SlotNum);
	ASSERT(a_Grid == &m_Contents);

	if (m_World == nullptr)
	{
		return;
	}

	if (GetWindow() != nullptr)
	{
		GetWindow()->BroadcastWholeWindow();
	}

	m_World->MarkChunkDirty(GetChunkX(), GetChunkZ());

	// Notify comparators:
	m_World->WakeUpSimulators(m_Pos);
}
