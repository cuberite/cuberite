
#pragma once

#include "BlockHandler.h"





class cBlockFlowerHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	cBlockFlowerHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		NIBBLETYPE meta = a_BlockMeta & 0x7;
		return cItem(m_BlockType, 1, meta);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
	{
		return ((a_RelPos.y > 0) && IsBlockTypeOfDirt(a_Chunk.GetBlock(a_RelPos.addedY(-1))));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




