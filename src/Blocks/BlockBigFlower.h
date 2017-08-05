
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"




class cBlockBigFlowerHandler :
	public cBlockHandler
{
public:
	typedef cBlockHandler super;

	cBlockBigFlowerHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) override
	{
		if (IsMetaTopPart(a_Meta))
		{
			BLOCKTYPE BottomType;
			if (
				(a_Pos.y < 1) ||
				!a_ChunkInterface.GetBlockTypeMeta(a_Pos.x, a_Pos.y - 1, a_Pos.z, BottomType, a_Meta) ||
				(BottomType != E_BLOCK_BIG_FLOWER)
			)
			{
				// Can't find the flower meta so assume grass
				return true;
			}
		}

		NIBBLETYPE FlowerMeta = a_Meta & 0x07;
		return (
			(FlowerMeta == E_META_BIG_FLOWER_DOUBLE_TALL_GRASS) ||
			(FlowerMeta == E_META_BIG_FLOWER_LARGE_FERN)
		);
	}

	virtual void DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_CanDrop) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		int AlternateY  = a_BlockY;
		int BottomY     = a_BlockY;

		if (Meta & 0x8)
		{
			--AlternateY;
			--BottomY;
		}
		else
		{
			++AlternateY;
		}
		// also destroy the other block if it has a valid height and is a big flower
		if (cChunkDef::IsValidHeight(AlternateY) && a_ChunkInterface.GetBlock(a_BlockX, AlternateY, a_BlockZ) == E_BLOCK_BIG_FLOWER)
		{
			super::DropBlock(a_ChunkInterface, a_WorldInterface, a_BlockPluginInterface, a_Digger, a_BlockX, BottomY, a_BlockZ, a_CanDrop);
			a_ChunkInterface.FastSetBlock(a_BlockX, AlternateY, a_BlockZ, E_BLOCK_AIR, 0);
		}
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		if (IsMetaTopPart(a_BlockMeta))
		{
			return;  // No way to tell flower type
		}

		NIBBLETYPE Meta = a_BlockMeta & 0x7;
		if (Meta == E_META_BIG_FLOWER_DOUBLE_TALL_GRASS)
		{
			if (GetRandomProvider().RandBool(1.0 / 24.0))
			{
				a_Pickups.push_back(cItem(E_ITEM_SEEDS));
			}
		}
		else if (Meta != E_META_BIG_FLOWER_LARGE_FERN)
		{
			a_Pickups.push_back(cItem(E_BLOCK_BIG_FLOWER, 1, Meta));
		}
	}

	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if (Meta & 0x8)
		{
			Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ);
		}

		NIBBLETYPE FlowerMeta = Meta & 0x7;
		if (!a_Player.IsGameModeCreative())
		{
			if (
				(a_Player.GetEquippedItem().m_ItemType == E_ITEM_SHEARS) &&
				(
					(FlowerMeta == E_META_BIG_FLOWER_DOUBLE_TALL_GRASS) ||
					(FlowerMeta == E_META_BIG_FLOWER_LARGE_FERN)
				)
			)
			{
				if (GetRandomProvider().RandBool(0.10))
				{
					cItems Pickups;
					if (FlowerMeta == E_META_BIG_FLOWER_DOUBLE_TALL_GRASS)
					{
						Pickups.Add(E_BLOCK_TALL_GRASS, 2, 1);
					}
					else if (FlowerMeta == E_META_BIG_FLOWER_LARGE_FERN)
					{
						Pickups.Add(E_BLOCK_TALL_GRASS, 2, 2);
					}
					a_WorldInterface.SpawnItemPickups(Pickups, a_BlockX, a_BlockY, a_BlockZ);
				}
				a_Player.UseEquippedItem();
			}
		}

		if (
			(a_Player.GetEquippedItem().m_ItemType != E_ITEM_SHEARS) &&
			(
				(FlowerMeta == E_META_BIG_FLOWER_DOUBLE_TALL_GRASS) ||
				(FlowerMeta == E_META_BIG_FLOWER_LARGE_FERN)
			)
		)
		{
			a_ChunkInterface.SetBlock(a_BlockX, a_BlockY, a_BlockZ, 0, 0);
		}
	}

	bool IsMetaTopPart(NIBBLETYPE a_Meta)
	{
		return (a_Meta & 0x08) != 0;
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		a_Chunk.GetBlockTypeMeta(a_RelX, a_RelY - 1, a_RelZ, BlockType, BlockMeta);

		return IsBlockTypeOfDirt(BlockType) || ((BlockType == E_BLOCK_BIG_FLOWER) && !IsMetaTopPart(BlockMeta));
	}

	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

		if (OldMeta & 0x8)
		{
			// Was upper part of flower
			if (a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ) == m_BlockType)
			{
				a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			}
		}
		else
		{
			// Was lower part
			if (a_ChunkInterface.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ) == m_BlockType)
			{
				a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY + 1, a_BlockZ, E_BLOCK_AIR, 0);
			}
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




