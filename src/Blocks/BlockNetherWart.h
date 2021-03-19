
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





class cBlockNetherWartHandler final :
	public cBlockPlant<false>
{
	using Super = cBlockPlant<false>;

public:

	using Super::Super;

	const unsigned char RipeAge = 3;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		if (Block::NetherWart::Age(a_Block) == RipeAge)
		{
			// Fully grown, drop the entire produce:
			const auto DropNum = FortuneDiscreteRandom(2, 4, ToolFortuneLevel(a_Tool));
			return cItem(Item::NetherWart, DropNum);
		}

		return cItem(Item::NetherWart);
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, unsigned char a_NumStages = 1) const override
	{
		auto OldAge = Block::NetherWart::Age(a_Chunk.GetBlock(a_RelPos));
		auto NewAge = std::min<unsigned char>(OldAge + a_NumStages, RipeAge);
		if ((OldAge < RipeAge) && (CanGrow(a_Chunk, a_RelPos) == paGrowth))
		{
			a_Chunk.FastSetBlock(a_RelPos, Block::NetherWart::NetherWart(NewAge));
			return NewAge - OldAge;
		}

		// In older versions of cuberite, there was a bug which made wart grow too much. This check fixes previously saved buggy warts.
		if (OldAge > 3)
		{
			a_Chunk.FastSetBlock(a_RelPos, Block::NetherWart::NetherWart(RipeAge));
		}
		return 0;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		// Needs to be placed on top of a Soulsand block:
		return ((a_RelPos.y > 0) && (a_Chunk.GetBlock(a_RelPos.addedY(-1)).Type() == BlockType::SoulSand));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 35;
	}
} ;
