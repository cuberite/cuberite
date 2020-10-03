
#pragma once

#include "BlockHandler.h"





class cBlockDeadBushHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) const override
	{
		return true;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock = a_Chunk.GetBlock(a_RelPos.addedY(-1));
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





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
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
