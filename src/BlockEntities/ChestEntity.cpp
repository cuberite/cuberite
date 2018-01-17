
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChestEntity.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/ChestWindow.h"
#include "../ClientHandle.h"
#include "../Mobs/Ocelot.h"





cChestEntity::cChestEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World),
	m_NumActivePlayers(0),
	m_Neighbour(nullptr)
{
	int ChunkX = 0, ChunkZ = 0;
	cChunkDef::BlockToChunk(m_PosX, m_PosZ, ChunkX, ChunkZ);
	if (
		(m_World != nullptr) &&
		m_World->IsChunkValid(ChunkX, ChunkZ)
	)
	{
		ScanNeighbours();
	}
}




cChestEntity::~cChestEntity()
{
	if (m_Neighbour != nullptr)
	{
		// Neighbour may share a window with us, force the window shut
		m_Neighbour->DestroyWindow();
		m_Neighbour->m_Neighbour = nullptr;
	}

	DestroyWindow();
}





void cChestEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = reinterpret_cast<const cChestEntity &>(a_Src);
	m_Contents.CopyFrom(src.m_Contents);

	// Reset the neighbor and player count, there's no sense in copying these:
	m_Neighbour = nullptr;
	m_NumActivePlayers = 0;
}





void cChestEntity::SendTo(cClientHandle & a_Client)
{
	// Send a dummy "number of players with chest open" packet to make the chest visible:
	a_Client.SendBlockAction(m_PosX, m_PosY, m_PosZ, 1, 0, m_BlockType);
}





bool cChestEntity::UsedBy(cPlayer * a_Player)
{
	if (IsBlocked())
	{
		// Obstruction, don't open
		return true;
	}

	if (m_Neighbour == nullptr)
	{
		ScanNeighbours();
	}

	// The primary chest should be the one with lesser X or Z coord:
	cChestEntity * PrimaryChest = this;
	if (m_Neighbour != nullptr)
	{
		if (m_Neighbour->IsBlocked())
		{
			// Obstruction, don't open
			return true;
		}

		if (
			(m_Neighbour->GetPosX() > GetPosX()) ||
			(m_Neighbour->GetPosZ() > GetPosZ())
		)
		{
			PrimaryChest = m_Neighbour;
		}
	}

	// If the window is not created, open it anew:
	cWindow * Window = PrimaryChest->GetWindow();
	if (Window == nullptr)
	{
		PrimaryChest->OpenNewWindow();
		Window = PrimaryChest->GetWindow();
	}

	// Open the window for the player:
	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(*Window);
		}
	}

	// This is rather a hack
	// Instead of marking the chunk as dirty upon chest contents change, we mark it dirty now
	// We cannot properly detect contents change, but such a change doesn't happen without a player opening the chest first.
	// The few false positives aren't much to worry about
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(m_PosX, m_PosZ, ChunkX, ChunkZ);
	m_World->MarkChunkDirty(ChunkX, ChunkZ);
	return true;
}





void cChestEntity::ScanNeighbours()
{
	// Callback for finding neighbouring chest:
	auto FindNeighbour = [this](cChestEntity & a_Chest)
	{
		if (a_Chest.GetBlockType() != m_BlockType)
		{
			// Neighboring block is not the same type of chest
			return true;
		}
		m_Neighbour = &a_Chest;
		return false;
	};

	// Scan horizontally adjacent blocks for any neighbouring chest of the same type:
	if (
		m_World->DoWithChestAt(m_PosX - 1, m_PosY, m_PosZ,     FindNeighbour) ||
		m_World->DoWithChestAt(m_PosX + 1, m_PosY, m_PosZ,     FindNeighbour) ||
		m_World->DoWithChestAt(m_PosX,     m_PosY, m_PosZ - 1, FindNeighbour) ||
		m_World->DoWithChestAt(m_PosX,     m_PosY, m_PosZ + 1, FindNeighbour)
	)
	{
		m_Neighbour->m_Neighbour = this;
		// Force neighbour's window shut. Does Mojang server do this or should a double window open?
		m_Neighbour->DestroyWindow();
	}
}





void cChestEntity::OpenNewWindow(void)
{
	if (m_Neighbour != nullptr)
	{
		ASSERT(  // This should be the primary chest
			(m_Neighbour->GetPosX() < GetPosX()) ||
			(m_Neighbour->GetPosZ() < GetPosZ())
		);
		OpenWindow(new cChestWindow(this, m_Neighbour));
	}
	else
	{
		// There is no chest neighbour, open a single-chest window:
		OpenWindow(new cChestWindow(this));
	}
}





void cChestEntity::DestroyWindow()
{
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
		CloseWindow();
	}
}





bool cChestEntity::IsBlocked()
{
	return (
		(GetPosY() < cChunkDef::Height - 1) &&
		(
			!cBlockInfo::IsTransparent(GetWorld()->GetBlock(GetPosX(), GetPosY() + 1, GetPosZ())) ||
			!cOcelot::IsCatSittingOnBlock(GetWorld(), Vector3d(GetPos()))
		)
	);
}




