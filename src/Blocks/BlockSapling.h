
#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"





class cBlockSaplingHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// The low 3 bits store the sapling type; bit 0x08 is the growth timer (not used in pickups)
		return cItem(m_BlockType, 1, a_BlockMeta & 0x07);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		return (a_RelPos.y > 0) && IsBlockTypeOfDirt(a_Chunk.GetBlock(a_RelPos.addedY(-1)));
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		auto Meta = a_Chunk.GetMeta(a_RelPos);
		auto Light = std::max(a_Chunk.GetBlockLight(a_RelPos), a_Chunk.GetTimeAlteredLight(a_Chunk.GetSkyLight(a_RelPos)));

		// Only grow if we have the right amount of light
		if (Light > 8)
		{
			auto & random = GetRandomProvider();
			// Only grow if we are in the right growth stage and have the right amount of space around them.
			if (((Meta & 0x08) != 0) && random.RandBool(0.45) && CanGrowAt(a_Chunk, a_RelPos.x, a_RelPos.y, a_RelPos.z, Meta))
			{
				auto WorldPos = a_Chunk.RelativeToAbsolute(a_RelPos);
				a_Chunk.GetWorld()->GrowTree(WorldPos);
			}
			// Only move to the next growth stage if we haven't gone there yet
			else if (((Meta & 0x08) == 0) && random.RandBool(0.45))
			{
				a_Chunk.SetMeta(a_RelPos, Meta | 0x08);
			}
		}
	}





	static bool CanGrowAt(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Meta)
	{
		a_Meta = a_Meta & 0x07;
		int CheckHeight = 0;
		bool LargeTree = false;

		// Get the height to check against
		switch (a_Meta)
		{
			case E_META_SAPLING_APPLE:
			{
				CheckHeight = 5;
				break;
			}
			case E_META_SAPLING_CONIFER:
			{
				CheckHeight = 7;
				if (IsLargeTree(a_Chunk, a_RelX, a_RelY, a_RelZ, a_Meta))
				{
					CheckHeight = 16;
					LargeTree = true;
				}
				break;
			}
			case E_META_SAPLING_BIRCH:
			{
				CheckHeight = 6;
				break;
			}
			case E_META_SAPLING_JUNGLE:
			{
				CheckHeight = 7;
				if (IsLargeTree(a_Chunk, a_RelX, a_RelY, a_RelZ, a_Meta))
				{
					CheckHeight = 13;
					LargeTree = true;
				}
				break;
			}
			// Acacias don't need horizontal clearance
			case E_META_SAPLING_ACACIA:
			{
				if (!IsLargeTree(a_Chunk, a_RelX, a_RelY, a_RelZ, a_Meta))
				{
					return false;
				}
				CheckHeight = 7;
				LargeTree = true;
				break;
			}
			// Dark Oaks don't need horizontal clearance
			case E_META_SAPLING_DARK_OAK:
			{
				if (!IsLargeTree(a_Chunk, a_RelX, a_RelY, a_RelZ, a_Meta))
				{
					return false;
				}
				CheckHeight = 7;
				LargeTree = true;
				break;
			}
		}
		// We should always get a valid CheckHeight
		ASSERT(CheckHeight != 0);

		// Don't grow a tree if we don't have enough space left above it in the chunk
		if ((a_RelY + CheckHeight) > cChunkDef::Height)
		{
			return false;
		}
		bool CanGrow = true;

		// Validate the neighbor blocks. They cannot be solid.
		BLOCKTYPE check = E_BLOCK_AIR;
		a_Chunk.UnboundedRelGetBlockType(a_RelX - 1, a_RelY, a_RelZ, check);
		CanGrow = CanGrow && cBlockInfo::IsTransparent(check);

		a_Chunk.UnboundedRelGetBlockType(a_RelX + 1, a_RelY, a_RelZ, check);
		CanGrow = CanGrow && cBlockInfo::IsTransparent(check);

		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ - 1, check);
		CanGrow = CanGrow && cBlockInfo::IsTransparent(check);

		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ + 1, check);
		CanGrow = CanGrow && cBlockInfo::IsTransparent(check);

		while (CheckHeight && CanGrow)
		{
			check = a_Chunk.GetBlock(a_RelX, a_RelY + CheckHeight, a_RelZ);
			CanGrow = CanGrow && ((check == E_BLOCK_AIR) || (check == E_BLOCK_LEAVES));

			// We have to check above the neighboring saplings as well
			if (LargeTree)
			{
				a_Chunk.UnboundedRelGetBlockType(a_RelX + 1, a_RelY + CheckHeight, a_RelZ, check);
				CanGrow = CanGrow && ((check == E_BLOCK_AIR) || (check == E_BLOCK_LEAVES));

				a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY + CheckHeight, a_RelZ + 1, check);
				CanGrow = CanGrow && ((check == E_BLOCK_AIR) || (check == E_BLOCK_LEAVES));

				a_Chunk.UnboundedRelGetBlockType(a_RelX + 1, a_RelY + CheckHeight, a_RelZ + 1, check);
				CanGrow = CanGrow && ((check == E_BLOCK_AIR) || (check == E_BLOCK_LEAVES));
			}

			--CheckHeight;
		}

		return CanGrow;
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
	{
		auto blockMeta = a_Chunk.GetMeta(a_RelPos);
		auto typeMeta = blockMeta & 0x07;
		auto growState = blockMeta >> 3;
		int res = 0;

		// Try to increase the sapling's growState:
		if (growState < 1)
		{
			++growState;
			a_Chunk.FastSetBlock(a_RelPos, m_BlockType, static_cast<NIBBLETYPE>(growState << 3 | typeMeta));
			if (a_NumStages == 1)
			{
				// Only asked to grow one stage, which we did. Bail out.
				return 1;
			}
			res = 1;
		}

		// The sapling is grown, now it becomes a tree:
		a_Chunk.GetWorld()->GrowTreeFromSapling(a_Chunk.RelativeToAbsolute(a_RelPos));
		return res + 1;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}

	static bool IsLargeTree(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Meta)
	{
		BLOCKTYPE type;
		NIBBLETYPE meta;
		bool LargeTree = true;
		a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY, a_RelZ, type, meta);
		LargeTree = LargeTree && (type == E_BLOCK_SAPLING) && ((a_Meta & meta) == a_Meta);

		a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY, a_RelZ + 1, type, meta);
		LargeTree = LargeTree && (type == E_BLOCK_SAPLING) && ((a_Meta & meta) == a_Meta);

		a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ + 1, type, meta);
		LargeTree = LargeTree && (type == E_BLOCK_SAPLING) && ((a_Meta & meta) == a_Meta);

		return LargeTree;
	}
} ;




