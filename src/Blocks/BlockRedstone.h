
#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"





class cBlockRedstoneHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		if (a_Position.y <= 0)
		{
			return false;
		}

		auto BelowBlock = a_Chunk.GetBlock(a_Position.addedY(-1));

		if (cBlockInfo::FullyOccupiesVoxel(BelowBlock))
		{
			return true;
		}
		else if (cBlockSlabHandler::IsAnySlabType(BelowBlock) && cBlockSlabHandler::IsSlabTop(BelowBlock))
		{
			return true;
		}
		return false;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::Redstone);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;




