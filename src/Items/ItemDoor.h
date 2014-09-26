
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemDoorHandler :
	public cItemHandler
{
public:
	cItemDoorHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable(void) override
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
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_DOOR:   a_BlockType = E_BLOCK_WOODEN_DOOR; break;
			case E_ITEM_IRON_DOOR:     a_BlockType = E_BLOCK_IRON_DOOR; break;
			case E_ITEM_SPRUCE_DOOR:   a_BlockType = E_BLOCK_SPRUCE_DOOR; break;
			case E_ITEM_BIRCH_DOOR:    a_BlockType = E_BLOCK_BIRCH_DOOR; break;
			case E_ITEM_JUNGLE_DOOR:   a_BlockType = E_BLOCK_JUNGLE_DOOR; break;
			case E_ITEM_DARK_OAK_DOOR: a_BlockType = E_BLOCK_DARK_OAK_DOOR; break;
			case E_ITEM_ACACIA_DOOR:   a_BlockType = E_BLOCK_ACACIA_DOOR; break;
			default:
			{
				ASSERT(!"Unhandled door type");
				return false;
			}
		}
		
		cChunkInterface ChunkInterface(a_World->GetChunkMap());
		bool Meta = BlockHandler(a_BlockType)->GetPlacementBlockTypeMeta(
			ChunkInterface, a_Player,
			a_BlockX, a_BlockY, a_BlockZ, a_BlockFace,
			a_CursorX, a_CursorY, a_CursorZ,
			a_BlockType, a_BlockMeta
		);
		return Meta;
	}
} ;




