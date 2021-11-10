
#pragma once

#include "Blocks/BlockStairs.h"
#include "Items/ItemHandler.h"
#include "Registries/BlockItemConverter.h"
#include "Protocol/Palettes/Upgrade.h"





class cItemStairsHandler final  :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		BlockState BlockToPlace;
		using namespace Block;

		auto BlockFace = RotationToBlockFace(a_Player.GetYaw());
		bool Inverse = false;
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_TOP:    break;
			case BLOCK_FACE_BOTTOM: Inverse = true; break;  // When placing onto a bottom face, always place an upside-down stairs block
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			case BLOCK_FACE_WEST:
			{
				// When placing onto a sideways face, check cursor, if in top half, make it an upside-down stairs block
				if (a_CursorPosition.y > 8)
				{
					Inverse = true;
				}
				break;
			}
			case BLOCK_FACE_NONE: return false;
		}

		switch (BlockItemConverter::FromItem(a_HeldItem.m_ItemType))
		{
			case BlockType::AcaciaStairs:                  BlockToPlace = AcaciaStairs::AcaciaStairs(                                   BlockFace, Inverse ? AcaciaStairs::Half::Top                  : AcaciaStairs::Half::Bottom,                  AcaciaStairs::Shape::Straight); break;
			case BlockType::AndesiteStairs:                BlockToPlace = AndesiteStairs::AndesiteStairs(                               BlockFace, Inverse ? AndesiteStairs::Half::Top                : AndesiteStairs::Half::Bottom,                AndesiteStairs::Shape::Straight); break;
			case BlockType::BirchStairs:                   BlockToPlace = BirchStairs::BirchStairs(                                     BlockFace, Inverse ? BirchStairs::Half::Top                   : BirchStairs::Half::Bottom,                   BirchStairs::Shape::Straight); break;
			case BlockType::BlackstoneStairs:              BlockToPlace = BlackstoneStairs::BlackstoneStairs(                           BlockFace, Inverse ? BlackstoneStairs::Half::Top              : BlackstoneStairs::Half::Bottom,              BlackstoneStairs::Shape::Straight); break;
			case BlockType::BrickStairs:                   BlockToPlace = BrickStairs::BrickStairs(                                     BlockFace, Inverse ? BrickStairs::Half::Top                   : BrickStairs::Half::Bottom,                   BrickStairs::Shape::Straight); break;
			case BlockType::CobblestoneStairs:             BlockToPlace = CobblestoneStairs::CobblestoneStairs(                         BlockFace, Inverse ? CobblestoneStairs::Half::Top             : CobblestoneStairs::Half::Bottom,             CobblestoneStairs::Shape::Straight); break;
			case BlockType::CrimsonStairs:                 BlockToPlace = CrimsonStairs::CrimsonStairs(                                 BlockFace, Inverse ? CrimsonStairs::Half::Top                 : CrimsonStairs::Half::Bottom,                 CrimsonStairs::Shape::Straight); break;
			case BlockType::DarkOakStairs:                 BlockToPlace = DarkOakStairs::DarkOakStairs(                                 BlockFace, Inverse ? DarkOakStairs::Half::Top                 : DarkOakStairs::Half::Bottom,                 DarkOakStairs::Shape::Straight); break;
			case BlockType::DarkPrismarineStairs:          BlockToPlace = DarkPrismarineStairs::DarkPrismarineStairs(                   BlockFace, Inverse ? DarkPrismarineStairs::Half::Top          : DarkPrismarineStairs::Half::Bottom,          DarkPrismarineStairs::Shape::Straight); break;
			case BlockType::DioriteStairs:                 BlockToPlace = DioriteStairs::DioriteStairs(                                 BlockFace, Inverse ? DioriteStairs::Half::Top                 : DioriteStairs::Half::Bottom,                 DioriteStairs::Shape::Straight); break;
			case BlockType::EndStoneBrickStairs:           BlockToPlace = EndStoneBrickStairs::EndStoneBrickStairs(                     BlockFace, Inverse ? EndStoneBrickStairs::Half::Top           : EndStoneBrickStairs::Half::Bottom,           EndStoneBrickStairs::Shape::Straight); break;
			case BlockType::GraniteStairs:                 BlockToPlace = GraniteStairs::GraniteStairs(                                 BlockFace, Inverse ? GraniteStairs::Half::Top                 : GraniteStairs::Half::Bottom,                 GraniteStairs::Shape::Straight); break;
			case BlockType::JungleStairs:                  BlockToPlace = JungleStairs::JungleStairs(                                   BlockFace, Inverse ? JungleStairs::Half::Top                  : JungleStairs::Half::Bottom,                  JungleStairs::Shape::Straight); break;
			case BlockType::MossyCobblestoneStairs:        BlockToPlace = MossyCobblestoneStairs::MossyCobblestoneStairs(               BlockFace, Inverse ? MossyCobblestoneStairs::Half::Top        : MossyCobblestoneStairs::Half::Bottom,        MossyCobblestoneStairs::Shape::Straight); break;
			case BlockType::MossyStoneBrickStairs:         BlockToPlace = MossyStoneBrickStairs::MossyStoneBrickStairs(                 BlockFace, Inverse ? MossyStoneBrickStairs::Half::Top         : MossyStoneBrickStairs::Half::Bottom,         MossyStoneBrickStairs::Shape::Straight); break;
			case BlockType::NetherBrickStairs:             BlockToPlace = NetherBrickStairs::NetherBrickStairs(                         BlockFace, Inverse ? NetherBrickStairs::Half::Top             : NetherBrickStairs::Half::Bottom,             NetherBrickStairs::Shape::Straight); break;
			case BlockType::OakStairs:                     BlockToPlace = OakStairs::OakStairs(                                         BlockFace, Inverse ? OakStairs::Half::Top                     : OakStairs::Half::Bottom,                     OakStairs::Shape::Straight); break;
			case BlockType::PolishedAndesiteStairs:        BlockToPlace = PolishedAndesiteStairs::PolishedAndesiteStairs(               BlockFace, Inverse ? PolishedAndesiteStairs::Half::Top        : PolishedAndesiteStairs::Half::Bottom,        PolishedAndesiteStairs::Shape::Straight); break;
			case BlockType::PolishedBlackstoneBrickStairs: BlockToPlace = PolishedBlackstoneBrickStairs::PolishedBlackstoneBrickStairs( BlockFace, Inverse ? PolishedBlackstoneBrickStairs::Half::Top : PolishedBlackstoneBrickStairs::Half::Bottom, PolishedBlackstoneBrickStairs::Shape::Straight); break;
			case BlockType::PolishedBlackstoneStairs:      BlockToPlace = PolishedBlackstoneStairs::PolishedBlackstoneStairs(           BlockFace, Inverse ? PolishedBlackstoneStairs::Half::Top      : PolishedBlackstoneStairs::Half::Bottom,      PolishedBlackstoneStairs::Shape::Straight); break;
			case BlockType::PolishedDioriteStairs:         BlockToPlace = PolishedDioriteStairs::PolishedDioriteStairs(                 BlockFace, Inverse ? PolishedDioriteStairs::Half::Top         : PolishedDioriteStairs::Half::Bottom,         PolishedDioriteStairs::Shape::Straight); break;
			case BlockType::PolishedGraniteStairs:         BlockToPlace = PolishedGraniteStairs::PolishedGraniteStairs(                 BlockFace, Inverse ? PolishedGraniteStairs::Half::Top         : PolishedGraniteStairs::Half::Bottom,         PolishedGraniteStairs::Shape::Straight); break;
			case BlockType::PrismarineBrickStairs:         BlockToPlace = PrismarineBrickStairs::PrismarineBrickStairs(                 BlockFace, Inverse ? PrismarineBrickStairs::Half::Top         : PrismarineBrickStairs::Half::Bottom,         PrismarineBrickStairs::Shape::Straight); break;
			case BlockType::PrismarineStairs:              BlockToPlace = PrismarineStairs::PrismarineStairs(                           BlockFace, Inverse ? PrismarineStairs::Half::Top              : PrismarineStairs::Half::Bottom,              PrismarineStairs::Shape::Straight); break;
			case BlockType::QuartzStairs:                  BlockToPlace = QuartzStairs::QuartzStairs(                                   BlockFace, Inverse ? QuartzStairs::Half::Top                  : QuartzStairs::Half::Bottom,                  QuartzStairs::Shape::Straight); break;
			case BlockType::PurpurStairs:                  BlockToPlace = PurpurStairs::PurpurStairs(                                   BlockFace, Inverse ? PurpurStairs::Half::Top                  : PurpurStairs::Half::Bottom,                  PurpurStairs::Shape::Straight); break;
			case BlockType::RedNetherBrickStairs:          BlockToPlace = RedNetherBrickStairs::RedNetherBrickStairs(                   BlockFace, Inverse ? RedNetherBrickStairs::Half::Top          : RedNetherBrickStairs::Half::Bottom,          RedNetherBrickStairs::Shape::Straight); break;
			case BlockType::RedSandstoneStairs:            BlockToPlace = RedSandstoneStairs::RedSandstoneStairs(                       BlockFace, Inverse ? RedSandstoneStairs::Half::Top            : RedSandstoneStairs::Half::Bottom,            RedSandstoneStairs::Shape::Straight); break;
			case BlockType::SandstoneStairs:               BlockToPlace = SandstoneStairs::SandstoneStairs(                             BlockFace, Inverse ? SandstoneStairs::Half::Top               : SandstoneStairs::Half::Bottom,               SandstoneStairs::Shape::Straight); break;
			case BlockType::SmoothQuartzStairs:            BlockToPlace = SmoothQuartzStairs::SmoothQuartzStairs(                       BlockFace, Inverse ? SmoothQuartzStairs::Half::Top            : SmoothQuartzStairs::Half::Bottom,            SmoothQuartzStairs::Shape::Straight); break;
			case BlockType::SmoothRedSandstoneStairs:      BlockToPlace = SmoothRedSandstoneStairs::SmoothRedSandstoneStairs(           BlockFace, Inverse ? SmoothRedSandstoneStairs::Half::Top      : SmoothRedSandstoneStairs::Half::Bottom,      SmoothRedSandstoneStairs::Shape::Straight); break;
			case BlockType::SmoothSandstoneStairs:         BlockToPlace = SmoothSandstoneStairs::SmoothSandstoneStairs(                 BlockFace, Inverse ? SmoothSandstoneStairs::Half::Top         : SmoothSandstoneStairs::Half::Bottom,         SmoothSandstoneStairs::Shape::Straight); break;
			case BlockType::SpruceStairs:                  BlockToPlace = SpruceStairs::SpruceStairs(                                   BlockFace, Inverse ? SpruceStairs::Half::Top                  : SpruceStairs::Half::Bottom,                  SpruceStairs::Shape::Straight); break;
			case BlockType::StoneBrickStairs:              BlockToPlace = StoneBrickStairs::StoneBrickStairs(                           BlockFace, Inverse ? StoneBrickStairs::Half::Top              : StoneBrickStairs::Half::Bottom,              StoneBrickStairs::Shape::Straight); break;
			case BlockType::StoneStairs:                   BlockToPlace = StoneStairs::StoneStairs(                                     BlockFace, Inverse ? StoneStairs::Half::Top                   : StoneStairs::Half::Bottom,                   StoneStairs::Shape::Straight); break;
			case BlockType::WarpedStairs:                  BlockToPlace = WarpedStairs::WarpedStairs(                                   BlockFace, Inverse ? WarpedStairs::Half::Top                  : WarpedStairs::Half::Bottom,                  WarpedStairs::Shape::Straight); break;
			default: return false;
		}
		return a_Player.PlaceBlock(a_PlacePosition, BlockToPlace);
	}
};
