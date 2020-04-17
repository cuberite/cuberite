
#pragma once

#include "BlockHandler.h"





class cBlockFluidHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	cBlockFluidHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{

	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// No pickups
		return {};
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) override
	{
		return true;
	}





	virtual void Check(
		cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface,
		Vector3i a_RelPos,
		cChunk & a_Chunk
	) override
	{
		switch (m_BlockType)
		{
			case E_BLOCK_STATIONARY_LAVA:
			{
				a_Chunk.FastSetBlock(a_RelPos, E_BLOCK_LAVA, a_Chunk.GetMeta(a_RelPos));
				break;
			}
			case E_BLOCK_STATIONARY_WATER:
			{
				a_Chunk.FastSetBlock(a_RelPos, E_BLOCK_WATER, a_Chunk.GetMeta(a_RelPos));
				break;
			}
		}
		Super::Check(a_ChunkInterface, a_PluginInterface, a_RelPos, a_Chunk);
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





class cBlockLavaHandler:
	public cBlockFluidHandler
{
	using Super = cBlockFluidHandler;

public:

	cBlockLavaHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) override
	{
		if (a_Chunk.GetWorld()->ShouldLavaSpawnFire())
		{
			// Try to start up to 5 fires:
			for (int i = 0; i < 5; i++)
			{
				TryStartFireNear(a_RelPos, a_Chunk);
			}
		}
	}





	/** Tries to start a fire near the lava at given coords. Returns true if fire started. */
	static bool TryStartFireNear(const Vector3i a_RelPos, cChunk & a_Chunk)
	{
		// Pick a random block next to this lava block:
		int rnd = a_Chunk.GetWorld()->GetTickRandomNumber(cChunkDef::NumBlocks * 8) / 7;
		int x = (rnd % 3) - 1;         // -1 .. 1
		int y = ((rnd / 4) % 4) - 1;   // -1 .. 2
		int z = ((rnd / 16) % 3) - 1;  // -1 .. 1
		auto Pos = a_RelPos + Vector3i(x, y, z);

		// Check if it's fuel:
		BLOCKTYPE BlockType;
		if (
			!cChunkDef::IsValidHeight(Pos.y) ||
			!a_Chunk.UnboundedRelGetBlockType(Pos, BlockType) ||
			!cFireSimulator::IsFuel(BlockType)
		)
		{
			return false;
		}

		// Try to set it on fire:
		static Vector3i CrossCoords[] =
		{
			{-1,  0,  0},
			{ 1,  0,  0},
			{ 0, -1,  0},
			{ 0,  1,  0},
			{ 0,  0, -1},
			{ 0,  0,  1},
		} ;
		for (size_t i = 0; i < ARRAYCOUNT(CrossCoords); i++)
		{
			auto NeighborPos = Pos + CrossCoords[i];
			if (
				cChunkDef::IsValidHeight(NeighborPos.y) &&
				a_Chunk.UnboundedRelGetBlockType(NeighborPos, BlockType) &&
				(BlockType == E_BLOCK_AIR)
			)
			{
				// This is an air block next to a fuel next to lava, light the fuel block up:
				a_Chunk.UnboundedRelSetBlock(NeighborPos, E_BLOCK_FIRE, 0);
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




