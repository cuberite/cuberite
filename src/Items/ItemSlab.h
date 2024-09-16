
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

		using namespace Block;

		switch (m_ItemType)
		{
			case Item::AcaciaSlab:
			{
				BlockToPlace = AcaciaSlab::AcaciaSlab(IsTop ? AcaciaSlab::Type::Top : AcaciaSlab::Type::Bottom);
				break;
			}
			case Item::AndesiteSlab:
			{
				BlockToPlace = AndesiteSlab::AndesiteSlab(IsTop ? AndesiteSlab::Type::Top : AndesiteSlab::Type::Bottom);
				break;
			}
			case Item::BirchSlab:
			{
				BlockToPlace = BirchSlab::BirchSlab(IsTop ? BirchSlab::Type::Top : BirchSlab::Type::Bottom);
				break;
			}
			case Item::BlackstoneSlab:
			{
				BlockToPlace = BlackstoneSlab::BlackstoneSlab(IsTop ? BlackstoneSlab::Type::Top : BlackstoneSlab::Type::Bottom);
				break;
			}
			case Item::BrickSlab:
			{
				BlockToPlace = BrickSlab::BrickSlab(IsTop ? BrickSlab::Type::Top : BrickSlab::Type::Bottom);
				break;
			}
			case Item::CobblestoneSlab:
			{
				BlockToPlace = CobblestoneSlab::CobblestoneSlab(IsTop ? CobblestoneSlab::Type::Top : CobblestoneSlab::Type::Bottom);
				break;
			}
			case Item::CrimsonSlab:
			{
				BlockToPlace = CrimsonSlab::CrimsonSlab(IsTop ? CrimsonSlab::Type::Top : CrimsonSlab::Type::Bottom);
				break;
			}
			case Item::CutRedSandstoneSlab:
			{
				BlockToPlace = CutRedSandstoneSlab::CutRedSandstoneSlab(IsTop ? CutRedSandstoneSlab::Type::Top : CutRedSandstoneSlab::Type::Bottom);
				break;
			}
			case Item::CutSandstoneSlab:
			{
				BlockToPlace = CutSandstoneSlab::CutSandstoneSlab(IsTop ? CutSandstoneSlab::Type::Top : CutSandstoneSlab::Type::Bottom);
				break;
			}
			case Item::DarkOakSlab:
			{
				BlockToPlace = DarkOakSlab::DarkOakSlab(IsTop ? DarkOakSlab::Type::Top : DarkOakSlab::Type::Bottom);
				break;
			}
			case Item::DarkPrismarineSlab:
			{
				BlockToPlace = DarkPrismarineSlab::DarkPrismarineSlab(IsTop ? DarkPrismarineSlab::Type::Top : DarkPrismarineSlab::Type::Bottom);
				break;
			}
			case Item::DioriteSlab:
			{
				BlockToPlace = DioriteSlab::DioriteSlab(IsTop ? DioriteSlab::Type::Top : DioriteSlab::Type::Bottom);
				break;
			}
			case Item::EndStoneBrickSlab:
			{
				BlockToPlace = EndStoneBrickSlab::EndStoneBrickSlab(IsTop ? EndStoneBrickSlab::Type::Top : EndStoneBrickSlab::Type::Bottom);
				break;
			}
			case Item::GraniteSlab:
			{
				BlockToPlace = GraniteSlab::GraniteSlab(IsTop ? GraniteSlab::Type::Top : GraniteSlab::Type::Bottom);
				break;
			}
			case Item::JungleSlab:
			{
				BlockToPlace = JungleSlab::JungleSlab(IsTop ? JungleSlab::Type::Top : JungleSlab::Type::Bottom);
				break;
			}
			case Item::MossyCobblestoneSlab:
			{
				BlockToPlace = MossyCobblestoneSlab::MossyCobblestoneSlab(IsTop ? MossyCobblestoneSlab::Type::Top : MossyCobblestoneSlab::Type::Bottom);
				break;
			}
			case Item::MossyStoneBrickSlab:
			{
				BlockToPlace = MossyStoneBrickSlab::MossyStoneBrickSlab(IsTop ? MossyStoneBrickSlab::Type::Top : MossyStoneBrickSlab::Type::Bottom);
				break;
			}
			case Item::NetherBrickSlab:
			{
				BlockToPlace = NetherBrickSlab::NetherBrickSlab(IsTop ? NetherBrickSlab::Type::Top : NetherBrickSlab::Type::Bottom);
				break;
			}
			case Item::OakSlab:
			{
				BlockToPlace = OakSlab::OakSlab(IsTop ? OakSlab::Type::Top : OakSlab::Type::Bottom);
				break;
			}
			case Item::PetrifiedOakSlab:
			{
				BlockToPlace = PetrifiedOakSlab::PetrifiedOakSlab(IsTop ? PetrifiedOakSlab::Type::Top : PetrifiedOakSlab::Type::Bottom);
				break;
			}
			case Item::PolishedAndesiteSlab:
			{
				BlockToPlace = PolishedAndesiteSlab::PolishedAndesiteSlab(IsTop ? PolishedAndesiteSlab::Type::Top : PolishedAndesiteSlab::Type::Bottom);
				break;
			}
			case Item::PolishedBlackstoneBrickSlab:
			{
				BlockToPlace = PolishedBlackstoneBrickSlab::PolishedBlackstoneBrickSlab(IsTop ? PolishedBlackstoneBrickSlab::Type::Top : PolishedBlackstoneBrickSlab::Type::Bottom);
				break;
			}
			case Item::PolishedBlackstoneSlab:
			{
				BlockToPlace = PolishedBlackstoneSlab::PolishedBlackstoneSlab(IsTop ? PolishedBlackstoneSlab::Type::Top : PolishedBlackstoneSlab::Type::Bottom);
				break;
			}
			case Item::PolishedDioriteSlab:
			{
				BlockToPlace = PolishedDioriteSlab::PolishedDioriteSlab(IsTop ? PolishedDioriteSlab::Type::Top : PolishedDioriteSlab::Type::Bottom);
				break;
			}
			case Item::PolishedGraniteSlab:
			{
				BlockToPlace = PolishedGraniteSlab::PolishedGraniteSlab(IsTop ? PolishedGraniteSlab::Type::Top : PolishedGraniteSlab::Type::Bottom);
				break;
			}
			case Item::PrismarineBrickSlab:
			{
				BlockToPlace = PrismarineBrickSlab::PrismarineBrickSlab(IsTop ? PrismarineBrickSlab::Type::Top : PrismarineBrickSlab::Type::Bottom);
				break;
			}
			case Item::PrismarineSlab:
			{
				BlockToPlace = PrismarineSlab::PrismarineSlab(IsTop ? PrismarineSlab::Type::Top : PrismarineSlab::Type::Bottom);
				break;
			}
			case Item::PurpurSlab:
			{
				BlockToPlace = PurpurSlab::PurpurSlab(IsTop ? PurpurSlab::Type::Top : PurpurSlab::Type::Bottom);
				break;
			}
			case Item::QuartzSlab:
			{
				BlockToPlace = QuartzSlab::QuartzSlab(IsTop ? QuartzSlab::Type::Top : QuartzSlab::Type::Bottom);
				break;
			}
			case Item::RedNetherBrickSlab:
			{
				BlockToPlace = RedNetherBrickSlab::RedNetherBrickSlab(IsTop ? RedNetherBrickSlab::Type::Top : RedNetherBrickSlab::Type::Bottom);
				break;
			}
			case Item::RedSandstoneSlab:
			{
				BlockToPlace = RedSandstoneSlab::RedSandstoneSlab(IsTop ? RedSandstoneSlab::Type::Top : RedSandstoneSlab::Type::Bottom);
				break;
			}
			case Item::SandstoneSlab:
			{
				BlockToPlace = SandstoneSlab::SandstoneSlab(IsTop ? SandstoneSlab::Type::Top : SandstoneSlab::Type::Bottom);
				break;
			}
			case Item::SmoothQuartzSlab:
			{
				BlockToPlace = SmoothQuartzSlab::SmoothQuartzSlab(IsTop ? SmoothQuartzSlab::Type::Top : SmoothQuartzSlab::Type::Bottom);
				break;
			}
			case Item::SmoothRedSandstoneSlab:
			{
				BlockToPlace = SmoothRedSandstoneSlab::SmoothRedSandstoneSlab(IsTop ? SmoothRedSandstoneSlab::Type::Top : SmoothRedSandstoneSlab::Type::Bottom);
				break;
			}
			case Item::SmoothSandstoneSlab:
			{
				BlockToPlace = SmoothSandstoneSlab::SmoothSandstoneSlab(IsTop ? SmoothSandstoneSlab::Type::Top : SmoothSandstoneSlab::Type::Bottom);
				break;
			}
			case Item::SmoothStoneSlab:
			{
				BlockToPlace = SmoothStoneSlab::SmoothStoneSlab(IsTop ? SmoothStoneSlab::Type::Top : SmoothStoneSlab::Type::Bottom);
				break;
			}
			case Item::SpruceSlab:
			{
				BlockToPlace = SpruceSlab::SpruceSlab(IsTop ? SpruceSlab::Type::Top : SpruceSlab::Type::Bottom);
				break;
			}
			case Item::StoneBrickSlab:
			{
				BlockToPlace = StoneBrickSlab::StoneBrickSlab(IsTop ? StoneBrickSlab::Type::Top : StoneBrickSlab::Type::Bottom);
				break;
			}
			case Item::StoneSlab:
			{
				BlockToPlace = StoneSlab::StoneSlab(IsTop ? StoneSlab::Type::Top : StoneSlab::Type::Bottom);
				break;
			}
			case Item::WarpedSlab:
			{
				BlockToPlace = WarpedSlab::WarpedSlab(IsTop ? WarpedSlab::Type::Top : WarpedSlab::Type::Bottom);
				break;
			}
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





	static BlockState GetDoubleSlab(BlockState BlockToPlace)
	{
		using namespace Block;
		switch (BlockToPlace.Type())
		{
			case BlockType::AcaciaSlab:                  return AcaciaSlab::AcaciaSlab(AcaciaSlab::Type::Double);
			case BlockType::AndesiteSlab:                return AndesiteSlab::AndesiteSlab(AndesiteSlab::Type::Double);
			case BlockType::BirchSlab:                   return BirchSlab::BirchSlab(BirchSlab::Type::Double);
			case BlockType::BlackstoneSlab:              return BlackstoneSlab::BlackstoneSlab(BlackstoneSlab::Type::Double);
			case BlockType::BrickSlab:                   return BlackstoneSlab::BlackstoneSlab(BlackstoneSlab::Type::Double);
			case BlockType::CobblestoneSlab:             return CobblestoneSlab::CobblestoneSlab(CobblestoneSlab::Type::Double);
			case BlockType::CrimsonSlab:                 return CrimsonSlab::CrimsonSlab(CrimsonSlab::Type::Double);
			case BlockType::CutRedSandstoneSlab:         return CutRedSandstoneSlab::CutRedSandstoneSlab(CutRedSandstoneSlab::Type::Double);
			case BlockType::CutSandstoneSlab:            return CutSandstoneSlab::CutSandstoneSlab(CutSandstoneSlab::Type::Double);
			case BlockType::DarkOakSlab:                 return DarkOakSlab::DarkOakSlab(DarkOakSlab::Type::Double);
			case BlockType::DarkPrismarineSlab:          return DarkPrismarineSlab::DarkPrismarineSlab(DarkPrismarineSlab::Type::Double);
			case BlockType::DioriteSlab:                 return DioriteSlab::DioriteSlab(DioriteSlab::Type::Double);
			case BlockType::EndStoneBrickSlab:           return EndStoneBrickSlab::EndStoneBrickSlab(EndStoneBrickSlab::Type::Double);
			case BlockType::GraniteSlab:                 return GraniteSlab::GraniteSlab(GraniteSlab::Type::Double);
			case BlockType::JungleSlab:                  return JungleSlab::JungleSlab(JungleSlab::Type::Double);
			case BlockType::MossyCobblestoneSlab:        return MossyCobblestoneSlab::MossyCobblestoneSlab(MossyCobblestoneSlab::Type::Double);
			case BlockType::MossyStoneBrickSlab:         return MossyStoneBrickSlab::MossyStoneBrickSlab(MossyStoneBrickSlab::Type::Double);
			case BlockType::NetherBrickSlab:             return NetherBrickSlab::NetherBrickSlab(NetherBrickSlab::Type::Double);
			case BlockType::OakSlab:                     return OakSlab::OakSlab(OakSlab::Type::Double);
			case BlockType::PetrifiedOakSlab:            return PetrifiedOakSlab::PetrifiedOakSlab(PetrifiedOakSlab::Type::Double);
			case BlockType::PolishedAndesiteSlab:        return PolishedAndesiteSlab::PolishedAndesiteSlab(PolishedAndesiteSlab::Type::Double);
			case BlockType::PolishedBlackstoneBrickSlab: return PolishedBlackstoneBrickSlab::PolishedBlackstoneBrickSlab(PolishedBlackstoneBrickSlab::Type::Double);
			case BlockType::PolishedBlackstoneSlab:      return PolishedBlackstoneSlab::PolishedBlackstoneSlab(PolishedBlackstoneSlab::Type::Double);
			case BlockType::PolishedDioriteSlab:         return PolishedDioriteSlab::PolishedDioriteSlab(PolishedDioriteSlab::Type::Double);
			case BlockType::PolishedGraniteSlab:         return PolishedGraniteSlab::PolishedGraniteSlab(PolishedGraniteSlab::Type::Double);
			case BlockType::PrismarineBrickSlab:         return PrismarineBrickSlab::PrismarineBrickSlab(PrismarineBrickSlab::Type::Double);
			case BlockType::PrismarineSlab:              return PrismarineSlab::PrismarineSlab(PrismarineSlab::Type::Double);
			case BlockType::PurpurSlab:                  return PurpurSlab::PurpurSlab(PurpurSlab::Type::Double);
			case BlockType::QuartzSlab:                  return QuartzSlab::QuartzSlab(QuartzSlab::Type::Double);
			case BlockType::RedNetherBrickSlab:          return RedNetherBrickSlab::RedNetherBrickSlab(RedNetherBrickSlab::Type::Double);
			case BlockType::RedSandstoneSlab:            return RedSandstoneSlab::RedSandstoneSlab(RedSandstoneSlab::Type::Double);
			case BlockType::SandstoneSlab:               return SandstoneSlab::SandstoneSlab(SandstoneSlab::Type::Double);
			case BlockType::SmoothQuartzSlab:            return SmoothQuartzSlab::SmoothQuartzSlab(SmoothQuartzSlab::Type::Double);
			case BlockType::SmoothRedSandstoneSlab:      return SmoothRedSandstoneSlab::SmoothRedSandstoneSlab(SmoothRedSandstoneSlab::Type::Double);
			case BlockType::SmoothSandstoneSlab:         return SmoothSandstoneSlab::SmoothSandstoneSlab(SmoothSandstoneSlab::Type::Double);
			case BlockType::SmoothStoneSlab:             return SmoothStoneSlab::SmoothStoneSlab(SmoothStoneSlab::Type::Double);
			case BlockType::SpruceSlab:                  return SpruceSlab::SpruceSlab(SpruceSlab::Type::Double);
			case BlockType::StoneBrickSlab:              return StoneBrickSlab::StoneBrickSlab(StoneBrickSlab::Type::Double);
			case BlockType::StoneSlab:                   return StoneSlab::StoneSlab(StoneSlab::Type::Double);
			case BlockType::WarpedSlab:                  return WarpedSlab::WarpedSlab(WarpedSlab::Type::Double);
			default: return Air::Air();
		}
	}
};
