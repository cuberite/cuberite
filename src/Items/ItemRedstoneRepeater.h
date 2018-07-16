
#pragma once

#include "ItemHandler.h"
#include "../Blocks/BlockRedstoneRepeater.h"





class cItemRedstoneRepeaterHandler :
	public cItemHandler
{
public:
	cItemRedstoneRepeaterHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		cBlockHandler * BlockH = BlockHandler(E_BLOCK_REDSTONE_REPEATER_OFF);
		cChunkInterface ChunkInterface(a_World->GetChunkMap());
		return BlockH->GetPlacementBlockTypeMeta(
			ChunkInterface, *a_Player,
			a_BlockX, a_BlockY, a_BlockZ, a_BlockFace,
			a_CursorX, a_CursorY, a_CursorZ,
			a_BlockType, a_BlockMeta
		);
	}
} ;




