
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

	/** Returns true if the specified blocktype is one of the slabs handled by this handler */
	static bool IsAnySlabType(BLOCKTYPE a_BlockType)
	{
		return (
			(a_BlockType == E_BLOCK_WOODEN_SLAB) ||
			(a_BlockType == E_BLOCK_STONE_SLAB) ||
			(a_BlockType == E_BLOCK_RED_SANDSTONE_SLAB) ||
			(a_BlockType == E_BLOCK_PURPUR_SLAB)
		);
	}

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
				return true;
			default: return false;
		}
	}

/** Macro only for internal use. */
#define IS_SLAB_TOP(SlabType) (SlabType::Type(a_Block) == SlabType::Type::Top);

	static inline bool IsSlabTop(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSlab:                   return IS_SLAB_TOP(AcaciaSlab)
			case BlockType::AndesiteSlab:                 return IS_SLAB_TOP(AndesiteSlab)
			case BlockType::BirchSlab:                    return IS_SLAB_TOP(BirchSlab)
			case BlockType::BlackstoneSlab:               return IS_SLAB_TOP(BlackstoneSlab)
			case BlockType::BrickSlab:                    return IS_SLAB_TOP(BrickSlab)
			case BlockType::CobblestoneSlab:              return IS_SLAB_TOP(CobblestoneSlab)
			case BlockType::CrimsonSlab:                  return IS_SLAB_TOP(CrimsonSlab)
			case BlockType::CutRedSandstoneSlab:          return IS_SLAB_TOP(CutRedSandstoneSlab)
			case BlockType::CutSandstoneSlab:             return IS_SLAB_TOP(CutSandstoneSlab)
			case BlockType::DarkOakSlab:                  return IS_SLAB_TOP(DarkOakSlab)
			case BlockType::DarkPrismarineSlab:           return IS_SLAB_TOP(DarkPrismarineSlab)
			case BlockType::DioriteSlab:                  return IS_SLAB_TOP(DioriteSlab)
			case BlockType::EndStoneBrickSlab:            return IS_SLAB_TOP(EndStoneBrickSlab)
			case BlockType::GraniteSlab:                  return IS_SLAB_TOP(GraniteSlab)
			case BlockType::JungleSlab:                   return IS_SLAB_TOP(JungleSlab)
			case BlockType::MossyCobblestoneSlab:         return IS_SLAB_TOP(MossyCobblestoneSlab)
			case BlockType::MossyStoneBrickSlab:          return IS_SLAB_TOP(MossyStoneBrickSlab)
			case BlockType::NetherBrickSlab:              return IS_SLAB_TOP(NetherBrickSlab)
			case BlockType::OakSlab:                      return IS_SLAB_TOP(OakSlab)
			case BlockType::PetrifiedOakSlab:             return IS_SLAB_TOP(PetrifiedOakSlab)
			case BlockType::PolishedAndesiteSlab:         return IS_SLAB_TOP(PolishedAndesiteSlab)
			case BlockType::PolishedBlackstoneBrickSlab:  return IS_SLAB_TOP(PolishedBlackstoneBrickSlab)
			case BlockType::PolishedBlackstoneSlab:       return IS_SLAB_TOP(PolishedBlackstoneSlab)
			case BlockType::PolishedDioriteSlab:          return IS_SLAB_TOP(PolishedDioriteSlab)
			case BlockType::PolishedGraniteSlab:          return IS_SLAB_TOP(PolishedGraniteSlab)
			case BlockType::PrismarineBrickSlab:          return IS_SLAB_TOP(PrismarineBrickSlab)
			case BlockType::PrismarineSlab:               return IS_SLAB_TOP(PrismarineSlab)
			case BlockType::PurpurSlab:                   return IS_SLAB_TOP(PurpurSlab)
			case BlockType::QuartzSlab:                   return IS_SLAB_TOP(QuartzSlab)
			case BlockType::RedNetherBrickSlab:           return IS_SLAB_TOP(RedNetherBrickSlab)
			case BlockType::RedSandstoneSlab:             return IS_SLAB_TOP(RedSandstoneSlab)
			case BlockType::SandstoneSlab:                return IS_SLAB_TOP(SandstoneSlab)
			case BlockType::SmoothQuartzSlab:             return IS_SLAB_TOP(SmoothQuartzSlab)
			case BlockType::SmoothRedSandstoneSlab:       return IS_SLAB_TOP(SmoothRedSandstoneSlab)
			case BlockType::SmoothSandstoneSlab:          return IS_SLAB_TOP(SmoothSandstoneSlab)
			case BlockType::SmoothStoneSlab:              return IS_SLAB_TOP(SmoothStoneSlab)
			case BlockType::SpruceSlab:                   return IS_SLAB_TOP(SpruceSlab)
			case BlockType::StoneBrickSlab:               return IS_SLAB_TOP(StoneBrickSlab)
			case BlockType::StoneSlab:                    return IS_SLAB_TOP(StoneSlab)
			case BlockType::WarpedSlab:                   return IS_SLAB_TOP(WarpedSlab)
			default: return false;
		}
	}
#undef IS_SLAB_TOP

/** Macro only for internal use. */
#define IS_SLAB_FULL(SlabType) (SlabType::Type(a_Block) == SlabType::Type::Double);

	static inline bool IsSlabFull(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSlab:                   return IS_SLAB_FULL(AcaciaSlab)
			case BlockType::AndesiteSlab:                 return IS_SLAB_FULL(AndesiteSlab)
			case BlockType::BirchSlab:                    return IS_SLAB_FULL(BirchSlab)
			case BlockType::BlackstoneSlab:               return IS_SLAB_FULL(BlackstoneSlab)
			case BlockType::BrickSlab:                    return IS_SLAB_FULL(BrickSlab)
			case BlockType::CobblestoneSlab:              return IS_SLAB_FULL(CobblestoneSlab)
			case BlockType::CrimsonSlab:                  return IS_SLAB_FULL(CrimsonSlab)
			case BlockType::CutRedSandstoneSlab:          return IS_SLAB_FULL(CutRedSandstoneSlab)
			case BlockType::CutSandstoneSlab:             return IS_SLAB_FULL(CutSandstoneSlab)
			case BlockType::DarkOakSlab:                  return IS_SLAB_FULL(DarkOakSlab)
			case BlockType::DarkPrismarineSlab:           return IS_SLAB_FULL(DarkPrismarineSlab)
			case BlockType::DioriteSlab:                  return IS_SLAB_FULL(DioriteSlab)
			case BlockType::EndStoneBrickSlab:            return IS_SLAB_FULL(EndStoneBrickSlab)
			case BlockType::GraniteSlab:                  return IS_SLAB_FULL(GraniteSlab)
			case BlockType::JungleSlab:                   return IS_SLAB_FULL(JungleSlab)
			case BlockType::MossyCobblestoneSlab:         return IS_SLAB_FULL(MossyCobblestoneSlab)
			case BlockType::MossyStoneBrickSlab:          return IS_SLAB_FULL(MossyStoneBrickSlab)
			case BlockType::NetherBrickSlab:              return IS_SLAB_FULL(NetherBrickSlab)
			case BlockType::OakSlab:                      return IS_SLAB_FULL(OakSlab)
			case BlockType::PetrifiedOakSlab:             return IS_SLAB_FULL(PetrifiedOakSlab)
			case BlockType::PolishedAndesiteSlab:         return IS_SLAB_FULL(PolishedAndesiteSlab)
			case BlockType::PolishedBlackstoneBrickSlab:  return IS_SLAB_FULL(PolishedBlackstoneBrickSlab)
			case BlockType::PolishedBlackstoneSlab:       return IS_SLAB_FULL(PolishedBlackstoneSlab)
			case BlockType::PolishedDioriteSlab:          return IS_SLAB_FULL(PolishedDioriteSlab)
			case BlockType::PolishedGraniteSlab:          return IS_SLAB_FULL(PolishedGraniteSlab)
			case BlockType::PrismarineBrickSlab:          return IS_SLAB_FULL(PrismarineBrickSlab)
			case BlockType::PrismarineSlab:               return IS_SLAB_FULL(PrismarineSlab)
			case BlockType::PurpurSlab:                   return IS_SLAB_FULL(PurpurSlab)
			case BlockType::QuartzSlab:                   return IS_SLAB_FULL(QuartzSlab)
			case BlockType::RedNetherBrickSlab:           return IS_SLAB_FULL(RedNetherBrickSlab)
			case BlockType::RedSandstoneSlab:             return IS_SLAB_FULL(RedSandstoneSlab)
			case BlockType::SandstoneSlab:                return IS_SLAB_FULL(SandstoneSlab)
			case BlockType::SmoothQuartzSlab:             return IS_SLAB_FULL(SmoothQuartzSlab)
			case BlockType::SmoothRedSandstoneSlab:       return IS_SLAB_FULL(SmoothRedSandstoneSlab)
			case BlockType::SmoothSandstoneSlab:          return IS_SLAB_FULL(SmoothSandstoneSlab)
			case BlockType::SmoothStoneSlab:              return IS_SLAB_FULL(SmoothStoneSlab)
			case BlockType::SpruceSlab:                   return IS_SLAB_FULL(SpruceSlab)
			case BlockType::StoneBrickSlab:               return IS_SLAB_FULL(StoneBrickSlab)
			case BlockType::StoneSlab:                    return IS_SLAB_FULL(StoneSlab)
			case BlockType::WarpedSlab:                   return IS_SLAB_FULL(WarpedSlab)
			default: return false;
		}
	}
#undef IS_SLAB_FULL

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Reset the "top half" flag:
		return cItem(m_BlockType, 1, a_BlockMeta & 0x07);
	}


	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, const Vector3i a_Position, const NIBBLETYPE a_Meta, const eBlockFace a_ClickedBlockFace, const bool a_ClickedDirectly) const override
	{
		/* Double slab combining uses build collision checks to replace single slabs with double slabs in the right conditions.
		For us to be replaced, the player must be:
		1. Placing the same slab material.
		2. Placing the same slab sub-kind (and existing slab is single). */
		if ((m_BlockType != a_HeldItem.m_ItemType) || ((a_Meta & 0x07) != a_HeldItem.m_ItemDamage))
		{
			return false;
		}

		const bool IsTopSlab = (a_Meta & 0x08) == 0x08;
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
		if ((a_BlockFace == BLOCK_FACE_NONE) || (a_Player.GetEquippedItem().m_ItemType != static_cast<short>(m_BlockType)))
		{
			return;
		}

		// Sends the slab back to the client. It's to refuse a doubleslab placement. */
		a_Player.GetWorld()->SendBlockTo(a_BlockPos, a_Player);
	}





	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) const override
	{
		// Toggle the 4th bit - up / down:
		return (a_Meta ^ 0x08);
	}


/** Internal use only */
#define MIRROR_XZ(SlabType) \
{ \
	if (IsSlabTop(a_Block)) \
	{ \
		return SlabType::SlabType(SlabType::Type::Bottom); \
	} \
	else \
	{ \
		return SlabType::SlabType(SlabType::Type::Top); \
	}\
}

	// Flips top and bottom
	// TODO add override when BlockStates are used
	BlockState MirrorXZ(BlockState a_Block) const
	{
		using namespace Block;

		if (cBlockSlabHandler::IsSlabFull(a_Block))
		{
			return a_Block;
		}

		switch (a_Block.Type())
		{
			case BlockType::AcaciaSlab:                  MIRROR_XZ(AcaciaSlab)
			case BlockType::AndesiteSlab:                MIRROR_XZ(AndesiteSlab)
			case BlockType::BirchSlab:                   MIRROR_XZ(BirchSlab)
			case BlockType::BlackstoneSlab:              MIRROR_XZ(BlackstoneSlab)
			case BlockType::BrickSlab:                   MIRROR_XZ(BrickSlab)
			case BlockType::CobblestoneSlab:             MIRROR_XZ(CobblestoneSlab)
			case BlockType::CrimsonSlab:                 MIRROR_XZ(CrimsonSlab)
			case BlockType::CutRedSandstoneSlab:         MIRROR_XZ(CutRedSandstoneSlab)
			case BlockType::CutSandstoneSlab:            MIRROR_XZ(CutSandstoneSlab)
			case BlockType::DarkOakSlab:                 MIRROR_XZ(DarkOakSlab)
			case BlockType::DarkPrismarineSlab:          MIRROR_XZ(DarkPrismarineSlab)
			case BlockType::DioriteSlab:                 MIRROR_XZ(DioriteSlab)
			case BlockType::EndStoneBrickSlab:           MIRROR_XZ(EndStoneBrickSlab)
			case BlockType::GraniteSlab:                 MIRROR_XZ(GraniteSlab)
			case BlockType::JungleSlab:                  MIRROR_XZ(JungleSlab)
			case BlockType::MossyCobblestoneSlab:        MIRROR_XZ(MossyCobblestoneSlab)
			case BlockType::MossyStoneBrickSlab:         MIRROR_XZ(MossyStoneBrickSlab)
			case BlockType::NetherBrickSlab:             MIRROR_XZ(NetherBrickSlab)
			case BlockType::OakSlab:                     MIRROR_XZ(OakSlab)
			case BlockType::PetrifiedOakSlab:            MIRROR_XZ(PetrifiedOakSlab)
			case BlockType::PolishedAndesiteSlab:        MIRROR_XZ(PolishedAndesiteSlab)
			case BlockType::PolishedBlackstoneBrickSlab: MIRROR_XZ(PolishedBlackstoneBrickSlab)
			case BlockType::PolishedBlackstoneSlab:      MIRROR_XZ(PolishedBlackstoneSlab)
			case BlockType::PolishedDioriteSlab:         MIRROR_XZ(PolishedDioriteSlab)
			case BlockType::PolishedGraniteSlab:         MIRROR_XZ(PolishedGraniteSlab)
			case BlockType::PrismarineBrickSlab:         MIRROR_XZ(PrismarineBrickSlab)
			case BlockType::PrismarineSlab:              MIRROR_XZ(PrismarineSlab)
			case BlockType::PurpurSlab:                  MIRROR_XZ(PurpurSlab)
			case BlockType::QuartzSlab:                  MIRROR_XZ(QuartzSlab)
			case BlockType::RedNetherBrickSlab:          MIRROR_XZ(RedNetherBrickSlab)
			case BlockType::RedSandstoneSlab:            MIRROR_XZ(RedSandstoneSlab)
			case BlockType::SandstoneSlab:               MIRROR_XZ(SandstoneSlab)
			case BlockType::SmoothQuartzSlab:            MIRROR_XZ(SmoothQuartzSlab)
			case BlockType::SmoothRedSandstoneSlab:      MIRROR_XZ(SmoothRedSandstoneSlab)
			case BlockType::SmoothSandstoneSlab:         MIRROR_XZ(SmoothSandstoneSlab)
			case BlockType::SmoothStoneSlab:             MIRROR_XZ(SmoothStoneSlab)
			case BlockType::SpruceSlab:                  MIRROR_XZ(SpruceSlab)
			case BlockType::StoneBrickSlab:              MIRROR_XZ(StoneBrickSlab)
			case BlockType::StoneSlab:                   MIRROR_XZ(StoneSlab)
			case BlockType::WarpedSlab:                  MIRROR_XZ(WarpedSlab)
			default: return a_Block;
		}
	}
#undef MIRROR_XZ




	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		a_Meta &= 0x7;

		switch (m_BlockType)
		{
			case E_BLOCK_STONE_SLAB:
			{
				switch (a_Meta)
				{
					case E_META_STONE_SLAB_SANDSTONE: return 2;
					case E_META_STONE_SLAB_PLANKS: return 13;
					case E_META_STONE_SLAB_STONE_BRICK:
					case E_META_STONE_SLAB_STONE:
					case E_META_STONE_SLAB_COBBLESTONE: return 11;
					case E_META_STONE_SLAB_BRICK: return 28;
					case E_META_STONE_SLAB_NETHER_BRICK: return 35;
					case E_META_STONE_SLAB_QUARTZ: return 8;
					default:
					{
						ASSERT(!"Unhandled meta in slab handler!");
						return 0;
					}
				}
			}
			case E_BLOCK_WOODEN_SLAB:
			{
				switch (a_Meta)
				{
					case E_META_WOODEN_SLAB_BIRCH: return 2;
					case E_META_WOODEN_SLAB_JUNGLE: return 10;
					case E_META_WOODEN_SLAB_OAK: return 13;
					case E_META_WOODEN_SLAB_ACACIA: return 15;
					case E_META_WOODEN_SLAB_DARK_OAK: return 26;
					case E_META_WOODEN_SLAB_SPRUCE: return 34;
					default:
					{
						ASSERT(!"Unhandled meta in slab handler!");
						return 0;
					}
				}
			}
			case E_BLOCK_RED_SANDSTONE_SLAB:
			{
				return 10;
			}
			case E_BLOCK_PURPUR_SLAB:
			{
				return 16;
			}
			default:
			{
				ASSERT(!"Unhandled blocktype in slab handler!");
				return 0;
			}
		}
	}




	// Todo: This is temporary. Please override this function if block states are implemented.
	ColourID GetMapBaseColourID() const
	{
		// switch (m_BlockType)
		// Todo: replace with the correct switch statement
		switch (BlockType::AcaciaSlab)
		{
			case BlockType::AcaciaSlab:                  return 15;
			case BlockType::AndesiteSlab:                return 11;
			case BlockType::BirchSlab:                   return 2;
			case BlockType::BlackstoneSlab:              return 29;
			case BlockType::BrickSlab:                   return 28;
			case BlockType::CobblestoneSlab:             return 11;
			case BlockType::CrimsonSlab:                 return 35;
			case BlockType::CutRedSandstoneSlab:         return 10;
			case BlockType::CutSandstoneSlab:            return 2;
			case BlockType::DarkOakSlab:                 return 26;
			case BlockType::DarkPrismarineSlab:          return 31;
			case BlockType::DioriteSlab:                 return 14;
			case BlockType::EndStoneBrickSlab:           return 2;
			case BlockType::GraniteSlab:                 return 10;
			case BlockType::JungleSlab:                  return 10;
			case BlockType::MossyCobblestoneSlab:        return 11;
			case BlockType::MossyStoneBrickSlab:         return 11;
			case BlockType::NetherBrickSlab:             return 35;
			case BlockType::OakSlab:                     return 13;
			case BlockType::PetrifiedOakSlab:            return 13;
			case BlockType::PolishedAndesiteSlab:        return 11;
			case BlockType::PolishedBlackstoneBrickSlab: return 29;
			case BlockType::PolishedBlackstoneSlab:      return 29;
			case BlockType::PolishedDioriteSlab:         return 14;
			case BlockType::PolishedGraniteSlab:         return 10;
			case BlockType::PrismarineBrickSlab:         return 23;
			case BlockType::PrismarineSlab:              return 23;
			case BlockType::PurpurSlab:                  return 16;
			case BlockType::QuartzSlab:                  return 8;
			case BlockType::RedNetherBrickSlab:          return 35;
			case BlockType::RedSandstoneSlab:            return 10;
			case BlockType::SandstoneSlab:               return 2;
			case BlockType::SmoothQuartzSlab:            return 8;
			case BlockType::SmoothRedSandstoneSlab:      return 10;
			case BlockType::SmoothSandstoneSlab:         return 2;
			case BlockType::SmoothStoneSlab:             return 11;
			case BlockType::SpruceSlab:                  return 34;
			case BlockType::StoneBrickSlab:              return 11;
			case BlockType::StoneSlab:                   return 11;
			case BlockType::WarpedSlab:                  return 23;
			default:
			{
				ASSERT(!"Unhandled blocktype in slab handler!");
				return 0;
			}
		}
	}





	virtual bool IsInsideBlock(Vector3d a_Position, const NIBBLETYPE a_BlockMeta) const override
	{
		if (a_BlockMeta & 0x08)  // top half
		{
			return true;
		}
		return cBlockHandler::IsInsideBlock(a_Position, a_BlockMeta);
	}
} ;





class cBlockDoubleSlabHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		BLOCKTYPE Block = GetSingleSlabType(m_BlockType);
		return cItem(Block, 2, a_BlockMeta & 0x7);
	}





	constexpr static BLOCKTYPE GetSingleSlabType(BLOCKTYPE a_BlockType)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_DOUBLE_STONE_SLAB:         return E_BLOCK_STONE_SLAB;
			case E_BLOCK_DOUBLE_WOODEN_SLAB:        return E_BLOCK_WOODEN_SLAB;
			case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB: return E_BLOCK_RED_SANDSTONE_SLAB;
			case E_BLOCK_PURPUR_DOUBLE_SLAB:        return E_BLOCK_PURPUR_SLAB;
		}
		ASSERT(!"Unhandled double slab type!");
		return a_BlockType;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		// For double slabs, the meta values are the same. Only the meaning of the 4th bit changes, but that's ignored in the below handler
		return cBlockHandler::For(GetSingleSlabType(m_BlockType)).GetMapBaseColourID(a_Meta);
	}
} ;




