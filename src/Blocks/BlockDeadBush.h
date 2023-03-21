
#pragma once

#include "BlockHandler.h"





class cBlockDeadBushHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, const Vector3i a_Position, const NIBBLETYPE a_Meta, const eBlockFace a_ClickedBlockFace, const bool a_ClickedDirectly) const override
	{
		return true;
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		if (a_Position.y <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock = a_Chunk.GetBlock(a_Position.addedY(-1));
		switch (BelowBlock)
		{
			case E_BLOCK_CLAY:
			case E_BLOCK_HARDENED_CLAY:
			case E_BLOCK_STAINED_CLAY:
			case E_BLOCK_SAND:
			{
				return true;
			}
			default: return false;
		}
	}





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// If cutting down with shears, drop self:
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			return cItem(m_BlockType, 1, a_BlockMeta);
		}

		// Drop 0-3 sticks:
		auto chance = GetRandomProvider().RandInt<char>(3);
		if (chance > 0)
		{
			return cItem(E_ITEM_STICK, chance, 0);
		}
		return {};
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;
