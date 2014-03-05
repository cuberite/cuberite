
#include "Globals.h"
#include "BlockDoor.h"
#include "../Item.h"
#include "../Entities/Player.h"





cBlockDoorHandler::cBlockDoorHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockDoorHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	if (OldMeta & 8)
	{
		// Was upper part of door
		if (IsDoor(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ)))
		{
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
		}
	}
	else
	{
		// Was lower part
		if (IsDoor(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ)))
		{
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY + 1, a_BlockZ, E_BLOCK_AIR, 0);
		}
	}
}





void cBlockDoorHandler::OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	if (a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_WOODEN_DOOR)
	{
		ChangeDoor(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
	}
}





void cBlockDoorHandler::OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace)
{
	NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
	a_Player->GetClientHandle()->SendBlockChange(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, Meta);
	
	if (Meta & 8)
	{
		// Current block is top of the door
		a_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY - 1, a_BlockZ, a_Player);
	}
	else
	{
		// Current block is bottom of the door
		a_Player->GetWorld()->SendBlockTo(a_BlockX, a_BlockY + 1, a_BlockZ, a_Player);
	}
}





void cBlockDoorHandler::OnPlacedByPlayer(
	cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
)
{
	NIBBLETYPE a_TopBlockMeta = 8;
	if (
		((a_BlockMeta == 0) && (a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ - 1) == m_BlockType)) ||
		((a_BlockMeta == 1) && (a_ChunkInterface.GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ) == m_BlockType)) ||
		((a_BlockMeta == 2) && (a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ + 1) == m_BlockType)) ||
		((a_BlockMeta == 3) && (a_ChunkInterface.GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ) == m_BlockType))
	)
	{
		a_TopBlockMeta = 9;
	}
	a_ChunkInterface.SetBlock(a_WorldInterface, a_BlockX, a_BlockY + 1, a_BlockZ, m_BlockType, a_TopBlockMeta);
}





const char * cBlockDoorHandler::GetStepSound(void)
{
	return (m_BlockType == E_BLOCK_WOODEN_DOOR) ? "step.wood" : "step.stone";
}




