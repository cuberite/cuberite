#pragma once

#include "BlockHandler.h"


class cBlockVinesHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	enum class Directions
	{
		East = 0,
		North = 1,
		South = 2,
		Up = 3,
		West = 4
	};

	static inline bool IsAttachedTo(BlockState a_Block, eBlockFace a_Face)
	{
		switch (a_Face)
		{
			case BLOCK_FACE_XM: return Block::Vine::West(a_Block);
			case BLOCK_FACE_XP: return Block::Vine::East(a_Block);
			case BLOCK_FACE_YP: return Block::Vine::Up(a_Block);
			case BLOCK_FACE_ZM: return Block::Vine::North(a_Block);
			case BLOCK_FACE_ZP: return Block::Vine::South(a_Block);
			default: return false;
		}
	}

	static inline bool OnlyAttachedTo(BlockState a_Block, eBlockFace a_Face)
	{
		using namespace Block;
		switch (a_Face)
		{
			case BLOCK_FACE_XM: return (Vine::West(a_Block) &&  !(Vine::East(a_Block) || Vine::Up(a_Block)     || Vine::North(a_Block) || Vine::South(a_Block)));
			case BLOCK_FACE_XP: return (Vine::East(a_Block) &&  !(Vine::West(a_Block) || Vine::Up(a_Block)     || Vine::North(a_Block) || Vine::South(a_Block)));
			case BLOCK_FACE_YP: return (Vine::Up(a_Block) &&    !(Vine::East(a_Block) || Vine::West(a_Block)   || Vine::North(a_Block) || Vine::South(a_Block)));
			case BLOCK_FACE_ZM: return (Vine::North(a_Block) && !(Vine::East(a_Block) || Vine::West(a_Block)   || Vine::Up(a_Block)    || Vine::South(a_Block)));
			case BLOCK_FACE_ZP: return (Vine::South(a_Block) && !(Vine::East(a_Block) || Vine::West(a_Block)   || Vine::Up(a_Block)    || Vine::North(a_Block)));
			default: return false;
		}
	}

private:

	static const unsigned char VINE_LOST_SUPPORT = 16;
	static const unsigned char VINE_UNCHANGED = 17;

	// TODO(12xx12)
	/*
	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		// return GetMaxMeta(a_Chunk, a_Position, a_Self) != VINE_LOST_SUPPORT;
		return true;
	}
	*/


	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Only drops self when using shears, otherwise drops nothing:
		if ((a_Tool == nullptr) || (a_Tool->m_ItemType != Item::Shears))
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





	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		using namespace Block;
		BlockState Self = a_ChunkInterface.GetBlock(a_BlockPos);

		// If not attached - ignore
		if (!IsAttachedTo(Self, a_WhichNeighbor))
		{
			return;
		}

		auto BlockToCheck = a_ChunkInterface.GetBlock(AddFaceDirection(a_BlockPos, a_WhichNeighbor));

		// If only attached to this face: destroy
		if (OnlyAttachedTo(Self, a_WhichNeighbor) && !IsBlockAttachable(BlockToCheck))
		{
			a_ChunkInterface.SetBlock(a_BlockPos, Block::Air::Air());
			return;
		}

		switch (a_WhichNeighbor)
		{
			case BLOCK_FACE_XM:
			{
				if (!IsBlockAttachable(BlockToCheck))
				{
					Self = Vine::Vine(Vine::East(Self), Vine::North(Self), Vine::South(Self), Vine::Up(Self), false);
				}
				break;
			}
			case BLOCK_FACE_XP:
			{
				if (!IsBlockAttachable(BlockToCheck))
				{
					Self = Vine::Vine(false, Vine::North(Self), Vine::South(Self), Vine::Up(Self), Vine::West(Self));
				}
				break;
			}
			case BLOCK_FACE_YP:
			{
				if (!IsBlockAttachable(BlockToCheck))
				{
					Self = Vine::Vine(Vine::East(Self), Vine::North(Self), Vine::South(Self), false, Vine::West(Self));
				}
				break;
			}
			case BLOCK_FACE_ZM:
			{
				if (!IsBlockAttachable(BlockToCheck))
				{
					Self = Vine::Vine(Vine::East(Self), false, Vine::South(Self), Vine::Up(Self), Vine::West(Self));
				}
				break;
			}
			case BLOCK_FACE_ZP:
			{
				if (!IsBlockAttachable(BlockToCheck))
				{
					Self = Vine::Vine(Vine::East(Self), Vine::North(Self), false, Vine::Up(Self), Vine::West(Self));
				}
				break;
			}
			default: break;
		}
		a_ChunkInterface.SetBlock(a_BlockPos, Self);
		return;
	}





	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, BlockState a_ClickedBlock, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const override
	{
		return !a_ClickedDirectly || (BlockItemConverter::FromItem(a_HeldItem.m_ItemType) != m_BlockType);
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
		if (!cChunkDef::IsValidHeight(GrowPos))
		{
			return;
		}

		// Grow one block down, if possible:
		BlockState DestBlock;
		a_Chunk.UnboundedRelGetBlock(GrowPos, DestBlock);
		if (cBlockAirHandler::IsBlockAir(DestBlock))
		{
			auto WorldPos = a_Chunk.RelativeToAbsolute(GrowPos);
			if (!a_PluginInterface.CallHookBlockSpread(WorldPos, ssVineSpread))
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




