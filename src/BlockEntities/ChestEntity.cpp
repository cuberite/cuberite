
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChestEntity.h"
#include "../Chunk.h"
#include "../BlockInfo.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/ChestWindow.h"
#include "../ClientHandle.h"
#include "../Mobs/Ocelot.h"





cChestEntity::cChestEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, ContentsWidth, ContentsHeight, a_World),
	m_NumActivePlayers(0),
	m_Neighbour(nullptr)
{
}





cChestEntity & cChestEntity::GetPrimaryChest()
{
	if (m_Neighbour == nullptr)
	{
		return *this;
	}

	// The primary chest should be the one with lesser X or Z coord:
	return (
		(m_Neighbour->GetPosX() < GetPosX()) ||
		(m_Neighbour->GetPosZ() < GetPosZ())
	) ? *m_Neighbour : *this;
}





cChestEntity * cChestEntity::GetSecondaryChest()
{
	// If we're the primary, then our neighbour is the secondary, and vice versa:
	return (&GetPrimaryChest() == this) ? m_Neighbour : this;
}





bool cChestEntity::ScanNeighbour(cChunk & a_Chunk, Vector3i a_Position)
{
	const auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(a_Position);

	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		// If a chest was in fact there, they'll find us when their chunk loads.
		return false;
	}

	const auto BlockEntity = Chunk->GetBlockEntityRel(a_Position);

	if ((BlockEntity == nullptr) || (BlockEntity->GetBlockType() != m_Block.Type()))
	{
		// Neighbouring block is not the same type of chest:
		return false;
	}

	m_Neighbour = static_cast<cChestEntity *>(BlockEntity);
	return true;
}





void cChestEntity::DestroyWindow()
{
	const auto Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





bool cChestEntity::IsBlocked()
{
	return (
		(GetPosY() < cChunkDef::Height - 1) &&
		(
			!cBlockInfo::IsTransparent(GetWorld()->GetBlock(GetPos().addedY(1))) ||
			!cOcelot::IsCatSittingOnBlock(GetWorld(), Vector3d(GetPos()))
		)
	);
}





void cChestEntity::OpenNewWindow(void)
{
	if (m_Neighbour != nullptr)
	{
		ASSERT(&GetPrimaryChest() == this);  // Should only open windows for the primary chest.

		OpenWindow(new cChestWindow(this, m_Neighbour));
	}
	else
	{
		// There is no chest neighbour, open a single-chest window:
		OpenWindow(new cChestWindow(this));
	}
}





void cChestEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cChestEntity &>(a_Src);
	m_Contents.CopyFrom(src.m_Contents);

	// Reset the neighbor and player count, there's no sense in copying these:
	m_Neighbour = nullptr;
	m_NumActivePlayers = 0;
}





void cChestEntity::OnAddToWorld(cWorld & a_World, cChunk & a_Chunk)
{
	Super::OnAddToWorld(a_World, a_Chunk);

	// Scan horizontally adjacent blocks for any neighbouring chest of the same type:
	if (
		const auto Position = GetRelPos();

		ScanNeighbour(a_Chunk, Position.addedX(-1)) ||
		ScanNeighbour(a_Chunk, Position.addedX(+1)) ||
		ScanNeighbour(a_Chunk, Position.addedZ(-1)) ||
		ScanNeighbour(a_Chunk, Position.addedZ(+1))
	)
	{
		m_Neighbour->m_Neighbour = this;
		m_Neighbour->DestroyWindow();  // Force neighbour's window shut. Does Mojang server do this or should a double window open?
	}
}





void cChestEntity::OnRemoveFromWorld()
{
	if (m_Neighbour != nullptr)
	{
		// Neighbour may share a window with us, force the window shut:
		m_Neighbour->DestroyWindow();
		m_Neighbour->m_Neighbour = nullptr;
	}

	DestroyWindow();
}





void cChestEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





bool cChestEntity::UsedBy(cPlayer * a_Player)
{
	if (IsBlocked())
	{
		// Obstruction, don't open
		return true;
	}

	if ((m_Neighbour != nullptr) && m_Neighbour->IsBlocked())
	{
		return true;
	}

	if (m_Block.Type() == BlockType::Chest)
	{
		a_Player->GetStatistics().Custom[CustomStatistic::OpenChest]++;
	}
	else  // E_BLOCK_TRAPPED_CHEST
	{
		a_Player->GetStatistics().Custom[CustomStatistic::TriggerTrappedChest]++;
	}

	auto & PrimaryChest = GetPrimaryChest();
	cWindow * Window = PrimaryChest.GetWindow();

	// If the window is not created, open it anew:
	if (Window == nullptr)
	{
		PrimaryChest.OpenNewWindow();
		Window = PrimaryChest.GetWindow();
	}

	// Open the window for the player:
	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(*Window);
		}
	}

	return true;
}





void cChestEntity::OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum)
{
	ASSERT(a_Grid == &m_Contents);

	// Have cBlockEntityWithItems update redstone and try to broadcast our window:
	Super::OnSlotChanged(a_Grid, a_SlotNum);

	cWindow * Window = GetWindow();
	if ((Window == nullptr) && (m_Neighbour != nullptr))
	{
		// Window was null, Super will have failed.
		// Neighbour might own the window:
		Window = m_Neighbour->GetWindow();
	}

	if (Window != nullptr)
	{
		Window->BroadcastWholeWindow();
	}
}
