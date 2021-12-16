
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

private:

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

			a_Player.SendBlocksAround(a_PlacePosition.x, a_PlacePosition.y, a_PlacePosition.z, 2);  // (see below)
			return true;
		}

		// Set the correct metadata based on player equipped item:
		if (!a_Player.PlaceBlock(a_PlacePosition, BlockItemConverter::FromItem(a_HeldItem.m_ItemType)))
		{
			return false;
		}

		/* This is a workaround for versions < 1.13, where the client combines a slab in the
		direction of the clicked block face of a block ignoring build collision, rather than replacing said block.
		Resend blocks to the client to fix the bug.
		Ref.: https://forum.cuberite.org/thread-434-post-17388.html#pid17388 */
		a_Player.SendBlocksAround(a_PlacePosition.x, a_PlacePosition.y, a_PlacePosition.z, 2);

		return true;
	}





	static BlockState GetDoubleSlab(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
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

/*

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

*/
