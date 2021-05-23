
// BlockSlab.h

// Declares cBlockSlabHandler and cBlockDoubleSlabHandler classes





#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../BlockInfo.h"
#include "../Entities/Player.h"
#include "../BlockInfo.h"



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
			default: return false;
		}
	}

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaSlab: return cItem(Item::AcaciaSlab);
			case BlockType::AndesiteSlab: return cItem(Item::AndesiteSlab);
			case BlockType::BirchSlab: return cItem(Item::BirchSlab);
			case BlockType::BlackstoneSlab: return cItem(Item::BlackstoneSlab);
			case BlockType::BrickSlab: return cItem(Item::BrickSlab);
			case BlockType::CobblestoneSlab: return cItem(Item::CobblestoneSlab);
			case BlockType::CrimsonSlab: return cItem(Item::CrimsonSlab);
			case BlockType::CutRedSandstoneSlab: return cItem(Item::CutRedSandstoneSlab);
			case BlockType::CutSandstoneSlab: return cItem(Item::CutSandstoneSlab);
			case BlockType::DarkOakSlab: return cItem(Item::DarkOakSlab);
			case BlockType::DarkPrismarineSlab: return cItem(Item::DarkPrismarineSlab);
			case BlockType::DioriteSlab: return cItem(Item::DioriteSlab);
			case BlockType::EndStoneBrickSlab: return cItem(Item::EndStoneBrickSlab);
			case BlockType::GraniteSlab: return cItem(Item::GraniteSlab);
			case BlockType::JungleSlab: return cItem(Item::JungleSlab);
			case BlockType::MossyCobblestoneSlab: return cItem(Item::MossyCobblestoneSlab);
			case BlockType::MossyStoneBrickSlab: return cItem(Item::MossyStoneBrickSlab);
			case BlockType::NetherBrickSlab: return cItem(Item::NetherBrickSlab);
			case BlockType::OakSlab: return cItem(Item::OakSlab);
			case BlockType::PetrifiedOakSlab: return cItem(Item::PetrifiedOakSlab);
			case BlockType::PolishedAndesiteSlab: return cItem(Item::PolishedAndesiteSlab);
			case BlockType::PolishedBlackstoneBrickSlab: return cItem(Item::PolishedBlackstoneBrickSlab);
			case BlockType::PolishedBlackstoneSlab: return cItem(Item::PolishedBlackstoneSlab);
			case BlockType::PolishedDioriteSlab: return cItem(Item::PolishedDioriteSlab);
			case BlockType::PolishedGraniteSlab: return cItem(Item::PolishedGraniteSlab);
			case BlockType::PrismarineBrickSlab: return cItem(Item::PrismarineBrickSlab);
			case BlockType::PrismarineSlab: return cItem(Item::PrismarineSlab);
			case BlockType::PurpurSlab: return cItem(Item::PurpurSlab);
			case BlockType::QuartzSlab: return cItem(Item::QuartzSlab);
			case BlockType::RedNetherBrickSlab: return cItem(Item::RedNetherBrickSlab);
			case BlockType::RedSandstoneSlab: return cItem(Item::RedSandstoneSlab);
			case BlockType::SandstoneSlab: return cItem(Item::SandstoneSlab);
			case BlockType::SmoothQuartzSlab: return cItem(Item::SmoothQuartzSlab);
			case BlockType::SmoothRedSandstoneSlab: return cItem(Item::SmoothRedSandstoneSlab);
			case BlockType::SmoothSandstoneSlab: return cItem(Item::SmoothSandstoneSlab);
			case BlockType::SmoothStoneSlab: return cItem(Item::SmoothStoneSlab);
			case BlockType::SpruceSlab: return cItem(Item::SpruceSlab);
			case BlockType::StoneBrickSlab: return cItem(Item::StoneBrickSlab);
			case BlockType::StoneSlab: return cItem(Item::StoneSlab);
			case BlockType::WarpedSlab: return cItem(Item::WarpedSlab);
			default: return cItem();
		}
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

	virtual bool GetPlacementBlockTypeMeta(
			cChunkInterface &a_ChunkInterface,
			cPlayer &a_Player,
			const Vector3i a_PlacedBlockPos,
			eBlockFace a_ClickedBlockFace,
			const Vector3i a_CursorPos,
			BlockState & a_Block
	) const override
	{
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
				if (a_CursorPos.y > 7)
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
		}  // switch (a_BlockFace)

		bool DoubleSlab = false;

		// Check if the block at the coordinates is a single slab. Eligibility for combining has already been processed in ClientHandle
		// Changed to-be-placed to a double slab if we are clicking on a single slab, as opposed to placing one for the first time
		if (IsAnySlabType(a_ChunkInterface.GetBlock(a_PlacedBlockPos)))
		{
			DoubleSlab = true;
		}

		switch (m_BlockType)
		{
			case BlockType::AcaciaSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::AcaciaSlab::AcaciaSlab(Block::AcaciaSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::AcaciaSlab::AcaciaSlab(Block::AcaciaSlab::Type::Top);
					}
					else
					{
						a_Block = Block::AcaciaSlab::AcaciaSlab(Block::AcaciaSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::AndesiteSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::AndesiteSlab::AndesiteSlab(Block::AndesiteSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::AndesiteSlab::AndesiteSlab(Block::AndesiteSlab::Type::Top);
					}
					else
					{
						a_Block = Block::AndesiteSlab::AndesiteSlab(Block::AndesiteSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::BirchSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::BirchSlab::BirchSlab(Block::BirchSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::BirchSlab::BirchSlab(Block::BirchSlab::Type::Top);
					}
					else
					{
						a_Block = Block::BirchSlab::BirchSlab(Block::BirchSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::BlackstoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::BlackstoneSlab::BlackstoneSlab(Block::BlackstoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::BlackstoneSlab::BlackstoneSlab(Block::BlackstoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::BlackstoneSlab::BlackstoneSlab(Block::BlackstoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::BrickSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::BrickSlab::BrickSlab(Block::BrickSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::BrickSlab::BrickSlab(Block::BrickSlab::Type::Top);
					}
					else
					{
						a_Block = Block::BrickSlab::BrickSlab(Block::BrickSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::CobblestoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::CobblestoneSlab::CobblestoneSlab(Block::CobblestoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::CobblestoneSlab::CobblestoneSlab(Block::CobblestoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::CobblestoneSlab::CobblestoneSlab(Block::CobblestoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::CrimsonSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::CrimsonSlab::CrimsonSlab(Block::CrimsonSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::CrimsonSlab::CrimsonSlab(Block::CrimsonSlab::Type::Top);
					}
					else
					{
						a_Block = Block::CrimsonSlab::CrimsonSlab(Block::CrimsonSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::CutRedSandstoneSlab:
				{
				if (DoubleSlab)
				{
					a_Block = Block::CutRedSandstoneSlab::CutRedSandstoneSlab(Block::CutRedSandstoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::CutRedSandstoneSlab::CutRedSandstoneSlab(Block::CutRedSandstoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::CutRedSandstoneSlab::CutRedSandstoneSlab(Block::CutRedSandstoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::CutSandstoneSlab:
				{
				if (DoubleSlab)
				{
					a_Block = Block::CutSandstoneSlab::CutSandstoneSlab(Block::CutSandstoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::CutSandstoneSlab::CutSandstoneSlab(Block::CutSandstoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::CutSandstoneSlab::CutSandstoneSlab(Block::CutSandstoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::DarkOakSlab:
				{
				if (DoubleSlab)
				{
					a_Block = Block::DarkOakSlab::DarkOakSlab(Block::DarkOakSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::DarkOakSlab::DarkOakSlab(Block::DarkOakSlab::Type::Top);
					}
					else
					{
						a_Block = Block::DarkOakSlab::DarkOakSlab(Block::DarkOakSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::DarkPrismarineSlab:
				{
				if (DoubleSlab)
				{
					a_Block = Block::DarkPrismarineSlab::DarkPrismarineSlab(Block::DarkPrismarineSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::DarkPrismarineSlab::DarkPrismarineSlab(Block::DarkPrismarineSlab::Type::Top);
					}
					else
					{
						a_Block = Block::DarkPrismarineSlab::DarkPrismarineSlab(Block::DarkPrismarineSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::DioriteSlab:
				{
				if (DoubleSlab)
				{
					a_Block = Block::DioriteSlab::DioriteSlab(Block::DioriteSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::DioriteSlab::DioriteSlab(Block::DioriteSlab::Type::Top);
					}
					else
					{
						a_Block = Block::DioriteSlab::DioriteSlab(Block::DioriteSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::EndStoneBrickSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::EndStoneBrickSlab::EndStoneBrickSlab(Block::EndStoneBrickSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::EndStoneBrickSlab::EndStoneBrickSlab(Block::EndStoneBrickSlab::Type::Top);
					}
					else
					{
						a_Block = Block::EndStoneBrickSlab::EndStoneBrickSlab(Block::EndStoneBrickSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::GraniteSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::GraniteSlab::GraniteSlab(Block::GraniteSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::GraniteSlab::GraniteSlab(Block::GraniteSlab::Type::Top);
					}
					else
					{
						a_Block = Block::GraniteSlab::GraniteSlab(Block::GraniteSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::JungleSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::JungleSlab::JungleSlab(Block::JungleSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::JungleSlab::JungleSlab(Block::JungleSlab::Type::Top);
					}
					else
					{
						a_Block = Block::JungleSlab::JungleSlab(Block::JungleSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::MossyCobblestoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::MossyCobblestoneSlab::MossyCobblestoneSlab(Block::MossyCobblestoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::MossyCobblestoneSlab::MossyCobblestoneSlab(Block::MossyCobblestoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::MossyCobblestoneSlab::MossyCobblestoneSlab(Block::MossyCobblestoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::MossyStoneBrickSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::MossyStoneBrickSlab::MossyStoneBrickSlab(Block::MossyStoneBrickSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::MossyStoneBrickSlab::MossyStoneBrickSlab(Block::MossyStoneBrickSlab::Type::Top);
					}
					else
					{
						a_Block = Block::MossyStoneBrickSlab::MossyStoneBrickSlab(Block::MossyStoneBrickSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::NetherBrickSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::NetherBrickSlab::NetherBrickSlab(Block::NetherBrickSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::NetherBrickSlab::NetherBrickSlab(Block::NetherBrickSlab::Type::Top);
					}
					else
					{
						a_Block = Block::NetherBrickSlab::NetherBrickSlab(Block::NetherBrickSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::OakSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::OakSlab::OakSlab(Block::OakSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::OakSlab::OakSlab(Block::OakSlab::Type::Top);
					}
					else
					{
						a_Block = Block::OakSlab::OakSlab(Block::OakSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PetrifiedOakSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PetrifiedOakSlab::PetrifiedOakSlab(Block::PetrifiedOakSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PetrifiedOakSlab::PetrifiedOakSlab(Block::PetrifiedOakSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PetrifiedOakSlab::PetrifiedOakSlab(Block::PetrifiedOakSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PolishedAndesiteSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PolishedAndesiteSlab::PolishedAndesiteSlab(Block::PolishedAndesiteSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PolishedAndesiteSlab::PolishedAndesiteSlab(Block::PolishedAndesiteSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PolishedAndesiteSlab::PolishedAndesiteSlab(Block::PolishedAndesiteSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PolishedBlackstoneBrickSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PolishedBlackstoneBrickSlab::PolishedBlackstoneBrickSlab(Block::PolishedBlackstoneBrickSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PolishedBlackstoneBrickSlab::PolishedBlackstoneBrickSlab(Block::PolishedBlackstoneBrickSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PolishedBlackstoneBrickSlab::PolishedBlackstoneBrickSlab(Block::PolishedBlackstoneBrickSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PolishedBlackstoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PolishedBlackstoneSlab::PolishedBlackstoneSlab(Block::PolishedBlackstoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PolishedBlackstoneSlab::PolishedBlackstoneSlab(Block::PolishedBlackstoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PolishedBlackstoneSlab::PolishedBlackstoneSlab(Block::PolishedBlackstoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PolishedDioriteSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PolishedDioriteSlab::PolishedDioriteSlab(Block::PolishedDioriteSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PolishedDioriteSlab::PolishedDioriteSlab(Block::PolishedDioriteSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PolishedDioriteSlab::PolishedDioriteSlab(Block::PolishedDioriteSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PolishedGraniteSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PolishedGraniteSlab::PolishedGraniteSlab(Block::PolishedGraniteSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PolishedGraniteSlab::PolishedGraniteSlab(Block::PolishedGraniteSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PolishedGraniteSlab::PolishedGraniteSlab(Block::PolishedGraniteSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PrismarineBrickSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PrismarineBrickSlab::PrismarineBrickSlab(Block::PrismarineBrickSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PrismarineBrickSlab::PrismarineBrickSlab(Block::PrismarineBrickSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PrismarineBrickSlab::PrismarineBrickSlab(Block::PrismarineBrickSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PrismarineSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PrismarineSlab::PrismarineSlab(Block::PrismarineSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PrismarineSlab::PrismarineSlab(Block::PrismarineSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PrismarineSlab::PrismarineSlab(Block::PrismarineSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::PurpurSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::PurpurSlab::PurpurSlab(Block::PurpurSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::PurpurSlab::PurpurSlab(Block::PurpurSlab::Type::Top);
					}
					else
					{
						a_Block = Block::PurpurSlab::PurpurSlab(Block::PurpurSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::QuartzSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::QuartzSlab::QuartzSlab(Block::QuartzSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::QuartzSlab::QuartzSlab(Block::QuartzSlab::Type::Top);
					}
					else
					{
						a_Block = Block::QuartzSlab::QuartzSlab(Block::QuartzSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::RedNetherBrickSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::RedNetherBrickSlab::RedNetherBrickSlab(Block::RedNetherBrickSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::RedNetherBrickSlab::RedNetherBrickSlab(Block::RedNetherBrickSlab::Type::Top);
					}
					else
					{
						a_Block = Block::RedNetherBrickSlab::RedNetherBrickSlab(Block::RedNetherBrickSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::RedSandstoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::RedSandstoneSlab::RedSandstoneSlab(Block::RedSandstoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::RedSandstoneSlab::RedSandstoneSlab(Block::RedSandstoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::RedSandstoneSlab::RedSandstoneSlab(Block::RedSandstoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::SandstoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::SandstoneSlab::SandstoneSlab(Block::SandstoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::SandstoneSlab::SandstoneSlab(Block::SandstoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::SandstoneSlab::SandstoneSlab(Block::SandstoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::SmoothQuartzSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::SmoothQuartzSlab::SmoothQuartzSlab(Block::SmoothQuartzSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::SmoothQuartzSlab::SmoothQuartzSlab(Block::SmoothQuartzSlab::Type::Top);
					}
					else
					{
						a_Block = Block::SmoothQuartzSlab::SmoothQuartzSlab(Block::SmoothQuartzSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::SmoothRedSandstoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::SmoothRedSandstoneSlab::SmoothRedSandstoneSlab(Block::SmoothRedSandstoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::SmoothRedSandstoneSlab::SmoothRedSandstoneSlab(Block::SmoothRedSandstoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::SmoothRedSandstoneSlab::SmoothRedSandstoneSlab(Block::SmoothRedSandstoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::SmoothSandstoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::SmoothSandstoneSlab::SmoothSandstoneSlab(Block::SmoothSandstoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::SmoothSandstoneSlab::SmoothSandstoneSlab(Block::SmoothSandstoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::SmoothSandstoneSlab::SmoothSandstoneSlab(Block::SmoothSandstoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::SmoothStoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::SmoothStoneSlab::SmoothStoneSlab(Block::SmoothStoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::SmoothStoneSlab::SmoothStoneSlab(Block::SmoothStoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::SmoothStoneSlab::SmoothStoneSlab(Block::SmoothStoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::SpruceSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::SpruceSlab::SpruceSlab(Block::SpruceSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::SpruceSlab::SpruceSlab(Block::SpruceSlab::Type::Top);
					}
					else
					{
						a_Block = Block::SpruceSlab::SpruceSlab(Block::SpruceSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::StoneBrickSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::StoneBrickSlab::StoneBrickSlab(Block::StoneBrickSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::StoneBrickSlab::StoneBrickSlab(Block::StoneBrickSlab::Type::Top);
					}
					else
					{
						a_Block = Block::StoneBrickSlab::StoneBrickSlab(Block::StoneBrickSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::StoneSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::StoneSlab::StoneSlab(Block::StoneSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::StoneSlab::StoneSlab(Block::StoneSlab::Type::Top);
					}
					else
					{
						a_Block = Block::StoneSlab::StoneSlab(Block::StoneSlab::Type::Bottom);
					}
				}
				break;
			}
			case BlockType::WarpedSlab:
			{
				if (DoubleSlab)
				{
					a_Block = Block::WarpedSlab::WarpedSlab(Block::WarpedSlab::Type::Double);
				}
				else
				{
					if (IsTop)
					{
						a_Block = Block::WarpedSlab::WarpedSlab(Block::WarpedSlab::Type::Top);
					}
					else
					{
						a_Block = Block::WarpedSlab::WarpedSlab(Block::WarpedSlab::Type::Bottom);
					}
				}
				break;
			}
			default: return false;
		}

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


#define METAMIRRORXZ(SlabType) \
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
