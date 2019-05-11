
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
			sSeed(int x, int y, int z, int d)
			{
				m_Pos.x = x;
				m_Pos.y = y;
				m_Pos.z = z;
				m_Depth = d;
			}
			Vector3i m_Pos;
			int m_Depth;
		};
		BLOCKTYPE TargetBlock;
		NIBBLETYPE TargetMeta;
		a_Chunk.GetBlockTypeMeta(a_Rel.x, a_Rel.y, a_Rel.z, TargetBlock, TargetMeta);
		if (TargetMeta == E_META_SPONGE_DRY)
		{
			bool ShouldSoak = std::any_of(WaterCheck.cbegin(), WaterCheck.cend(), [a_Rel, & a_Chunk](Vector3i a_Offset)
				{
					BLOCKTYPE NeighborType;
					return (
						a_Chunk.UnboundedRelGetBlockType(a_Rel.x + a_Offset.x, a_Rel.y + a_Offset.y, a_Rel.z + a_Offset.z, NeighborType)
						&& IsBlockWater(NeighborType)
					);
				}
			);

			if (ShouldSoak)
			{
				std::queue<sSeed> Seeds;
				int count = 0;
				const int maxDepth = 7;
				Seeds.emplace(a_Rel.x - 1, a_Rel.y, a_Rel.z, maxDepth - 1);
				Seeds.emplace(a_Rel.x + 1, a_Rel.y, a_Rel.z, maxDepth - 1);
				Seeds.emplace(a_Rel.x, a_Rel.y - 1, a_Rel.z, maxDepth - 1);
				Seeds.emplace(a_Rel.x, a_Rel.y + 1, a_Rel.z, maxDepth - 1);
				Seeds.emplace(a_Rel.x, a_Rel.y, a_Rel.z - 1, maxDepth - 1);
				Seeds.emplace(a_Rel.x, a_Rel.y, a_Rel.z + 1, maxDepth - 1);

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
							Seeds.emplace(checkRel.x - 1, checkRel.y, checkRel.z, seed.m_Depth - 1);
							Seeds.emplace(checkRel.x + 1, checkRel.y, checkRel.z, seed.m_Depth - 1);
							Seeds.emplace(checkRel.x, checkRel.y - 1, checkRel.z, seed.m_Depth - 1);
							Seeds.emplace(checkRel.x, checkRel.y + 1, checkRel.z, seed.m_Depth - 1);
							Seeds.emplace(checkRel.x, checkRel.y, checkRel.z - 1, seed.m_Depth - 1);
							Seeds.emplace(checkRel.x, checkRel.y, checkRel.z + 1, seed.m_Depth - 1);
						}
					}
					Seeds.pop();
				}
				a_Chunk.SetBlock(a_Rel.x, a_Rel.y, a_Rel.z, E_BLOCK_SPONGE, E_META_SPONGE_WET);
				return true;
			}
		}
		return false;
	}

	void DryUp(Vector3i a_Rel, cChunk & a_Chunk)
	{
		a_Chunk.SetBlock(a_Rel.x, a_Rel.y, a_Rel.z, E_BLOCK_AIR, 0);
	}

	bool IsWet(Vector3i a_Rel, cChunk & a_Chunk)
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
