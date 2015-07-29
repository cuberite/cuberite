
// ChestWindow.cpp

// Representing the UI window for the chest block

#include "Globals.h"
#include "ChestWindow.h"
#include "../BlockEntities/ChestEntity.h"
#include "../Entities/Player.h"





cChestWindow::cChestWindow(cChestEntity * a_Chest) :
	cWindow(wtChest, (a_Chest->GetBlockType() == E_BLOCK_CHEST) ? "Chest" : "Trapped Chest"),
	m_World(a_Chest->GetWorld()),
	m_BlockX(a_Chest->GetPosX()),
	m_BlockY(a_Chest->GetPosY()),
	m_BlockZ(a_Chest->GetPosZ()),
	m_PrimaryChest(a_Chest),
	m_SecondaryChest(nullptr)
{
	m_SlotAreas.push_back(new cSlotAreaChest(a_Chest, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));

	// Play the opening sound:
	m_World->BroadcastSoundEffect("random.chestopen", static_cast<double>(m_BlockX), static_cast<double>(m_BlockY), static_cast<double>(m_BlockZ), 1, 1);

	// Send out the chest-open packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, a_Chest->GetBlockType());
}





cChestWindow::cChestWindow(cChestEntity * a_PrimaryChest, cChestEntity * a_SecondaryChest) :
	cWindow(wtChest, (a_PrimaryChest->GetBlockType() == E_BLOCK_CHEST) ? "Double Chest" : "Double Trapped Chest"),
	m_World(a_PrimaryChest->GetWorld()),
	m_BlockX(a_PrimaryChest->GetPosX()),
	m_BlockY(a_PrimaryChest->GetPosY()),
	m_BlockZ(a_PrimaryChest->GetPosZ()),
	m_PrimaryChest(a_PrimaryChest),
	m_SecondaryChest(a_SecondaryChest)
{
	m_SlotAreas.push_back(new cSlotAreaDoubleChest(a_PrimaryChest, a_SecondaryChest, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));

	// Play the opening sound:
	m_World->BroadcastSoundEffect("random.chestopen", static_cast<double>(m_BlockX), static_cast<double>(m_BlockY), static_cast<double>(m_BlockZ), 1, 1);

	// Send out the chest-open packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, a_PrimaryChest->GetBlockType());
}





cChestWindow::~cChestWindow()
{
	// Send out the chest-close packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 0, m_PrimaryChest->GetBlockType());

	m_World->BroadcastSoundEffect("random.chestclosed", static_cast<double>(m_BlockX), static_cast<double>(m_BlockY), static_cast<double>(m_BlockZ), 1, 1);
}





bool cChestWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	int ChunkX, ChunkZ;

	m_PrimaryChest->SetNumberOfPlayers(m_PrimaryChest->GetNumberOfPlayers() - 1);
	cChunkDef::BlockToChunk(m_PrimaryChest->GetPosX(), m_PrimaryChest->GetPosZ(), ChunkX, ChunkZ);
	m_PrimaryChest->GetWorld()->MarkRedstoneDirty(ChunkX, ChunkZ);

	if (m_SecondaryChest != nullptr)
	{
		m_SecondaryChest->SetNumberOfPlayers(m_SecondaryChest->GetNumberOfPlayers() - 1);
		cChunkDef::BlockToChunk(m_SecondaryChest->GetPosX(), m_SecondaryChest->GetPosZ(), ChunkX, ChunkZ);
		m_SecondaryChest->GetWorld()->MarkRedstoneDirty(ChunkX, ChunkZ);
	}

	cWindow::ClosedByPlayer(a_Player, a_CanRefuse);
	return true;
}





void cChestWindow::OpenedByPlayer(cPlayer & a_Player)
{
	int ChunkX, ChunkZ;

	m_PrimaryChest->SetNumberOfPlayers(m_PrimaryChest->GetNumberOfPlayers() + 1);
	cChunkDef::BlockToChunk(m_PrimaryChest->GetPosX(), m_PrimaryChest->GetPosZ(), ChunkX, ChunkZ);
	m_PrimaryChest->GetWorld()->MarkRedstoneDirty(ChunkX, ChunkZ);

	if (m_SecondaryChest != nullptr)
	{
		m_SecondaryChest->SetNumberOfPlayers(m_SecondaryChest->GetNumberOfPlayers() + 1);
		cChunkDef::BlockToChunk(m_SecondaryChest->GetPosX(), m_SecondaryChest->GetPosZ(), ChunkX, ChunkZ);
		m_SecondaryChest->GetWorld()->MarkRedstoneDirty(ChunkX, ChunkZ);
	}

	cWindow::OpenedByPlayer(a_Player);
}





void cChestWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Chest Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		// Hotbar or Inventory
		AreasInOrder.push_back(m_SlotAreas[0]);  /* Chest */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}




