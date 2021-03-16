#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"





class cBlockCocoaPodHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		// Check that we're attached to a jungle log block:
		eBlockFace BlockFace = Block::Cocoa::Facing(a_Chunk.GetBlock(a_RelPos));
		auto LogPos = AddFaceDirection(a_RelPos, BlockFace, true);
		BlockState LogBlock;
		a_Chunk.UnboundedRelGetBlock(LogPos, LogBlock);
		return (LogBlock.Type() == BlockType::JungleLog);
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		if (GetRandomProvider().RandBool(0.20))
		{
			Grow(a_Chunk, a_RelPos);
		}
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// If fully grown, give 3 items, otherwise just one:
		auto GrowState = Block::Cocoa::Age(a_Block);
		return cItem(E_ITEM_DYE, ((GrowState >= 2) ? 3 : 1), E_META_DYE_BROWN);
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, unsigned char a_NumStages = 1) const override
	{
		auto OldAge = Block::Cocoa::Age(a_Chunk.GetBlock(a_RelPos));

		if (OldAge >= 2)
		{
			return 0;
		}
		auto NewAge = std::min<unsigned char>(OldAge + a_NumStages, 2);
		a_Chunk.FastSetBlock(a_RelPos, Block::Cocoa::Cocoa(NewAge, Block::Cocoa::Facing(a_Chunk.GetBlock(a_RelPos))));
		return NewAge - OldAge;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 34;
	}
} ;




