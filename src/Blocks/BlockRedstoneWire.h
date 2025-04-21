
#pragma once

#include "BlockHandler.h"
#include "Mixins/SolidSurfaceUnderneath.h"




class cBlockRedstoneWireHandler final :
	public cSolidSurfaceUnderneath<cBlockHandler>
{
	using Super = cSolidSurfaceUnderneath<cBlockHandler>;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		if (!cChunkDef::IsValidHeight(a_Position))
		{
			return false;
		}

		auto BelowBlock = a_Chunk.GetBlock(a_Position.addedY(-1));

		return (
			(cBlockInfo::FullyOccupiesVoxel(BelowBlock)) ||
			(cBlockSlabHandler::IsAnySlabType(BelowBlock) && cBlockSlabHandler::IsSlabTop(BelowBlock)) ||
			(cBlockStairsHandler::IsAnyStairType(BelowBlock) && cBlockStairsHandler::IsStairsUpsideDown(BelowBlock))
		);
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




