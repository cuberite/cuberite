
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

		auto BelowBlock = a_Chunk.GetBlock(a_RelPos.addedY(-1));
		switch (BelowBlock.Type())
		{
			case BlockType::Clay:
				// TODO(12xx12) add hardened clay and stained clay
			case BlockType::Sand:
			{
				return true;
			}
			default: return false;
		}
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
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





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
