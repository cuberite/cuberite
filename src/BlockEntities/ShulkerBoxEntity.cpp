
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ShulkerBoxEntity.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/ShulkerBoxWindow.h"
#include "../ClientHandle.h"





cShulkerBoxEntity::cShulkerBoxEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	super(a_BlockType, a_BlockMeta, a_Pos, ContentsWidth, ContentsHeight, a_World),
	m_NumActivePlayers(0)
{
}





cShulkerBoxEntity::~cShulkerBoxEntity()
{
	DestroyWindow();
}





void cShulkerBoxEntity::CopyFrom(const cBlockEntity & a_Src)
{
	super::CopyFrom(a_Src);
	auto & src = static_cast<const cShulkerBoxEntity &>(a_Src);
	m_Contents.CopyFrom(src.m_Contents);

	// Reset the neighbor, there's no sense in copying it:
	m_NumActivePlayers = 0;
}





void cShulkerBoxEntity::SendTo(cClientHandle & a_Client)
{
	// Send a dummy "number of players with shulker box open" packet to make the shulker box visible:
	a_Client.SendBlockAction(m_Pos.x, m_Pos.y, m_Pos.z, 1, 0, m_BlockType);
}





bool cShulkerBoxEntity::UsedBy(cPlayer * a_Player)
{
	if (IsBlocked())
	{
		// Obstruction, don't open
		return true;
	}

	// If the window is not created, open it anew:
	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenNewWindow();
		Window = GetWindow();
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
	// Instead of marking the chunk as dirty upon shulker box contents change, we mark it dirty now
	// We cannot properly detect contents change, but such a change doesn't happen without a player opening the shulker box first.
	// The few false positives aren't much to worry about
	auto chunkCoords = cChunkDef::BlockToChunk(m_Pos);
	m_World->MarkChunkDirty(chunkCoords.m_ChunkX, chunkCoords.m_ChunkZ);
	return true;
}





void cShulkerBoxEntity::OpenNewWindow(void)
{
	OpenWindow(new cShulkerBoxWindow(this));
}





void cShulkerBoxEntity::DestroyWindow()
{
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
		CloseWindow();
	}
}





bool cShulkerBoxEntity::IsBlocked()
{
	// TODO The current check is inexact: shulker boxes should open as long as there is space,
	// which is not the same as just being solid.
	const int Facing = m_World->GetBlockMeta(m_Pos) & 0x7;
	switch (Facing)
	{
		case 0:  // Down
		{
			return (
				(GetPosY() > 0) &&
				cBlockInfo::IsSolid(GetWorld()->GetBlock(GetPosX(), GetPosY() - 1, GetPosZ()))
			);
		}
		case 1:  // Up
		{
			return (
				(GetPosY() < cChunkDef::Height - 1) &&
				cBlockInfo::IsSolid(GetWorld()->GetBlock(GetPosX(), GetPosY() + 1, GetPosZ()))
			);
		}
		case 2:  // North
		{
			return cBlockInfo::IsSolid(GetWorld()->GetBlock(GetPosX(), GetPosY(), GetPosZ() - 1));
		}
		case 3:  // South
		{
			return cBlockInfo::IsSolid(GetWorld()->GetBlock(GetPosX(), GetPosY(), GetPosZ() + 1));
		}
		case 4:  // West
		{
			return cBlockInfo::IsSolid(GetWorld()->GetBlock(GetPosX() + 1, GetPosY(), GetPosZ()));
		}
		case 5:  // East
		{
			return cBlockInfo::IsSolid(GetWorld()->GetBlock(GetPosX() - 1, GetPosY(), GetPosZ()));
		}
		default:
		{
			return true;
		}
	}
}




