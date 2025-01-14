
#pragma once

#include "BlockHandler.h"





class cBlockPortalHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// No pickups
		return {};
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_Position
	) const override
	{
		// Spawn zombie pigmen with a 0.05% chance:
		if (GetRandomProvider().RandBool(0.9995))
		{
			return;
		}
		auto WorldPos = a_Chunk.RelativeToAbsolute(a_Position);
		a_WorldInterface.SpawnMob(WorldPos.x, WorldPos.y, WorldPos.z, mtZombiePigman, false);
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		if (!cChunkDef::IsValidHeight(a_Position.addedY(-1)) || !cChunkDef::IsValidHeight(a_Position.addedY(1)))
		{
			return false;  // Must be 1 away from the boundary, there will always be another portal or an obsidian between the portal block and the boundary.
		}

		switch (Block::NetherPortal::Axis(a_Chunk.GetBlock(a_Position)))
		{
			case Block::NetherPortal::Axis::X:
			{
				static const std::array<Vector3i, 4> PortalCheck
				{
					{
						{ 0, 1, 0 },
						{ 0, -1, 0 },
						{ 1, 0, 0 },
						{ -1, 0, 0 },
					}
				};

				for (const auto & Direction : PortalCheck)
				{
					BlockState Block;
					a_Chunk.UnboundedRelGetBlock(a_Position + Direction, Block);
					if ((Block.Type() != BlockType::NetherPortal) && (Block.Type() != BlockType::Obsidian))
					{
						return false;
					}
				}
				break;
			}
			case Block::NetherPortal::Axis::Z:
			{
				static const std::array<Vector3i, 4> PortalCheck
				{
					{
						{ 0, 1, 0 },
						{ 0, -1, 0 },
						{ 0, 0, -1 },
						{ 0, 0, 1 },
					}
				};

				for (const auto & Direction : PortalCheck)
				{
					BlockState Block;
					a_Chunk.UnboundedRelGetBlock(a_Position + Direction, Block);
					if ((Block.Type() != BlockType::NetherPortal) && (Block.Type() != BlockType::Obsidian))
					{
						return false;
					}
				}
				break;
			}
		}
		return true;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 24;
	}
} ;




