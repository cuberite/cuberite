
#pragma once

#include "Items/ItemHandler.h"
#include "Protocol/Palettes/Upgrade.h"
#include "Registries/BlockItemConverter.h"





class cItemSlabHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool IsPlaceable(void) const override
	{
		return true;
	}

protected:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		auto OldBlock = a_Player.GetWorld()->GetBlock(a_PlacePosition);
		// Confer BlockSlab.h, which we're in cahoots with to make the below logic work.

		// If clicking a slab, combine it into a double-slab:
		if (cBlockSlabHandler::IsAnySlabType(OldBlock))
		{
			if (!a_Player.PlaceBlock(a_PlacePosition, GetDoubleSlab(OldBlock)))
			{
				return false;
			}

			a_Player.SendBlocksAround(a_PlacePosition, 2);  // (see below)
			return true;
		}

		bool IsTop = false;

		// Set the correct metadata based on player equipped item (i.e. a_BlockMeta not initialised yet)
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_TOP:
			{
				// Bottom half slab block
				IsTop = false;
				break;
			}
			case BLOCK_FACE_BOTTOM:
			{
				// Top half slab block
				IsTop = true;
				break;
			}
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			case BLOCK_FACE_WEST:
			{
				if (a_CursorPosition.y > 7)
				{
					// Cursor at top half of block, place top slab
					IsTop = true;
					break;
				}
				else
				{
					// Cursor at bottom half of block, place bottom slab
					IsTop = false;
					break;
				}
			}
			case BLOCK_FACE_NONE: return false;
		}

		BlockState BlockToPlace;

		bool IsPlacedInWater =a_Player.GetWorld()->GetBlock(a_PlacePosition).Type() == BlockType::Water;
		using namespace Block;

#define PLACE_SLAB(SlabType) \
	{\
		BlockToPlace = SlabType::SlabType(IsTop ? SlabType::Type::Top : SlabType::Type::Bottom, IsPlacedInWater);\
		break;\
	}\

		switch (m_ItemType)
		{
			case Item::AcaciaSlab:
			{
				BlockToPlace = AcaciaSlab::AcaciaSlab(IsTop ? AcaciaSlab::Type::Top : AcaciaSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::AndesiteSlab:
			{
				BlockToPlace = AndesiteSlab::AndesiteSlab(IsTop ? AndesiteSlab::Type::Top : AndesiteSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::BirchSlab:
			{
				BlockToPlace = BirchSlab::BirchSlab(IsTop ? BirchSlab::Type::Top : BirchSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::BlackstoneSlab:
			{
				BlockToPlace = BlackstoneSlab::BlackstoneSlab(IsTop ? BlackstoneSlab::Type::Top : BlackstoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::BrickSlab:
			{
				BlockToPlace = BrickSlab::BrickSlab(IsTop ? BrickSlab::Type::Top : BrickSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::CobblestoneSlab:
			{
				BlockToPlace = CobblestoneSlab::CobblestoneSlab(IsTop ? CobblestoneSlab::Type::Top : CobblestoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::CrimsonSlab:
			{
				BlockToPlace = CrimsonSlab::CrimsonSlab(IsTop ? CrimsonSlab::Type::Top : CrimsonSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::CutRedSandstoneSlab:
			{
				BlockToPlace = CutRedSandstoneSlab::CutRedSandstoneSlab(IsTop ? CutRedSandstoneSlab::Type::Top : CutRedSandstoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::CutSandstoneSlab:
			{
				BlockToPlace = CutSandstoneSlab::CutSandstoneSlab(IsTop ? CutSandstoneSlab::Type::Top : CutSandstoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::DarkOakSlab:
			{
				BlockToPlace = DarkOakSlab::DarkOakSlab(IsTop ? DarkOakSlab::Type::Top : DarkOakSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::DarkPrismarineSlab:
			{
				BlockToPlace = DarkPrismarineSlab::DarkPrismarineSlab(IsTop ? DarkPrismarineSlab::Type::Top : DarkPrismarineSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::DioriteSlab:
			{
				BlockToPlace = DioriteSlab::DioriteSlab(IsTop ? DioriteSlab::Type::Top : DioriteSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::EndStoneBrickSlab:
			{
				BlockToPlace = EndStoneBrickSlab::EndStoneBrickSlab(IsTop ? EndStoneBrickSlab::Type::Top : EndStoneBrickSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::GraniteSlab:
			{
				BlockToPlace = GraniteSlab::GraniteSlab(IsTop ? GraniteSlab::Type::Top : GraniteSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::JungleSlab:
			{
				BlockToPlace = JungleSlab::JungleSlab(IsTop ? JungleSlab::Type::Top : JungleSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::MossyCobblestoneSlab:
			{
				BlockToPlace = MossyCobblestoneSlab::MossyCobblestoneSlab(IsTop ? MossyCobblestoneSlab::Type::Top : MossyCobblestoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::MossyStoneBrickSlab:
			{
				BlockToPlace = MossyStoneBrickSlab::MossyStoneBrickSlab(IsTop ? MossyStoneBrickSlab::Type::Top : MossyStoneBrickSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::NetherBrickSlab:
			{
				BlockToPlace = NetherBrickSlab::NetherBrickSlab(IsTop ? NetherBrickSlab::Type::Top : NetherBrickSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::OakSlab:
			{
				BlockToPlace = OakSlab::OakSlab(IsTop ? OakSlab::Type::Top : OakSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PetrifiedOakSlab:
			{
				BlockToPlace = PetrifiedOakSlab::PetrifiedOakSlab(IsTop ? PetrifiedOakSlab::Type::Top : PetrifiedOakSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PolishedAndesiteSlab:
			{
				BlockToPlace = PolishedAndesiteSlab::PolishedAndesiteSlab(IsTop ? PolishedAndesiteSlab::Type::Top : PolishedAndesiteSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PolishedBlackstoneBrickSlab:
			{
				BlockToPlace = PolishedBlackstoneBrickSlab::PolishedBlackstoneBrickSlab(IsTop ? PolishedBlackstoneBrickSlab::Type::Top : PolishedBlackstoneBrickSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PolishedBlackstoneSlab:
			{
				BlockToPlace = PolishedBlackstoneSlab::PolishedBlackstoneSlab(IsTop ? PolishedBlackstoneSlab::Type::Top : PolishedBlackstoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PolishedDioriteSlab:
			{
				BlockToPlace = PolishedDioriteSlab::PolishedDioriteSlab(IsTop ? PolishedDioriteSlab::Type::Top : PolishedDioriteSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PolishedGraniteSlab:
			{
				BlockToPlace = PolishedGraniteSlab::PolishedGraniteSlab(IsTop ? PolishedGraniteSlab::Type::Top : PolishedGraniteSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PrismarineBrickSlab:
			{
				BlockToPlace = PrismarineBrickSlab::PrismarineBrickSlab(IsTop ? PrismarineBrickSlab::Type::Top : PrismarineBrickSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PrismarineSlab:
			{
				BlockToPlace = PrismarineSlab::PrismarineSlab(IsTop ? PrismarineSlab::Type::Top : PrismarineSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::PurpurSlab:
			{
				BlockToPlace = PurpurSlab::PurpurSlab(IsTop ? PurpurSlab::Type::Top : PurpurSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::QuartzSlab:
			{
				BlockToPlace = QuartzSlab::QuartzSlab(IsTop ? QuartzSlab::Type::Top : QuartzSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::RedNetherBrickSlab:
			{
				BlockToPlace = RedNetherBrickSlab::RedNetherBrickSlab(IsTop ? RedNetherBrickSlab::Type::Top : RedNetherBrickSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::RedSandstoneSlab:
			{
				BlockToPlace = RedSandstoneSlab::RedSandstoneSlab(IsTop ? RedSandstoneSlab::Type::Top : RedSandstoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::SandstoneSlab:
			{
				BlockToPlace = SandstoneSlab::SandstoneSlab(IsTop ? SandstoneSlab::Type::Top : SandstoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::SmoothQuartzSlab:
			{
				BlockToPlace = SmoothQuartzSlab::SmoothQuartzSlab(IsTop ? SmoothQuartzSlab::Type::Top : SmoothQuartzSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::SmoothRedSandstoneSlab:
			{
				BlockToPlace = SmoothRedSandstoneSlab::SmoothRedSandstoneSlab(IsTop ? SmoothRedSandstoneSlab::Type::Top : SmoothRedSandstoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::SmoothSandstoneSlab:
			{
				BlockToPlace = SmoothSandstoneSlab::SmoothSandstoneSlab(IsTop ? SmoothSandstoneSlab::Type::Top : SmoothSandstoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::SmoothStoneSlab:
			{
				BlockToPlace = SmoothStoneSlab::SmoothStoneSlab(IsTop ? SmoothStoneSlab::Type::Top : SmoothStoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::SpruceSlab:
			{
				BlockToPlace = SpruceSlab::SpruceSlab(IsTop ? SpruceSlab::Type::Top : SpruceSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::StoneBrickSlab:
			{
				BlockToPlace = StoneBrickSlab::StoneBrickSlab(IsTop ? StoneBrickSlab::Type::Top : StoneBrickSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::StoneSlab:
			{
				BlockToPlace = StoneSlab::StoneSlab(IsTop ? StoneSlab::Type::Top : StoneSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::WarpedSlab:
			{
				BlockToPlace = WarpedSlab::WarpedSlab(IsTop ? WarpedSlab::Type::Top : WarpedSlab::Type::Bottom, IsPlacedInWater);
				break;
			}
			case Item::BambooMosaicSlab:            PLACE_SLAB(BambooMosaicSlab)
			case Item::BambooSlab:                  PLACE_SLAB(BambooSlab)
			case Item::CherrySlab:                  PLACE_SLAB(CherrySlab)
			case Item::CobbledDeepslateSlab:        PLACE_SLAB(CobbledDeepslateSlab)
			case Item::TuffSlab:                    PLACE_SLAB(TuffSlab)
			case Item::TuffBrickSlab:               PLACE_SLAB(TuffBrickSlab)
			case Item::PolishedTuffSlab:            PLACE_SLAB(PolishedTuffSlab)
			case Item::MudBrickSlab:                PLACE_SLAB(MudBrickSlab)
			case Item::MangroveSlab:                PLACE_SLAB(MangroveSlab)
			case Item::DeepslateTileSlab:           PLACE_SLAB(DeepslateTileSlab)
			case Item::DeepslateBrickSlab:          PLACE_SLAB(DeepslateBrickSlab)
			case Item::PolishedDeepslateSlab:       PLACE_SLAB(PolishedDeepslateSlab)
			case Item::CutCopperSlab:               PLACE_SLAB(CutCopperSlab)
			case Item::ExposedCutCopperSlab:        PLACE_SLAB(ExposedCutCopperSlab)
			case Item::WeatheredCutCopperSlab:      PLACE_SLAB(WeatheredCutCopperSlab)
			case Item::OxidizedCutCopperSlab:       PLACE_SLAB(OxidizedCutCopperSlab)
			case Item::WaxedCutCopperSlab:          PLACE_SLAB(WaxedCutCopperSlab)
			case Item::WaxedExposedCutCopperSlab:        PLACE_SLAB(WaxedExposedCutCopperSlab)
			case Item::WaxedWeatheredCutCopperSlab:      PLACE_SLAB(WaxedWeatheredCutCopperSlab)
			case Item::WaxedOxidizedCutCopperSlab:       PLACE_SLAB(WaxedOxidizedCutCopperSlab)
			case Item::ResinBrickSlab:                   PLACE_SLAB(ResinBrickSlab)
			default:
			{
				FLOGWARNING("{}: Item type not handled {}.", __FUNCTION__, m_ItemType);
				UNREACHABLE("Unhandled slab type");
			}
		}

		if (!a_Player.PlaceBlock(a_PlacePosition, BlockToPlace))
		{
			return false;
		}

		/* This is a workaround for versions < 1.13, where the client combines a slab in the
		direction of the clicked block face of a block ignoring build collision, rather than replacing said block.
		Resend blocks to the client to fix the bug.
		Ref.: https://forum.cuberite.org/thread-434-post-17388.html#pid17388 */
		a_Player.SendBlocksAround(a_PlacePosition, 2);

		return true;
	}



#define GET_DOUBLE_SLAB(SlabType) \
	return SlabType::SlabType(SlabType::Type::Double, SlabType::Waterlogged(BlockToPlace));\

	static BlockState GetDoubleSlab(BlockState BlockToPlace)
	{
		using namespace Block;
		switch (BlockToPlace.Type())
		{
			case BlockType::AcaciaSlab:                  return AcaciaSlab::AcaciaSlab(AcaciaSlab::Type::Double, AcaciaSlab::Waterlogged(BlockToPlace));
			case BlockType::AndesiteSlab:                return AndesiteSlab::AndesiteSlab(AndesiteSlab::Type::Double, AndesiteSlab::Waterlogged(BlockToPlace));
			case BlockType::BirchSlab:                   return BirchSlab::BirchSlab(BirchSlab::Type::Double, BirchSlab::Waterlogged(BlockToPlace));
			case BlockType::BlackstoneSlab:              return BlackstoneSlab::BlackstoneSlab(BlackstoneSlab::Type::Double, BlackstoneSlab::Waterlogged(BlockToPlace));
			case BlockType::BrickSlab:                   return BrickSlab::BrickSlab(BrickSlab::Type::Double, BrickSlab::Waterlogged(BlockToPlace));
			case BlockType::CobblestoneSlab:             return CobblestoneSlab::CobblestoneSlab(CobblestoneSlab::Type::Double, CobblestoneSlab::Waterlogged(BlockToPlace));
			case BlockType::CrimsonSlab:                 return CrimsonSlab::CrimsonSlab(CrimsonSlab::Type::Double, CrimsonSlab::Waterlogged(BlockToPlace));
			case BlockType::CutRedSandstoneSlab:         return CutRedSandstoneSlab::CutRedSandstoneSlab(CutRedSandstoneSlab::Type::Double, CutRedSandstoneSlab::Waterlogged(BlockToPlace));
			case BlockType::CutSandstoneSlab:            return CutSandstoneSlab::CutSandstoneSlab(CutSandstoneSlab::Type::Double, CutSandstoneSlab::Waterlogged(BlockToPlace));
			case BlockType::DarkOakSlab:                 return DarkOakSlab::DarkOakSlab(DarkOakSlab::Type::Double, DarkOakSlab::Waterlogged(BlockToPlace));
			case BlockType::DarkPrismarineSlab:          return DarkPrismarineSlab::DarkPrismarineSlab(DarkPrismarineSlab::Type::Double, DarkPrismarineSlab::Waterlogged(BlockToPlace));
			case BlockType::DioriteSlab:                 return DioriteSlab::DioriteSlab(DioriteSlab::Type::Double, DioriteSlab::Waterlogged(BlockToPlace));
			case BlockType::EndStoneBrickSlab:           return EndStoneBrickSlab::EndStoneBrickSlab(EndStoneBrickSlab::Type::Double, EndStoneBrickSlab::Waterlogged(BlockToPlace));
			case BlockType::GraniteSlab:                 return GraniteSlab::GraniteSlab(GraniteSlab::Type::Double, GraniteSlab::Waterlogged(BlockToPlace));
			case BlockType::JungleSlab:                  return JungleSlab::JungleSlab(JungleSlab::Type::Double, JungleSlab::Waterlogged(BlockToPlace));
			case BlockType::MossyCobblestoneSlab:        return MossyCobblestoneSlab::MossyCobblestoneSlab(MossyCobblestoneSlab::Type::Double, MossyCobblestoneSlab::Waterlogged(BlockToPlace));
			case BlockType::MossyStoneBrickSlab:         return MossyStoneBrickSlab::MossyStoneBrickSlab(MossyStoneBrickSlab::Type::Double, MossyStoneBrickSlab::Waterlogged(BlockToPlace));
			case BlockType::NetherBrickSlab:             return NetherBrickSlab::NetherBrickSlab(NetherBrickSlab::Type::Double, NetherBrickSlab::Waterlogged(BlockToPlace));
			case BlockType::OakSlab:                     return OakSlab::OakSlab(OakSlab::Type::Double, OakSlab::Waterlogged(BlockToPlace));
			case BlockType::PetrifiedOakSlab:            return PetrifiedOakSlab::PetrifiedOakSlab(PetrifiedOakSlab::Type::Double, PetrifiedOakSlab::Waterlogged(BlockToPlace));
			case BlockType::PolishedAndesiteSlab:        return PolishedAndesiteSlab::PolishedAndesiteSlab(PolishedAndesiteSlab::Type::Double, PolishedAndesiteSlab::Waterlogged(BlockToPlace));
			case BlockType::PolishedBlackstoneBrickSlab: return PolishedBlackstoneBrickSlab::PolishedBlackstoneBrickSlab(PolishedBlackstoneBrickSlab::Type::Double, PolishedBlackstoneBrickSlab::Waterlogged(BlockToPlace));
			case BlockType::PolishedBlackstoneSlab:      return PolishedBlackstoneSlab::PolishedBlackstoneSlab(PolishedBlackstoneSlab::Type::Double, PolishedBlackstoneSlab::Waterlogged(BlockToPlace));
			case BlockType::PolishedDioriteSlab:         return PolishedDioriteSlab::PolishedDioriteSlab(PolishedDioriteSlab::Type::Double, PolishedDioriteSlab::Waterlogged(BlockToPlace));
			case BlockType::PolishedGraniteSlab:         return PolishedGraniteSlab::PolishedGraniteSlab(PolishedGraniteSlab::Type::Double, PolishedGraniteSlab::Waterlogged(BlockToPlace));
			case BlockType::PrismarineBrickSlab:         return PrismarineBrickSlab::PrismarineBrickSlab(PrismarineBrickSlab::Type::Double, PrismarineBrickSlab::Waterlogged(BlockToPlace));
			case BlockType::PrismarineSlab:              return PrismarineSlab::PrismarineSlab(PrismarineSlab::Type::Double, PrismarineSlab::Waterlogged(BlockToPlace));
			case BlockType::PurpurSlab:                  return PurpurSlab::PurpurSlab(PurpurSlab::Type::Double, PurpurSlab::Waterlogged(BlockToPlace));
			case BlockType::QuartzSlab:                  return QuartzSlab::QuartzSlab(QuartzSlab::Type::Double, QuartzSlab::Waterlogged(BlockToPlace));
			case BlockType::RedNetherBrickSlab:          return RedNetherBrickSlab::RedNetherBrickSlab(RedNetherBrickSlab::Type::Double, RedNetherBrickSlab::Waterlogged(BlockToPlace));
			case BlockType::RedSandstoneSlab:            return RedSandstoneSlab::RedSandstoneSlab(RedSandstoneSlab::Type::Double, RedSandstoneSlab::Waterlogged(BlockToPlace));
			case BlockType::SandstoneSlab:               return SandstoneSlab::SandstoneSlab(SandstoneSlab::Type::Double, SandstoneSlab::Waterlogged(BlockToPlace));
			case BlockType::SmoothQuartzSlab:            return SmoothQuartzSlab::SmoothQuartzSlab(SmoothQuartzSlab::Type::Double, SmoothQuartzSlab::Waterlogged(BlockToPlace));
			case BlockType::SmoothRedSandstoneSlab:      return SmoothRedSandstoneSlab::SmoothRedSandstoneSlab(SmoothRedSandstoneSlab::Type::Double, SmoothRedSandstoneSlab::Waterlogged(BlockToPlace));
			case BlockType::SmoothSandstoneSlab:         return SmoothSandstoneSlab::SmoothSandstoneSlab(SmoothSandstoneSlab::Type::Double, SmoothSandstoneSlab::Waterlogged(BlockToPlace));
			case BlockType::SmoothStoneSlab:             return SmoothStoneSlab::SmoothStoneSlab(SmoothStoneSlab::Type::Double, SmoothStoneSlab::Waterlogged(BlockToPlace));
			case BlockType::SpruceSlab:                  return SpruceSlab::SpruceSlab(SpruceSlab::Type::Double, SpruceSlab::Waterlogged(BlockToPlace));
			case BlockType::StoneBrickSlab:              return StoneBrickSlab::StoneBrickSlab(StoneBrickSlab::Type::Double, StoneBrickSlab::Waterlogged(BlockToPlace));
			case BlockType::StoneSlab:                   return StoneSlab::StoneSlab(StoneSlab::Type::Double, StoneSlab::Waterlogged(BlockToPlace));
			case BlockType::WarpedSlab:                  return WarpedSlab::WarpedSlab(WarpedSlab::Type::Double, WarpedSlab::Waterlogged(BlockToPlace));
			case BlockType::BambooMosaicSlab:            GET_DOUBLE_SLAB(BambooMosaicSlab)
			case BlockType::BambooSlab:                  GET_DOUBLE_SLAB(BambooSlab)
			case BlockType::CherrySlab:                  GET_DOUBLE_SLAB(CherrySlab)
			case BlockType::CobbledDeepslateSlab:        GET_DOUBLE_SLAB(CobbledDeepslateSlab)
			case BlockType::TuffSlab:                    GET_DOUBLE_SLAB(TuffSlab)
			case BlockType::TuffBrickSlab:               GET_DOUBLE_SLAB(TuffBrickSlab)
			case BlockType::PolishedTuffSlab:            GET_DOUBLE_SLAB(PolishedTuffSlab)
			case BlockType::MudBrickSlab:                GET_DOUBLE_SLAB(MudBrickSlab)
			case BlockType::MangroveSlab:                GET_DOUBLE_SLAB(MangroveSlab)
			case BlockType::DeepslateTileSlab:           GET_DOUBLE_SLAB(DeepslateTileSlab)
			case BlockType::DeepslateBrickSlab:          GET_DOUBLE_SLAB(DeepslateBrickSlab)
			case BlockType::PolishedDeepslateSlab:       GET_DOUBLE_SLAB(PolishedDeepslateSlab)
			case BlockType::CutCopperSlab:               GET_DOUBLE_SLAB(CutCopperSlab)
			case BlockType::ExposedCutCopperSlab:        GET_DOUBLE_SLAB(ExposedCutCopperSlab)
			case BlockType::WeatheredCutCopperSlab:      GET_DOUBLE_SLAB(WeatheredCutCopperSlab)
			case BlockType::OxidizedCutCopperSlab:       GET_DOUBLE_SLAB(OxidizedCutCopperSlab)
			case BlockType::WaxedCutCopperSlab:          GET_DOUBLE_SLAB(WaxedCutCopperSlab)
			case BlockType::WaxedExposedCutCopperSlab:        GET_DOUBLE_SLAB(WaxedExposedCutCopperSlab)
			case BlockType::WaxedWeatheredCutCopperSlab:      GET_DOUBLE_SLAB(WaxedWeatheredCutCopperSlab)
			case BlockType::WaxedOxidizedCutCopperSlab:       GET_DOUBLE_SLAB(WaxedOxidizedCutCopperSlab)
			case BlockType::PaleOakSlab:                 GET_DOUBLE_SLAB(PaleOakSlab)
			case BlockType::ResinBrickSlab:              GET_DOUBLE_SLAB(ResinBrickSlab)
			default: return Air::Air();
		}
	}
};
