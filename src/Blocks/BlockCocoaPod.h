#pragma once

#include "BlockHandler.h"
#include "Chunk.h"
#include "../FastRandom.h"





class cBlockCocoaPodHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static constexpr unsigned char MaxAge = 2;

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_Self) const override
	{
		// Check that we're attached to a jungle log block:
		eBlockFace BlockFace = Block::Cocoa::Facing(a_Chunk.GetBlock(a_Position));
		auto LogPos = AddFaceDirection(a_Position, BlockFace, true);
		BlockState LogBlock;
		if (!a_Chunk.UnboundedRelGetBlock(LogPos, LogBlock))
		{
			return true;
		}
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





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// If fully grown, give 3 items, otherwise just one:
		auto GrowState = Block::Cocoa::Age(a_Block);
		return cItem(Item::CocoaBeans, ((GrowState >= 2) ? 3 : 1));
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, char a_NumStages = 1) const override
	{
		auto OldAge = Block::Cocoa::Age(a_Chunk.GetBlock(a_RelPos));

		if (OldAge >= MaxAge)
		{
			return 0;
		}
		auto NewAge = std::min<unsigned char>(Clamp(static_cast<unsigned char>(static_cast<char>(OldAge) + a_NumStages), static_cast<unsigned char>(0), MaxAge), MaxAge);
		a_Chunk.FastSetBlock(a_RelPos, Block::Cocoa::Cocoa(NewAge, Block::Cocoa::Facing(a_Chunk.GetBlock(a_RelPos))));
		return NewAge - OldAge;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 34;
	}
} ;




