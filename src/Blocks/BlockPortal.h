
#pragma once

#include "BlockHandler.h"





class cBlockPortalHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// No pickups
		return {};
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		// Spawn zombie pigmen with a 0.05% chance:
		if (GetRandomProvider().RandBool(0.9995))
		{
			return;
		}
		auto WorldPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		a_WorldInterface.SpawnMob(WorldPos.x, WorldPos.y, WorldPos.z, mtZombiePigman, false);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if ((a_RelPos.y <= 0) || (a_RelPos.y >= cChunkDef::Height - 1))
		{
			return false;  // In case someone places a portal with meta 1 or 2 at boundaries, and server tries to get invalid coords at Y - 1 or Y + 1
		}

		switch (Block::NetherPortal::Axis(a_Chunk.GetBlock(a_RelPos)))
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
					a_Chunk.UnboundedRelGetBlockType(a_RelPos + Direction, Block);
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
					a_Chunk.UnboundedRelGetBlockType(a_RelPos + Direction, Block);
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




