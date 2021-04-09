#pragma once

#include "BlockHandler.h"


class cBlockVineHandler final :
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

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		using namespace Block;
		// TODO: Disallow placement where the vine doesn't attach to something properly
		auto BlockToReplace = a_ChunkInterface.GetBlock(a_PlacedBlockPos);
		if (BlockToReplace.Type() == m_BlockType)
		{
			if (IsAttachedTo(BlockToReplace, a_ClickedBlockFace))
			{
				// There is already a vine at that rotation
				return false;
			}
			switch (a_ClickedBlockFace)
			{
				//                                      East                         North                        South                        Up                        West
				case BLOCK_FACE_XM: a_Block = Vine::Vine(Vine::East(BlockToReplace), Vine::North(BlockToReplace), Vine::South(BlockToReplace), Vine::Up(BlockToReplace), true);                       break;
				case BLOCK_FACE_XP: a_Block = Vine::Vine(true,                       Vine::North(BlockToReplace), Vine::South(BlockToReplace), Vine::Up(BlockToReplace), Vine::West(BlockToReplace)); break;
				case BLOCK_FACE_YP: a_Block = Vine::Vine(Vine::East(BlockToReplace), Vine::North(BlockToReplace), Vine::South(BlockToReplace), true,                     Vine::West(BlockToReplace)); break;
				case BLOCK_FACE_ZM: a_Block = Vine::Vine(Vine::East(BlockToReplace), true,                        Vine::South(BlockToReplace), Vine::Up(BlockToReplace), Vine::West(BlockToReplace)); break;
				case BLOCK_FACE_ZP: a_Block = Vine::Vine(Vine::East(BlockToReplace), Vine::North(BlockToReplace), true,                        Vine::Up(BlockToReplace), Vine::West(BlockToReplace)); break;
				default: return false;
			}
		}
		else
		{
			switch (a_ClickedBlockFace)
			{
				//                                       East   North  South  Up     West
				case BLOCK_FACE_XM: a_Block = Vine::Vine(false, false, false, false, true); break;
				case BLOCK_FACE_XP: a_Block = Vine::Vine(true,  false, false, false, false); break;
				case BLOCK_FACE_YP: a_Block = Vine::Vine(false, false, false, true,  false); break;
				case BLOCK_FACE_ZM: a_Block = Vine::Vine(false, true,  false, false, false); break;
				case BLOCK_FACE_ZP: a_Block = Vine::Vine(false, false, true,  false, false); break;
				default: return false;
			}
		}
		return true;
	}





	virtual cItems ConvertToPickups(BlockState a_BlockMeta, const cItem * a_Tool) const override
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
		a_Chunk.UnboundedRelGetBlock(GrowPos, DestBlock);
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




