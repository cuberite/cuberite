
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

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}

		auto BelowBlock = a_Chunk.GetBlock(a_RelPos.addedY(-1));

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





	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(Item::Redstone);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;




