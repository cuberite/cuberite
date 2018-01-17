
#pragma once

#include "BlockHandler.h"





class cBlockFluidHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;

public:
	cBlockFluidHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{

	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
	}

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) override
	{
		return true;
	}

	virtual void Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk) override
	{
		switch (m_BlockType)
		{
			case E_BLOCK_STATIONARY_LAVA:
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_LAVA, a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
				break;
			}
			case E_BLOCK_STATIONARY_WATER:
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_WATER, a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
				break;
			}
		}
		super::Check(a_ChunkInterface, a_PluginInterface, a_RelX, a_RelY, a_RelZ, a_Chunk);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		if (IsBlockWater(m_BlockType))
		{
			return 12;
		}
		ASSERT(!"Unhandled blocktype in fluid/water handler!");
		return 0;
	}

	virtual bool CanSustainPlant(BLOCKTYPE a_Plant) override
	{
		return (
			(a_Plant == E_BLOCK_BEETROOTS) ||
			(a_Plant == E_BLOCK_CROPS) ||
			(a_Plant == E_BLOCK_CARROTS) ||
			(a_Plant == E_BLOCK_POTATOES) ||
			(a_Plant == E_BLOCK_MELON_STEM) ||
			(a_Plant == E_BLOCK_PUMPKIN_STEM)
		);
	}
} ;





class cBlockLavaHandler :
	public cBlockFluidHandler
{
	typedef cBlockFluidHandler super;
public:

	cBlockLavaHandler(BLOCKTYPE a_BlockType) :
		super(a_BlockType)
	{
	}

	/** Called to tick the block */
	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		if (a_Chunk.GetWorld()->ShouldLavaSpawnFire())
		{
			// Try to start up to 5 fires:
			for (int i = 0; i < 5; i++)
			{
				TryStartFireNear(a_RelX, a_RelY, a_RelZ, a_Chunk);
			}
		}
	}

	/** Tries to start a fire near the lava at given coords. Returns true if fire started. */
	static bool TryStartFireNear(int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk)
	{
		// Pick a block next to this lava block:
		int rnd = a_Chunk.GetWorld()->GetTickRandomNumber(cChunkDef::NumBlocks * 8) / 7;
		int x = (rnd % 3) - 1;         // -1 .. 1
		int y = ((rnd / 4) % 4) - 1;   // -1 .. 2
		int z = ((rnd / 16) % 3) - 1;  // -1 .. 1

		// Check if it's fuel:
		BLOCKTYPE BlockType;
		if (
			((a_RelY + y < 0) || (a_RelY + y >= cChunkDef::Height)) ||
			!a_Chunk.UnboundedRelGetBlockType(a_RelX + x, a_RelY + y, a_RelZ + z, BlockType) ||
			!cFireSimulator::IsFuel(BlockType)
		)
		{
			return false;
		}

		// Try to set it on fire:
		static struct
		{
			int x, y, z;
		} CrossCoords[] =
		{
			{-1,  0,  0},
			{ 1,  0,  0},
			{ 0, -1,  0},
			{ 0,  1,  0},
			{ 0,  0, -1},
			{ 0,  0,  1},
		} ;
		int RelX = a_RelX + x;
		int RelY = a_RelY + y;
		int RelZ = a_RelZ + z;
		for (size_t i = 0; i < ARRAYCOUNT(CrossCoords); i++)
		{
			if (
				((RelY + CrossCoords[i].y >= 0) && (RelY + CrossCoords[i].y < cChunkDef::Height)) &&
				a_Chunk.UnboundedRelGetBlockType(RelX + CrossCoords[i].x, RelY + CrossCoords[i].y, RelZ + CrossCoords[i].z, BlockType) &&
				(BlockType == E_BLOCK_AIR)
			)
			{
				// This is an air block next to a fuel next to lava, light it up:
				a_Chunk.UnboundedRelSetBlock(RelX + CrossCoords[i].x, RelY + CrossCoords[i].y, RelZ + CrossCoords[i].z, E_BLOCK_FIRE, 0);
				return true;
			}
		}  // for i - CrossCoords[]
		return false;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 4;
	}

	virtual bool CanSustainPlant(BLOCKTYPE a_Plant) override
	{
		return false;
	}
} ;




