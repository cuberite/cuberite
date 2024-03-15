
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





class cBlockNetherWartHandler final :
	public cBlockPlant<false>
{
	using Super = cBlockPlant<false>;

public:

	using Super::Super;

	const unsigned char RipeAge = 0x03;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		if (a_BlockMeta == RipeAge)
		{
			// Fully grown, drop the entire produce:
			const auto DropNum = FortuneDiscreteRandom(2, 4, ToolFortuneLevel(a_Tool));
			return cItem(E_ITEM_NETHER_WART, DropNum);
		}

		return cItem(E_ITEM_NETHER_WART);
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
	{
		auto oldMeta = a_Chunk.GetMeta(a_RelPos);
		auto meta = std::min<unsigned char>(oldMeta + static_cast<unsigned char>(a_NumStages), RipeAge);
		if ((oldMeta < RipeAge) && (CanGrow(a_Chunk, a_RelPos) == paGrowth))
		{
			a_Chunk.SetBlock(a_RelPos, m_BlockType, static_cast<NIBBLETYPE>(meta));
			return meta - oldMeta;
		}

		// In older versions of cuberite, there was a bug which made wart grow too much. This check fixes previously saved buggy warts.
		if (oldMeta > RipeAge)
		{
			a_Chunk.FastSetBlock(a_RelPos, m_BlockType, RipeAge);
		}
		return 0;
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		// Needs to be placed on top of a Soulsand block:
		return (a_Position.y > 0) && (a_Chunk.GetBlock(a_Position.addedY(-1)) == E_BLOCK_SOULSAND);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 35;
	}
} ;
