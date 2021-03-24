
#pragma once

#include "BlockHandler.h"
#include "../Simulator/FireSimulator.h"





class cBlockFluidHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockLiquid(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Lava:
			case BlockType::Water:
				return true;
			default: return false;
		}
	}

	static inline unsigned char GetFalloff (BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Lava:  return Block::Lava::Level(a_Block);
			case BlockType::Water: return Block::Water::Level(a_Block);
			default: return 0;
		}
	}

	static inline BlockState SetFalloff (BlockState a_Block, unsigned char a_Falloff)
	{
		switch (a_Block.Type())
		{
			case BlockType::Lava:  return Block::Lava::Lava(a_Falloff);
			case BlockType::Water: return Block::Water::Water(a_Falloff);
			default: return a_Block;
		}
	}


private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// No pickups
		return {};
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, BlockState a_Block) const override
	{
		return true;
	}
} ;





class cBlockLavaHandler final :
	public cBlockFluidHandler
{
	using Super = cBlockFluidHandler;

public:

	using Super::Super;

private:

	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
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
		BlockState Self = 0;
		if (
			!cChunkDef::IsValidHeight(Pos.y) ||
			!a_Chunk.UnboundedRelGetBlock(Pos, Self) ||
			!cFireSimulator::IsFuel(Self)
		)
		{
			return false;
		}

		// Try to set it on fire:
		static std::array<Vector3i, 6> CrossCoords =
		{
			Vector3i(-1,  0,  0),
			Vector3i( 1,  0,  0),
			Vector3i( 0, -1,  0),
			Vector3i( 0,  1,  0),
			Vector3i( 0,  0, -1),
			Vector3i( 0,  0,  1),
		};

		for (auto CrossCord : CrossCoords)
		{
			auto NeighborPos = Pos + CrossCord;
			if (
				cChunkDef::IsValidHeight(NeighborPos.y) &&
				a_Chunk.UnboundedRelGetBlock(NeighborPos, Self) &&
				(Self.Type() == BlockType::Air)
			)
			{
				// This is an air block next to a fuel next to lava, light the fuel block up:
				a_Chunk.UnboundedRelSetBlock(NeighborPos, Block::Fire::Fire());
				return true;
			}
		}  // for i - CrossCoords[]
		return false;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 4;
	}
} ;





class cBlockWaterHandler final :
	public cBlockFluidHandler
{
public:

	using cBlockFluidHandler::cBlockFluidHandler;

private:

	virtual ColourID GetMapBaseColourID() const override
	{
		return 12;

	}

	virtual bool CanSustainPlant(BlockState a_Plant) const override
	{
		switch (a_Plant.Type())
		{
			case BlockType::Beetroots:
			case BlockType::Wheat:
			case BlockType::Carrots:
			case BlockType::Potatoes:
			case BlockType::MelonStem:
			case BlockType::AttachedMelonStem:
			case BlockType::PumpkinStem:
			case BlockType::AttachedPumpkinStem:
				return true;
			default: return false;
		}
	}
};
