#pragma once

#include "BlockHandler.h"


class cBlockVineHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	enum class Directions
	{
		East = 0,
		North = 1,
		South = 2,
		Up = 3,
		West = 4
	};

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_BlockType
	) const override
	{
		// TODO: Disallow placement where the vine doesn't attach to something properly
		BLOCKTYPE BlockType = 0;
		BlockState BlockMeta;
		a_ChunkInterface.GetBlockTypeMeta(a_PlacedBlockPos, BlockType, BlockMeta);
		if (BlockType == m_BlockType)
		{
			a_BlockMeta = BlockMeta | DirectionToMetaData(a_ClickedBlockFace);
		}
		else
		{
			a_BlockMeta = DirectionToMetaData(a_ClickedBlockFace);
		}
		a_BlockType = m_BlockType;
		Block::Vine::Vine()
		return true;
	}





	virtual cItems ConvertToPickups(BlockState a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Only drops self when using shears, otherwise drops nothing:
		if ((a_Tool == nullptr) || (a_Tool->m_ItemType != E_ITEM_SHEARS))
		{
			return {};
		}
		return cItem(Item::Vine);
	}





	/** Returns true if the specified block type is good for vines to attach to */
	static bool IsBlockAttachable(BlockState a_BlockType)
	{
		switch (a_BlockType.Type())
		{
			case BlockType::Chest:
			case BlockType::EnderChest:
			case BlockType::Piston:
			case BlockType::PistonHead:
			case BlockType::Repeater:
			case BlockType::Glass:
			case BlockType::BlackStainedGlass:
			case BlockType::BlueStainedGlass:
			case BlockType::BrownStainedGlass:
			case BlockType::CyanStainedGlass:
			case BlockType::GrayStainedGlass:
			case BlockType::GreenStainedGlass:
			case BlockType::LightBlueStainedGlass:
			case BlockType::LightGrayStainedGlass:
			case BlockType::LimeStainedGlass:
			case BlockType::MagentaStainedGlass:
			case BlockType::OrangeStainedGlass:
			case BlockType::PinkStainedGlass:
			case BlockType::PurpleStainedGlass:
			case BlockType::RedStainedGlass:
			case BlockType::YellowStainedGlass:
			case BlockType::WhiteStainedGlass:
			case BlockType::StickyPiston:
			case BlockType::TrappedChest:
			{
				// You can't attach a vine to this solid blocks.
				return false;
			}
			default:
			{
				return cBlockInfo::IsSolid(a_BlockType);
			}
		}
	}





	/** Returns the meta that has the maximum allowable sides of the vine, given the surroundings */
	static BlockState GetMaxMeta(cChunk & a_Chunk, Vector3i a_RelPos)
	{
		static const struct
		{
			int x, z;
			BlockState Bit;
		} Coords[] =
		{
			{ 0,  1, 1},  // south, ZP
			{-1,  0, 2},  // west,  XM
			{ 0, -1, 4},  // north, ZM
			{ 1,  0, 8},  // east,  XP
		} ;
		BlockState res = 0;
		for (auto & Coord : Coords)
		{
			BLOCKTYPE  BlockType;
			BlockState BlockMeta;
			auto checkPos = a_RelPos.addedXZ(Coord.x, Coord.z);
			if (
				a_Chunk.UnboundedRelGetBlock(checkPos.x, checkPos.y, checkPos.z, BlockType, BlockMeta) &&
				IsBlockAttachable(BlockType)
			)
			{
				res |= Coord.Bit;
			}
		}
		return res;
	}





	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		a_ChunkInterface.DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk)
		{
			const auto a_RelPos = cChunkDef::AbsoluteToRelative(a_BlockPos);
			BlockState CurMeta = a_Chunk.GetMeta(a_RelPos);
			BlockState MaxMeta = GetMaxMeta(a_Chunk, a_RelPos);

			// Check if vine above us, add its meta to MaxMeta
			if ((a_RelPos.y < cChunkDef::Height - 1) && (a_Chunk.GetBlock(a_RelPos.addedY(1)) == m_BlockType))
			{
				MaxMeta |= a_Chunk.GetMeta(a_RelPos.addedY(1));
			}

			BlockState Common = CurMeta & MaxMeta;  // Neighbors that we have and are legal
			if (Common != CurMeta)
			{
				// There is a neighbor missing, need to update the meta or even destroy the block
				bool HasTop = (a_RelPos.y < cChunkDef::Height - 1) && IsBlockAttachable(a_Chunk.GetBlock(a_RelPos.addedY(1)));
				if ((Common == 0) && !HasTop)
				{
					// The vine just lost all its support, destroy the block:
					a_Chunk.SetBlock(a_RelPos, BlockType::AIR, 0);
					return false;
				}
				a_Chunk.SetBlock(a_RelPos, m_BlockType, Common);
			}

			return false;
		});
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, BlockState a_Block) const override
	{
		return true;
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		UNUSED(a_ChunkInterface);
		UNUSED(a_WorldInterface);

		// Vine cannot grow down if at the bottom:
		auto GrowPos = a_RelPos.addedY(-1);
		if (!cChunkDef::IsValidHeight(GrowPos.y))
		{
			return;
		}

		// Grow one block down, if possible:
		BlockState DestBlock;
		a_Chunk.UnboundedRelGetBlockType(GrowPos, DestBlock);
		if (cBlockAirHandler::IsBlockAir(DestBlock))
		{
			auto WorldPos = a_Chunk.RelativeToAbsolute(GrowPos);
			if (!a_PluginInterface.CallHookBlockSpread(WorldPos.x, WorldPos.y, WorldPos.z, ssVineSpread))
			{
				a_Chunk.UnboundedRelSetBlock(GrowPos, a_Chunk.GetBlock(a_RelPos));
			}
		}
	}





	virtual BlockState RotateCCW(BlockState a_Block) const override
	{
		using namespace Block;
		return Vine::Vine(Vine::North(a_Block), Vine::West(a_Block), Vine::East(a_Block), Vine::Up(a_Block), Vine::South(a_Block));
	}





	virtual BlockState RotateCW(BlockState a_Block) const override
	{
		using namespace Block;
		return Vine::Vine(Vine::South(a_Block), Vine::East(a_Block), Vine::West(a_Block), Vine::Up(a_Block), Vine::North(a_Block));
	}





	virtual BlockState MirrorXY(BlockState a_Block) const override
	{
		using namespace Block;
		return Vine::Vine(Vine::West(a_Block), Vine::North(a_Block), Vine::South(a_Block), Vine::Up(a_Block), Vine::East(a_Block));
	}





	virtual BlockState MirrorYZ(BlockState a_Block) const override
	{
		using namespace Block;
		return Vine::Vine(Vine::East(a_Block), Vine::South(a_Block), Vine::North(a_Block), Vine::Up(a_Block), Vine::West(a_Block));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
} ;




