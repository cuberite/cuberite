
#pragma once

#include "BlockHandler.h"





class cBlockSpongeHandler :
	public cBlockHandler
{
public:
	cBlockSpongeHandler(BLOCKTYPE a_BlockType):
		cBlockHandler(a_BlockType)
	{
	}





	virtual void Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk) override
	{
		if (GetSoaked(Vector3i(a_RelX, a_RelY, a_RelZ), a_Chunk))
		{
			return;
		}
		cBlockHandler::Check(a_ChunkInterface, a_PluginInterface, a_RelX, a_RelY, a_RelZ, a_Chunk);
	}





	/** Check blocks above and around to see if they are water.
		If one is, soak this sponge and remove nearby water.
		Returns TRUE if the block was changed. */
	bool GetSoaked(Vector3i a_Rel, cChunk & a_Chunk)
	{
		static const std::array<Vector3i, 6> WaterCheck
		{
			{
				{ 1,  0,  0},
				{-1,  0,  0},
				{ 0,  0,  1},
				{ 0,  0, -1},
				{ 0,  1,  0},
				{ 0, -1,  0},
			}
		};
		struct sSeed
		{
			sSeed(Vector3i pos, int d)
			{
				m_Pos = pos;
				m_Depth = d;
			}
			Vector3i m_Pos;
			int m_Depth;
		};
		NIBBLETYPE TargetMeta = a_Chunk.GetMeta(a_Rel.x, a_Rel.y, a_Rel.z);
		if (TargetMeta != E_META_SPONGE_DRY)
		{
			return false;
		}

		bool ShouldSoak = std::any_of(WaterCheck.cbegin(), WaterCheck.cend(), [a_Rel, & a_Chunk](Vector3i a_Offset)
			{
				return IsWet(a_Rel + a_Offset, a_Chunk);
			}
		);

		if (! ShouldSoak)
		{
			return false;
		}

		std::queue<sSeed> Seeds;
		int count = 0;
		const int maxDepth = 7;
		for(unsigned int i = 0; i<6; i++)
		{
			Seeds.emplace(a_Rel + WaterCheck[i], maxDepth - 1);
		}

		while (!Seeds.empty() && count < 65)
		{
			sSeed seed = Seeds.front();
			Vector3i checkRel = seed.m_Pos;
			if (IsWet(checkRel, a_Chunk))
			{
				count++;
				DryUp(checkRel, a_Chunk);
				if (seed.m_Depth > 0)
				{
					for(unsigned int i = 0; i < 6; i++)
					{
						Seeds.emplace(checkRel + WaterCheck[i], seed.m_Depth - 1);
					}
				}
			}
			Seeds.pop();
		}
		a_Chunk.SetBlock(a_Rel.x, a_Rel.y, a_Rel.z, E_BLOCK_SPONGE, E_META_SPONGE_WET);
		return true;
	}

	void DryUp(Vector3i a_Rel, cChunk & a_Chunk)
	{
		a_Chunk.UnboundedRelSetBlock(a_Rel.x, a_Rel.y, a_Rel.z, E_BLOCK_AIR, 0);
	}

	static bool IsWet(Vector3i a_Rel, cChunk & a_Chunk)
	{
		BLOCKTYPE Type;
		return(a_Chunk.UnboundedRelGetBlockType(a_Rel.x, a_Rel.y, a_Rel.z, Type) && IsBlockWater(Type));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 18;
	}
};
