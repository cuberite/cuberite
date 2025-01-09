
// BlockSlab.h

// Declares cBlockSlabHandler and cBlockDoubleSlabHandler classes





#pragma once

#include "Blocks/BlockHandler.h"
#include "Blocks/ChunkInterface.h"
#include "BlockInfo.h"
#include "Entities/Player.h"
#include "Registries/BlockItemConverter.h"


class cBlockSlabHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	/** Returns true if the specified BlockType is one of the slabs handled by this handler */
	static bool IsAnySlabType(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSlab:
			case BlockType::AndesiteSlab:
			case BlockType::BirchSlab:
			case BlockType::BlackstoneSlab:
			case BlockType::BrickSlab:
			case BlockType::CobblestoneSlab:
			case BlockType::CrimsonSlab:
			case BlockType::CutRedSandstoneSlab:
			case BlockType::CutSandstoneSlab:
			case BlockType::DarkOakSlab:
			case BlockType::DarkPrismarineSlab:
			case BlockType::DioriteSlab:
			case BlockType::EndStoneBrickSlab:
			case BlockType::GraniteSlab:
			case BlockType::JungleSlab:
			case BlockType::MossyCobblestoneSlab:
			case BlockType::MossyStoneBrickSlab:
			case BlockType::NetherBrickSlab:
			case BlockType::OakSlab:
			case BlockType::PetrifiedOakSlab:
			case BlockType::PolishedAndesiteSlab:
			case BlockType::PolishedBlackstoneBrickSlab:
			case BlockType::PolishedBlackstoneSlab:
			case BlockType::PolishedDioriteSlab:
			case BlockType::PolishedGraniteSlab:
			case BlockType::PrismarineBrickSlab:
			case BlockType::PrismarineSlab:
			case BlockType::PurpurSlab:
			case BlockType::QuartzSlab:
			case BlockType::RedNetherBrickSlab:
			case BlockType::RedSandstoneSlab:
			case BlockType::SandstoneSlab:
			case BlockType::SmoothQuartzSlab:
			case BlockType::SmoothRedSandstoneSlab:
			case BlockType::SmoothSandstoneSlab:
			case BlockType::SmoothStoneSlab:
			case BlockType::SpruceSlab:
			case BlockType::StoneBrickSlab:
			case BlockType::StoneSlab:
			case BlockType::WarpedSlab:
			case BlockType::BambooMosaicSlab:
			case BlockType::BambooSlab:
			case BlockType::CherrySlab:
			case BlockType::CobbledDeepslateSlab:
			case BlockType::TuffSlab:
			case BlockType::TuffBrickSlab:
			case BlockType::PolishedTuffSlab:
			case BlockType::MudBrickSlab:
			case BlockType::MangroveSlab:
			case BlockType::DeepslateTileSlab:
			case BlockType::DeepslateBrickSlab:
			case BlockType::PolishedDeepslateSlab:
			case BlockType::CutCopperSlab:
			case BlockType::ExposedCutCopperSlab:
			case BlockType::WeatheredCutCopperSlab:
			case BlockType::OxidizedCutCopperSlab:
			case BlockType::WaxedCutCopperSlab:
			case BlockType::WaxedExposedCutCopperSlab:
			case BlockType::WaxedWeatheredCutCopperSlab:
			case BlockType::WaxedOxidizedCutCopperSlab:
			case BlockType::PaleOakSlab:
			case BlockType::ResinBrickSlab:
				return true;
			default: return false;
		}
	}

#define ISSLABTOP(SlabType) \
	return (SlabType::Type(a_Block) == SlabType::Type::Top);

	static bool IsSlabTop(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSlab:
			{
				ISSLABTOP(AcaciaSlab)
			}
			case BlockType::AndesiteSlab:
			{
				ISSLABTOP(AndesiteSlab)
			}
			case BlockType::BirchSlab:
			{
				ISSLABTOP(BirchSlab)
			}
			case BlockType::BlackstoneSlab:
			{
				ISSLABTOP(BlackstoneSlab)
			}
			case BlockType::BrickSlab:
			{
				ISSLABTOP(BrickSlab)
			}
			case BlockType::CobblestoneSlab:
			{
				ISSLABTOP(CobblestoneSlab)
			}
			case BlockType::CrimsonSlab:
			{
				ISSLABTOP(CrimsonSlab)
			}
			case BlockType::CutRedSandstoneSlab:
			{
				ISSLABTOP(CutRedSandstoneSlab)
			}
			case BlockType::CutSandstoneSlab:
			{
				ISSLABTOP(CutSandstoneSlab)
			}
			case BlockType::DarkOakSlab:
			{
				ISSLABTOP(DarkOakSlab)
			}
			case BlockType::DarkPrismarineSlab:
			{
				ISSLABTOP(DarkPrismarineSlab)
			}
			case BlockType::DioriteSlab:
			{
				ISSLABTOP(DioriteSlab)
			}
			case BlockType::EndStoneBrickSlab:
			{
				ISSLABTOP(EndStoneBrickSlab)
			}
			case BlockType::GraniteSlab:
			{
				ISSLABTOP(GraniteSlab)
			}
			case BlockType::JungleSlab:
			{
				ISSLABTOP(JungleSlab)
			}
			case BlockType::MossyCobblestoneSlab:
			{
				ISSLABTOP(MossyCobblestoneSlab)
			}
			case BlockType::MossyStoneBrickSlab:
			{
				ISSLABTOP(MossyStoneBrickSlab)
			}
			case BlockType::NetherBrickSlab:
			{
				ISSLABTOP(NetherBrickSlab)
			}
			case BlockType::OakSlab:
			{
				ISSLABTOP(OakSlab)
			}
			case BlockType::PetrifiedOakSlab:
			{
				ISSLABTOP(PetrifiedOakSlab)
			}
			case BlockType::PolishedAndesiteSlab:
			{
				ISSLABTOP(PolishedAndesiteSlab)
			}
			case BlockType::PolishedBlackstoneBrickSlab:
			{
				ISSLABTOP(PolishedBlackstoneBrickSlab)
			}
			case BlockType::PolishedBlackstoneSlab:
			{
				ISSLABTOP(PolishedBlackstoneSlab)
			}
			case BlockType::PolishedDioriteSlab:
			{
				ISSLABTOP(PolishedDioriteSlab)
			}
			case BlockType::PolishedGraniteSlab:
			{
				ISSLABTOP(PolishedGraniteSlab)
			}
			case BlockType::PrismarineBrickSlab:
			{
				ISSLABTOP(PrismarineBrickSlab)
			}
			case BlockType::PrismarineSlab:
			{
				ISSLABTOP(PrismarineSlab)
			}
			case BlockType::PurpurSlab:
			{
				ISSLABTOP(PurpurSlab)
			}
			case BlockType::QuartzSlab:
			{
				ISSLABTOP(QuartzSlab)
			}
			case BlockType::RedNetherBrickSlab:
			{
				ISSLABTOP(RedNetherBrickSlab)
			}
			case BlockType::RedSandstoneSlab:
			{
				ISSLABTOP(RedSandstoneSlab)
			}
			case BlockType::SandstoneSlab:
			{
				ISSLABTOP(SandstoneSlab)
			}
			case BlockType::SmoothQuartzSlab:
			{
				ISSLABTOP(SmoothQuartzSlab)
			}
			case BlockType::SmoothRedSandstoneSlab:
			{
				ISSLABTOP(SmoothRedSandstoneSlab)
			}
			case BlockType::SmoothSandstoneSlab:
			{
				ISSLABTOP(SmoothSandstoneSlab)
			}
			case BlockType::SmoothStoneSlab:
			{
				ISSLABTOP(SmoothStoneSlab)
			}
			case BlockType::SpruceSlab:
			{
				ISSLABTOP(SpruceSlab)
			}
			case BlockType::StoneBrickSlab:
			{
				ISSLABTOP(StoneBrickSlab)
			}
			case BlockType::StoneSlab:
			{
				ISSLABTOP(StoneSlab)
			}
			case BlockType::WarpedSlab:
			{
				ISSLABTOP(WarpedSlab)
			}
			case BlockType::BambooMosaicSlab:            ISSLABTOP(BambooMosaicSlab)
			case BlockType::BambooSlab:                  ISSLABTOP(BambooSlab)
			case BlockType::CherrySlab:                  ISSLABTOP(CherrySlab)
			case BlockType::CobbledDeepslateSlab:        ISSLABTOP(CobbledDeepslateSlab)
			case BlockType::TuffSlab:                    ISSLABTOP(TuffSlab)
			case BlockType::TuffBrickSlab:               ISSLABTOP(TuffBrickSlab)
			case BlockType::PolishedTuffSlab:            ISSLABTOP(PolishedTuffSlab)
			case BlockType::MudBrickSlab:                ISSLABTOP(MudBrickSlab)
			case BlockType::MangroveSlab:                ISSLABTOP(MangroveSlab)
			case BlockType::DeepslateTileSlab:           ISSLABTOP(DeepslateTileSlab)
			case BlockType::DeepslateBrickSlab:          ISSLABTOP(DeepslateBrickSlab)
			case BlockType::PolishedDeepslateSlab:       ISSLABTOP(PolishedDeepslateSlab)
			case BlockType::CutCopperSlab:               ISSLABTOP(CutCopperSlab)
			case BlockType::ExposedCutCopperSlab:        ISSLABTOP(ExposedCutCopperSlab)
			case BlockType::WeatheredCutCopperSlab:      ISSLABTOP(WeatheredCutCopperSlab)
			case BlockType::OxidizedCutCopperSlab:       ISSLABTOP(OxidizedCutCopperSlab)
			case BlockType::WaxedCutCopperSlab:          ISSLABTOP(WaxedCutCopperSlab)
			case BlockType::WaxedExposedCutCopperSlab:        ISSLABTOP(WaxedExposedCutCopperSlab)
			case BlockType::WaxedWeatheredCutCopperSlab:      ISSLABTOP(WaxedWeatheredCutCopperSlab)
			case BlockType::WaxedOxidizedCutCopperSlab:       ISSLABTOP(WaxedOxidizedCutCopperSlab)
			case BlockType::PaleOakSlab:                 ISSLABTOP(PaleOakSlab)
			case BlockType::ResinBrickSlab:              ISSLABTOP(ResinBrickSlab)
			default: return false;
		}
	}

#define ISSLABFULL(SlabType) \
	return (SlabType::Type(a_Block) == SlabType::Type::Double);

	static bool IsSlabFull(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSlab:
			{
				ISSLABFULL(AcaciaSlab)
			}
			case BlockType::AndesiteSlab:
			{
				ISSLABFULL(AndesiteSlab)
			}
			case BlockType::BirchSlab:
			{
				ISSLABFULL(BirchSlab)
			}
			case BlockType::BlackstoneSlab:
			{
				ISSLABFULL(BlackstoneSlab)
			}
			case BlockType::BrickSlab:
			{
				ISSLABFULL(BrickSlab)
			}
			case BlockType::CobblestoneSlab:
			{
				ISSLABFULL(CobblestoneSlab)
			}
			case BlockType::CrimsonSlab:
			{
				ISSLABFULL(CrimsonSlab)
			}
			case BlockType::CutRedSandstoneSlab:
			{
				ISSLABFULL(CutRedSandstoneSlab)
			}
			case BlockType::CutSandstoneSlab:
			{
				ISSLABFULL(CutSandstoneSlab)
			}
			case BlockType::DarkOakSlab:
			{
				ISSLABFULL(DarkOakSlab)
			}
			case BlockType::DarkPrismarineSlab:
			{
				ISSLABFULL(DarkPrismarineSlab)
			}
			case BlockType::DioriteSlab:
			{
				ISSLABFULL(DioriteSlab)
			}
			case BlockType::EndStoneBrickSlab:
			{
				ISSLABFULL(EndStoneBrickSlab)
			}
			case BlockType::GraniteSlab:
			{
				ISSLABFULL(GraniteSlab)
			}
			case BlockType::JungleSlab:
			{
				ISSLABFULL(JungleSlab)
			}
			case BlockType::MossyCobblestoneSlab:
			{
				ISSLABFULL(MossyCobblestoneSlab)
			}
			case BlockType::MossyStoneBrickSlab:
			{
				ISSLABFULL(MossyStoneBrickSlab)
			}
			case BlockType::NetherBrickSlab:
			{
				ISSLABFULL(NetherBrickSlab)
			}
			case BlockType::OakSlab:
			{
				ISSLABFULL(OakSlab)
			}
			case BlockType::PetrifiedOakSlab:
			{
				ISSLABFULL(PetrifiedOakSlab)
			}
			case BlockType::PolishedAndesiteSlab:
			{
				ISSLABFULL(PolishedAndesiteSlab)
			}
			case BlockType::PolishedBlackstoneBrickSlab:
			{
				ISSLABFULL(PolishedBlackstoneBrickSlab)
			}
			case BlockType::PolishedBlackstoneSlab:
			{
				ISSLABFULL(PolishedBlackstoneSlab)
			}
			case BlockType::PolishedDioriteSlab:
			{
				ISSLABFULL(PolishedDioriteSlab)
			}
			case BlockType::PolishedGraniteSlab:
			{
				ISSLABFULL(PolishedGraniteSlab)
			}
			case BlockType::PrismarineBrickSlab:
			{
				ISSLABFULL(PrismarineBrickSlab)
			}
			case BlockType::PrismarineSlab:
			{
				ISSLABFULL(PrismarineSlab)
			}
			case BlockType::PurpurSlab:
			{
				ISSLABFULL(PurpurSlab)
			}
			case BlockType::QuartzSlab:
			{
				ISSLABFULL(QuartzSlab)
			}
			case BlockType::RedNetherBrickSlab:
			{
				ISSLABFULL(RedNetherBrickSlab)
			}
			case BlockType::RedSandstoneSlab:
			{
				ISSLABFULL(RedSandstoneSlab)
			}
			case BlockType::SandstoneSlab:
			{
				ISSLABFULL(SandstoneSlab)
			}
			case BlockType::SmoothQuartzSlab:
			{
				ISSLABFULL(SmoothQuartzSlab)
			}
			case BlockType::SmoothRedSandstoneSlab:
			{
				ISSLABFULL(SmoothRedSandstoneSlab)
			}
			case BlockType::SmoothSandstoneSlab:
			{
				ISSLABFULL(SmoothSandstoneSlab)
			}
			case BlockType::SmoothStoneSlab:
			{
				ISSLABFULL(SmoothStoneSlab)
			}
			case BlockType::SpruceSlab:
			{
				ISSLABFULL(SpruceSlab)
			}
			case BlockType::StoneBrickSlab:
			{
				ISSLABFULL(StoneBrickSlab)
			}
			case BlockType::StoneSlab:
			{
				ISSLABFULL(StoneSlab)
			}
			case BlockType::WarpedSlab:
			{
				ISSLABFULL(WarpedSlab)
			}
			case BlockType::BambooMosaicSlab:            ISSLABFULL(BambooMosaicSlab)
			case BlockType::BambooSlab:                  ISSLABFULL(BambooSlab)
			case BlockType::CherrySlab:                  ISSLABFULL(CherrySlab)
			case BlockType::CobbledDeepslateSlab:        ISSLABFULL(CobbledDeepslateSlab)
			case BlockType::TuffSlab:                    ISSLABFULL(TuffSlab)
			case BlockType::TuffBrickSlab:               ISSLABFULL(TuffBrickSlab)
			case BlockType::PolishedTuffSlab:            ISSLABFULL(PolishedTuffSlab)
			case BlockType::MudBrickSlab:                ISSLABFULL(MudBrickSlab)
			case BlockType::MangroveSlab:                ISSLABFULL(MangroveSlab)
			case BlockType::DeepslateTileSlab:           ISSLABFULL(DeepslateTileSlab)
			case BlockType::DeepslateBrickSlab:          ISSLABFULL(DeepslateBrickSlab)
			case BlockType::PolishedDeepslateSlab:       ISSLABFULL(PolishedDeepslateSlab)
			case BlockType::CutCopperSlab:               ISSLABFULL(CutCopperSlab)
			case BlockType::ExposedCutCopperSlab:        ISSLABFULL(ExposedCutCopperSlab)
			case BlockType::WeatheredCutCopperSlab:      ISSLABFULL(WeatheredCutCopperSlab)
			case BlockType::OxidizedCutCopperSlab:       ISSLABFULL(OxidizedCutCopperSlab)
			case BlockType::WaxedCutCopperSlab:          ISSLABFULL(WaxedCutCopperSlab)
			case BlockType::WaxedExposedCutCopperSlab:        ISSLABFULL(WaxedExposedCutCopperSlab)
			case BlockType::WaxedWeatheredCutCopperSlab:      ISSLABFULL(WaxedWeatheredCutCopperSlab)
			case BlockType::WaxedOxidizedCutCopperSlab:       ISSLABFULL(WaxedOxidizedCutCopperSlab)
			case BlockType::PaleOakSlab:                 ISSLABFULL(PaleOakSlab)
			case BlockType::ResinBrickSlab:              ISSLABFULL(ResinBrickSlab)
			default: return false;
		}
	}

	static bool IsSlabDown(BlockState a_Block) { return !IsSlabTop(a_Block); }

	static bool IsSlabUpsideDown(BlockState a_Block) { UNREACHABLE("unimplemented"); }

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaSlab:                   return cItem(Item::AcaciaSlab);
			case BlockType::AndesiteSlab:                 return cItem(Item::AndesiteSlab);
			case BlockType::BirchSlab:                    return cItem(Item::BirchSlab);
			case BlockType::BlackstoneSlab:               return cItem(Item::BlackstoneSlab);
			case BlockType::BrickSlab:                    return cItem(Item::BrickSlab);
			case BlockType::CobblestoneSlab:              return cItem(Item::CobblestoneSlab);
			case BlockType::CrimsonSlab:                  return cItem(Item::CrimsonSlab);
			case BlockType::CutRedSandstoneSlab:          return cItem(Item::CutRedSandstoneSlab);
			case BlockType::CutSandstoneSlab:             return cItem(Item::CutSandstoneSlab);
			case BlockType::DarkOakSlab:                  return cItem(Item::DarkOakSlab);
			case BlockType::DarkPrismarineSlab:           return cItem(Item::DarkPrismarineSlab);
			case BlockType::DioriteSlab:                  return cItem(Item::DioriteSlab);
			case BlockType::EndStoneBrickSlab:            return cItem(Item::EndStoneBrickSlab);
			case BlockType::GraniteSlab:                  return cItem(Item::GraniteSlab);
			case BlockType::JungleSlab:                   return cItem(Item::JungleSlab);
			case BlockType::MossyCobblestoneSlab:         return cItem(Item::MossyCobblestoneSlab);
			case BlockType::MossyStoneBrickSlab:          return cItem(Item::MossyStoneBrickSlab);
			case BlockType::NetherBrickSlab:              return cItem(Item::NetherBrickSlab);
			case BlockType::OakSlab:                      return cItem(Item::OakSlab);
			case BlockType::PetrifiedOakSlab:             return cItem(Item::PetrifiedOakSlab);
			case BlockType::PolishedAndesiteSlab:         return cItem(Item::PolishedAndesiteSlab);
			case BlockType::PolishedBlackstoneBrickSlab:  return cItem(Item::PolishedBlackstoneBrickSlab);
			case BlockType::PolishedBlackstoneSlab:       return cItem(Item::PolishedBlackstoneSlab);
			case BlockType::PolishedDioriteSlab:          return cItem(Item::PolishedDioriteSlab);
			case BlockType::PolishedGraniteSlab:          return cItem(Item::PolishedGraniteSlab);
			case BlockType::PrismarineBrickSlab:          return cItem(Item::PrismarineBrickSlab);
			case BlockType::PrismarineSlab:               return cItem(Item::PrismarineSlab);
			case BlockType::PurpurSlab:                   return cItem(Item::PurpurSlab);
			case BlockType::QuartzSlab:                   return cItem(Item::QuartzSlab);
			case BlockType::RedNetherBrickSlab:           return cItem(Item::RedNetherBrickSlab);
			case BlockType::RedSandstoneSlab:             return cItem(Item::RedSandstoneSlab);
			case BlockType::SandstoneSlab:                return cItem(Item::SandstoneSlab);
			case BlockType::SmoothQuartzSlab:             return cItem(Item::SmoothQuartzSlab);
			case BlockType::SmoothRedSandstoneSlab:       return cItem(Item::SmoothRedSandstoneSlab);
			case BlockType::SmoothSandstoneSlab:          return cItem(Item::SmoothSandstoneSlab);
			case BlockType::SmoothStoneSlab:              return cItem(Item::SmoothStoneSlab);
			case BlockType::SpruceSlab:                   return cItem(Item::SpruceSlab);
			case BlockType::StoneBrickSlab:               return cItem(Item::StoneBrickSlab);
			case BlockType::StoneSlab:                    return cItem(Item::StoneSlab);
			case BlockType::WarpedSlab:                   return cItem(Item::WarpedSlab);
			case BlockType::BambooMosaicSlab:             return cItem(Item::BambooMosaicSlab);
			case BlockType::BambooSlab:                   return cItem(Item::BambooSlab);
			case BlockType::CherrySlab:                   return cItem(Item::CherrySlab);
			case BlockType::CobbledDeepslateSlab:         return cItem(Item::CobbledDeepslateSlab);
			case BlockType::TuffSlab:                     return cItem(Item::TuffSlab);
			case BlockType::TuffBrickSlab:                return cItem(Item::TuffBrickSlab);
			case BlockType::PolishedTuffSlab:             return cItem(Item::PolishedTuffSlab);
			case BlockType::MudBrickSlab:                 return cItem(Item::MudBrickSlab);
			case BlockType::MangroveSlab:                 return cItem(Item::MangroveSlab);
			case BlockType::DeepslateTileSlab:            return cItem(Item::DeepslateTileSlab);
			case BlockType::DeepslateBrickSlab:           return cItem(Item::DeepslateBrickSlab);
			case BlockType::PolishedDeepslateSlab:        return cItem(Item::PolishedDeepslateSlab);
			case BlockType::CutCopperSlab:                return cItem(Item::CutCopperSlab);
			case BlockType::ExposedCutCopperSlab:         return cItem(Item::ExposedCutCopperSlab);
			case BlockType::WeatheredCutCopperSlab:       return cItem(Item::WeatheredCutCopperSlab);
			case BlockType::OxidizedCutCopperSlab:        return cItem(Item::OxidizedCutCopperSlab);
			case BlockType::WaxedCutCopperSlab:           return cItem(Item::WaxedCutCopperSlab);
			case BlockType::WaxedExposedCutCopperSlab:    return cItem(Item::WaxedExposedCutCopperSlab);
			case BlockType::WaxedWeatheredCutCopperSlab:  return cItem(Item::WaxedWeatheredCutCopperSlab);
			case BlockType::WaxedOxidizedCutCopperSlab:   return cItem(Item::WaxedOxidizedCutCopperSlab);
			case BlockType::PaleOakSlab:                  return cItem(Item::PaleOakSlab);
			case BlockType::ResinBrickSlab:               return cItem(Item::ResinBrickSlab);
			default: return cItem();
		}
	}

	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, BlockState a_ClickedBlock, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const override
	{
		/* Double slab combining uses build collision checks to replace single slabs with double slabs in the right conditions.
		For us to be replaced, the player must be:
		1. Placing the same slab material.
		2. and existing slab is single. */
		if ((m_BlockType !=  BlockItemConverter::FromItem(a_HeldItem.m_ItemType)) || IsSlabFull(a_ClickedBlock))
		{
			return false;
		}

		const bool IsTopSlab = IsSlabTop(a_ClickedBlock);
		const auto CanClickCombine = ((a_ClickedBlockFace == BLOCK_FACE_TOP) && !IsTopSlab) || ((a_ClickedBlockFace == BLOCK_FACE_BOTTOM) && IsTopSlab);

		/* When the player clicks on us directly, we'll combine if we're
		a bottom slab and he clicked the top, or vice versa. Clicking on the sides will not combine.
		However, when indirectly clicked (on the side of another block, that caused placement to go to us)
		the conditions are exactly the opposite. */
		return a_ClickedDirectly ? CanClickCombine : !CanClickCombine;
	}


	virtual void OnCancelRightClick(
			cChunkInterface & a_ChunkInterface,
			cWorldInterface & a_WorldInterface,
			cPlayer & a_Player,
			const Vector3i a_BlockPos,
			eBlockFace a_BlockFace
	) const override
	{
		if ((a_BlockFace == BLOCK_FACE_NONE) || (BlockItemConverter::FromItem(a_Player.GetEquippedItem().m_ItemType) != m_BlockType))
		{
			return;
		}

		// Sends the slab back to the client. It's to refuse a doubleslab placement. */
		a_Player.GetWorld()->SendBlockTo(a_BlockPos, a_Player);
	}


#define METAMIRRORXZ(SlabType) \
{ \
	if (IsSlabTop(a_Block)) \
	{ \
		return SlabType::SlabType(SlabType::Type::Bottom, SlabType::Waterlogged(a_Block)); \
	} \
	else \
	{ \
		return SlabType::SlabType(SlabType::Type::Top, SlabType::Waterlogged(a_Block)); \
	}\
}
	// Flips top and bottom
	virtual BlockState MirrorXZ(BlockState a_Block) const override
	{
		using namespace Block;

		if (cBlockSlabHandler::IsSlabFull(a_Block))
		{
			return a_Block;
		}

		switch (a_Block.Type())
		{
			case BlockType::AcaciaSlab:                  METAMIRRORXZ(AcaciaSlab)
			case BlockType::AndesiteSlab:                METAMIRRORXZ(AndesiteSlab)
			case BlockType::BirchSlab:                   METAMIRRORXZ(BirchSlab)
			case BlockType::BlackstoneSlab:              METAMIRRORXZ(BlackstoneSlab)
			case BlockType::BrickSlab:                   METAMIRRORXZ(BrickSlab)
			case BlockType::CobblestoneSlab:             METAMIRRORXZ(CobblestoneSlab)
			case BlockType::CrimsonSlab:                 METAMIRRORXZ(CrimsonSlab)
			case BlockType::CutRedSandstoneSlab:         METAMIRRORXZ(CutRedSandstoneSlab)
			case BlockType::CutSandstoneSlab:            METAMIRRORXZ(CutSandstoneSlab)
			case BlockType::DarkOakSlab:                 METAMIRRORXZ(DarkOakSlab)
			case BlockType::DarkPrismarineSlab:          METAMIRRORXZ(DarkPrismarineSlab)
			case BlockType::DioriteSlab:                 METAMIRRORXZ(DioriteSlab)
			case BlockType::EndStoneBrickSlab:           METAMIRRORXZ(EndStoneBrickSlab)
			case BlockType::GraniteSlab:                 METAMIRRORXZ(GraniteSlab)
			case BlockType::JungleSlab:                  METAMIRRORXZ(JungleSlab)
			case BlockType::MossyCobblestoneSlab:        METAMIRRORXZ(MossyCobblestoneSlab)
			case BlockType::MossyStoneBrickSlab:         METAMIRRORXZ(MossyStoneBrickSlab)
			case BlockType::NetherBrickSlab:             METAMIRRORXZ(NetherBrickSlab)
			case BlockType::OakSlab:                     METAMIRRORXZ(OakSlab)
			case BlockType::PetrifiedOakSlab:            METAMIRRORXZ(PetrifiedOakSlab)
			case BlockType::PolishedAndesiteSlab:        METAMIRRORXZ(PolishedAndesiteSlab)
			case BlockType::PolishedBlackstoneBrickSlab: METAMIRRORXZ(PolishedBlackstoneBrickSlab)
			case BlockType::PolishedBlackstoneSlab:      METAMIRRORXZ(PolishedBlackstoneSlab)
			case BlockType::PolishedDioriteSlab:         METAMIRRORXZ(PolishedDioriteSlab)
			case BlockType::PolishedGraniteSlab:         METAMIRRORXZ(PolishedGraniteSlab)
			case BlockType::PrismarineBrickSlab:         METAMIRRORXZ(PrismarineBrickSlab)
			case BlockType::PrismarineSlab:              METAMIRRORXZ(PrismarineSlab)
			case BlockType::PurpurSlab:                  METAMIRRORXZ(PurpurSlab)
			case BlockType::QuartzSlab:                  METAMIRRORXZ(QuartzSlab)
			case BlockType::RedNetherBrickSlab:          METAMIRRORXZ(RedNetherBrickSlab)
			case BlockType::RedSandstoneSlab:            METAMIRRORXZ(RedSandstoneSlab)
			case BlockType::SandstoneSlab:               METAMIRRORXZ(SandstoneSlab)
			case BlockType::SmoothQuartzSlab:            METAMIRRORXZ(SmoothQuartzSlab)
			case BlockType::SmoothRedSandstoneSlab:      METAMIRRORXZ(SmoothRedSandstoneSlab)
			case BlockType::SmoothSandstoneSlab:         METAMIRRORXZ(SmoothSandstoneSlab)
			case BlockType::SmoothStoneSlab:             METAMIRRORXZ(SmoothStoneSlab)
			case BlockType::SpruceSlab:                  METAMIRRORXZ(SpruceSlab)
			case BlockType::StoneBrickSlab:              METAMIRRORXZ(StoneBrickSlab)
			case BlockType::StoneSlab:                   METAMIRRORXZ(StoneSlab)
			case BlockType::WarpedSlab:                  METAMIRRORXZ(WarpedSlab)
			case BlockType::BambooMosaicSlab:            METAMIRRORXZ(BambooMosaicSlab)
			case BlockType::BambooSlab:                  METAMIRRORXZ(BambooSlab)
			case BlockType::CherrySlab:                  METAMIRRORXZ(CherrySlab)
			case BlockType::CobbledDeepslateSlab:        METAMIRRORXZ(CobbledDeepslateSlab)
			case BlockType::TuffSlab:                    METAMIRRORXZ(TuffSlab)
			case BlockType::TuffBrickSlab:               METAMIRRORXZ(TuffBrickSlab)
			case BlockType::PolishedTuffSlab:            METAMIRRORXZ(PolishedTuffSlab)
			case BlockType::MudBrickSlab:                METAMIRRORXZ(MudBrickSlab)
			case BlockType::MangroveSlab:                METAMIRRORXZ(MangroveSlab)
			case BlockType::DeepslateTileSlab:           METAMIRRORXZ(DeepslateTileSlab)
			case BlockType::DeepslateBrickSlab:          METAMIRRORXZ(DeepslateBrickSlab)
			case BlockType::PolishedDeepslateSlab:       METAMIRRORXZ(PolishedDeepslateSlab)
			case BlockType::CutCopperSlab:               METAMIRRORXZ(CutCopperSlab)
			case BlockType::ExposedCutCopperSlab:        METAMIRRORXZ(ExposedCutCopperSlab)
			case BlockType::WeatheredCutCopperSlab:      METAMIRRORXZ(WeatheredCutCopperSlab)
			case BlockType::OxidizedCutCopperSlab:       METAMIRRORXZ(OxidizedCutCopperSlab)
			case BlockType::WaxedCutCopperSlab:          METAMIRRORXZ(WaxedCutCopperSlab)
			case BlockType::WaxedExposedCutCopperSlab:        METAMIRRORXZ(WaxedExposedCutCopperSlab)
			case BlockType::WaxedWeatheredCutCopperSlab:      METAMIRRORXZ(WaxedWeatheredCutCopperSlab)
			case BlockType::WaxedOxidizedCutCopperSlab:       METAMIRRORXZ(WaxedOxidizedCutCopperSlab)
			case BlockType::PaleOakSlab:                 METAMIRRORXZ(PaleOakSlab)
			case BlockType::ResinBrickSlab:              METAMIRRORXZ(ResinBrickSlab)
			default: return a_Block;
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaSlab:                  return 15;
			case BlockType::AndesiteSlab:                return 0;  // TODO
			case BlockType::BirchSlab:                   return 2;
			case BlockType::BlackstoneSlab:              return 0;  // TODO
			case BlockType::BrickSlab:                   return 28;
			case BlockType::CobblestoneSlab:             return 11;
			case BlockType::CrimsonSlab:                 return 0;  // TODO
			case BlockType::CutRedSandstoneSlab:         return 10;
			case BlockType::CutSandstoneSlab:            return 2;
			case BlockType::DarkOakSlab:                 return 26;
			case BlockType::DarkPrismarineSlab:          return 0;  // TODO
			case BlockType::DioriteSlab:                 return 0;  // TODO
			case BlockType::EndStoneBrickSlab:           return 0;  // TODO
			case BlockType::GraniteSlab:                 return 0;  // TODO
			case BlockType::JungleSlab:                  return 10;
			case BlockType::MossyCobblestoneSlab:        return 0;  // TODO
			case BlockType::MossyStoneBrickSlab:         return 0;  // TODO
			case BlockType::NetherBrickSlab:             return 35;
			case BlockType::OakSlab:                     return 13;
			case BlockType::PetrifiedOakSlab:            return 13;
			case BlockType::PolishedAndesiteSlab:        return 0;  // TODO
			case BlockType::PolishedBlackstoneBrickSlab: return 0;  // TODO
			case BlockType::PolishedBlackstoneSlab:      return 0;  // TODO
			case BlockType::PolishedDioriteSlab:         return 0;  // TODO
			case BlockType::PolishedGraniteSlab:         return 0;  // TODO
			case BlockType::PrismarineBrickSlab:         return 0;  // TODO
			case BlockType::PrismarineSlab:              return 0;  // TODO
			case BlockType::PurpurSlab:                  return 16;
			case BlockType::QuartzSlab:                  return 8;
			case BlockType::RedNetherBrickSlab:          return 0;  // TODO
			case BlockType::RedSandstoneSlab:            return 10;
			case BlockType::SandstoneSlab:               return 2;
			case BlockType::SmoothQuartzSlab:            return 8;
			case BlockType::SmoothRedSandstoneSlab:      return 10;
			case BlockType::SmoothSandstoneSlab:         return 2;
			case BlockType::SmoothStoneSlab:             return 0;  // TODO
			case BlockType::SpruceSlab:                  return 34;
			case BlockType::StoneBrickSlab:              return 11;
			case BlockType::StoneSlab:                   return 11;
			case BlockType::WarpedSlab:                  return 0;  // TODO
			case BlockType::BambooMosaicSlab:
			case BlockType::BambooSlab:
			case BlockType::CherrySlab:
			case BlockType::CobbledDeepslateSlab:
			case BlockType::TuffSlab:
			case BlockType::TuffBrickSlab:
			case BlockType::PolishedTuffSlab:
			case BlockType::MudBrickSlab:
			case BlockType::MangroveSlab:
			case BlockType::DeepslateTileSlab:
			case BlockType::DeepslateBrickSlab:
			case BlockType::PolishedDeepslateSlab:
			case BlockType::CutCopperSlab:
			case BlockType::ExposedCutCopperSlab:
			case BlockType::WeatheredCutCopperSlab:
			case BlockType::OxidizedCutCopperSlab:
			case BlockType::WaxedCutCopperSlab:
			case BlockType::WaxedExposedCutCopperSlab:
			case BlockType::WaxedWeatheredCutCopperSlab:
			case BlockType::WaxedOxidizedCutCopperSlab:
			case BlockType::PaleOakSlab:
			case BlockType::ResinBrickSlab: return 0;  // TODO
			default:
			{
				ASSERT(!"Unhandled blocktype in slab handler!");
				return 0;
			}
		}
	}





	virtual bool IsInsideBlock(Vector3d a_Position, const BlockState a_Block) const override
	{
		if (IsSlabTop(a_Block))
		{
			return (a_Position.y >= 0.5);
		}
		else if (IsSlabFull(a_Block))
		{
			return true;
		}
		else
		{
			return (a_Position.y <= 0.5);
		}
	}

	virtual bool FullyOccupiesVoxel(const BlockState a_Block) const override
	{
		return IsSlabFull(a_Block);
	}
} ;
