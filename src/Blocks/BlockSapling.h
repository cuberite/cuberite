
#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"





class cBlockSaplingHandler :
	public cBlockHandler
{
public:
	cBlockSaplingHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Only the first 2 bits contain the display information and the 4th bit is for the growth indicator, but, we use 0x07 for forward compatibility
		a_Pickups.push_back(cItem(E_BLOCK_SAPLING, 1, a_BlockMeta & 0x07));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return (a_RelY > 0) && IsBlockTypeOfDirt(a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ));
	}

	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		NIBBLETYPE Light = std::max(a_Chunk.GetBlockLight(a_RelX, a_RelY, a_RelZ), a_Chunk.GetTimeAlteredLight(a_Chunk.GetSkyLight(a_RelX, a_RelY, a_RelZ)));

		// Only grow if we have the right amount of light
		if (Light > 8)
		{
			auto & random = GetRandomProvider();
			// Only grow if we are in the right growth stage and have the right amount of space around them.
			if (((Meta & 0x08) != 0) && random.RandBool(0.45) && CanGrowAt(a_Chunk, a_RelX, a_RelY, a_RelZ, Meta))
			{
				int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
				int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
				a_Chunk.GetWorld()->GrowTree(BlockX, a_RelY, BlockZ);
			}
			// Only move to the next growth stage if we haven't gone there yet
			else if (((Meta & 0x08) == 0) && random.RandBool(0.45))
			{
				a_Chunk.SetMeta(a_RelX, a_RelY, a_RelZ, Meta | 0x08);
			}
		}
	}

	bool CanGrowAt(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Meta)
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

private:
	bool IsLargeTree(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Meta)
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

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




