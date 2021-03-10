
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"




class cBlockStairsHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockStairs(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaStairs:
			case BlockType::AndesiteStairs:
			case BlockType::BirchStairs:
			case BlockType::BlackstoneStairs:
			case BlockType::BrickStairs:
			case BlockType::CobblestoneStairs:
			case BlockType::CrimsonStairs:
			case BlockType::DarkOakStairs:
			case BlockType::DarkPrismarineStairs:
			case BlockType::DioriteStairs:
			case BlockType::EndStoneBrickStairs:
			case BlockType::GraniteStairs:
			case BlockType::JungleStairs:
			case BlockType::MossyCobblestoneStairs:
			case BlockType::MossyStoneBrickStairs:
			case BlockType::NetherBrickStairs:
			case BlockType::OakStairs:
			case BlockType::PolishedAndesiteStairs:
			case BlockType::PolishedBlackstoneBrickStairs:
			case BlockType::PolishedBlackstoneStairs:
			case BlockType::PolishedDioriteStairs:
			case BlockType::PolishedGraniteStairs:
			case BlockType::PrismarineBrickStairs:
			case BlockType::PrismarineStairs:
			case BlockType::QuartzStairs:
			case BlockType::PurpurStairs:
			case BlockType::RedNetherBrickStairs:
			case BlockType::RedSandstoneStairs:
			case BlockType::SandstoneStairs:
			case BlockType::SmoothQuartzStairs:
			case BlockType::SmoothRedSandstoneStairs:
			case BlockType::SmoothSandstoneStairs:
			case BlockType::SpruceStairs:
			case BlockType::StoneBrickStairs:
			case BlockType::StoneStairs:
			case BlockType::WarpedStairs:
				return true;
			default: return false;
		}
	}

	static inline bool IsStairsTopHalf(BlockState a_Block)
	{
		using namespace Block;

		switch (a_Block.Type())
		{
			case BlockType::AcaciaStairs:                  return AcaciaStairs::Half(a_Block) == AcaciaStairs::Half::Top;
			case BlockType::AndesiteStairs:                return AndesiteStairs::Half(a_Block) == AndesiteStairs::Half::Top;
			case BlockType::BirchStairs:                   return BirchStairs::Half(a_Block) == BirchStairs::Half::Top;
			case BlockType::BlackstoneStairs:              return BlackstoneStairs::Half(a_Block) == BlackstoneStairs::Half::Top;
			case BlockType::BrickStairs:                   return BrickStairs::Half(a_Block) == BrickStairs::Half::Top;
			case BlockType::CobblestoneStairs:             return CobblestoneStairs::Half(a_Block) == CobblestoneStairs::Half::Top;
			case BlockType::CrimsonStairs:                 return CrimsonStairs::Half(a_Block) == CrimsonStairs::Half::Top;
			case BlockType::DarkOakStairs:                 return DarkOakStairs::Half(a_Block) == DarkOakStairs::Half::Top;
			case BlockType::DarkPrismarineStairs:          return DarkPrismarineStairs::Half(a_Block) == DarkPrismarineStairs::Half::Top;
			case BlockType::DioriteStairs:                 return DioriteStairs::Half(a_Block) == DioriteStairs::Half::Top;
			case BlockType::EndStoneBrickStairs:           return EndStoneBrickStairs::Half(a_Block) == EndStoneBrickStairs::Half::Top;
			case BlockType::GraniteStairs:                 return GraniteStairs::Half(a_Block) == GraniteStairs::Half::Top;
			case BlockType::JungleStairs:                  return JungleStairs::Half(a_Block) == JungleStairs::Half::Top;
			case BlockType::MossyCobblestoneStairs:        return MossyCobblestoneStairs::Half(a_Block) == MossyCobblestoneStairs::Half::Top;
			case BlockType::MossyStoneBrickStairs:         return MossyStoneBrickStairs::Half(a_Block) == MossyStoneBrickStairs::Half::Top;
			case BlockType::NetherBrickStairs:             return NetherBrickStairs::Half(a_Block) == NetherBrickStairs::Half::Top;
			case BlockType::OakStairs:                     return OakStairs::Half(a_Block) == OakStairs::Half::Top;
			case BlockType::PolishedAndesiteStairs:        return PolishedAndesiteStairs::Half(a_Block) == PolishedAndesiteStairs::Half::Top;
			case BlockType::PolishedBlackstoneBrickStairs: return PolishedBlackstoneBrickStairs::Half(a_Block) == PolishedBlackstoneBrickStairs::Half::Top;
			case BlockType::PolishedBlackstoneStairs:      return PolishedBlackstoneStairs::Half(a_Block) == PolishedBlackstoneStairs::Half::Top;
			case BlockType::PolishedDioriteStairs:         return PolishedDioriteStairs::Half(a_Block) == PolishedDioriteStairs::Half::Top;
			case BlockType::PolishedGraniteStairs:         return PolishedGraniteStairs::Half(a_Block) == PolishedGraniteStairs::Half::Top;
			case BlockType::PrismarineBrickStairs:         return PrismarineBrickStairs::Half(a_Block) == PrismarineBrickStairs::Half::Top;
			case BlockType::PrismarineStairs:              return PrismarineStairs::Half(a_Block) == PrismarineStairs::Half::Top;
			case BlockType::QuartzStairs:                  return QuartzStairs::Half(a_Block) == QuartzStairs::Half::Top;
			case BlockType::PurpurStairs:                  return PurpurStairs::Half(a_Block) == PurpurStairs::Half::Top;
			case BlockType::RedNetherBrickStairs:          return RedNetherBrickStairs::Half(a_Block) == RedNetherBrickStairs::Half::Top;
			case BlockType::RedSandstoneStairs:            return RedSandstoneStairs::Half(a_Block) == RedSandstoneStairs::Half::Top;
			case BlockType::SandstoneStairs:               return SandstoneStairs::Half(a_Block) == SandstoneStairs::Half::Top;
			case BlockType::SmoothQuartzStairs:            return SmoothQuartzStairs::Half(a_Block) == SmoothQuartzStairs::Half::Top;
			case BlockType::SmoothRedSandstoneStairs:      return SmoothRedSandstoneStairs::Half(a_Block) == SmoothRedSandstoneStairs::Half::Top;
			case BlockType::SmoothSandstoneStairs:         return SmoothSandstoneStairs::Half(a_Block) == SmoothSandstoneStairs::Half::Top;
			case BlockType::SpruceStairs:                  return SpruceStairs::Half(a_Block) == SpruceStairs::Half::Top;
			case BlockType::StoneBrickStairs:              return StoneBrickStairs::Half(a_Block) == StoneBrickStairs::Half::Top;
			case BlockType::StoneStairs:                   return StoneStairs::Half(a_Block) == StoneStairs::Half::Top;
			case BlockType::WarpedStairs:                  return WarpedStairs::Half(a_Block) == WarpedStairs::Half::Top;
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
		UNUSED(a_ChunkInterface);
		UNUSED(a_PlacedBlockPos);

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
				if (a_CursorPos.y > 8)
				{
					Inverse = true;
				}
				break;
			}
			case BLOCK_FACE_NONE: return false;
		}

		switch (m_BlockType)
		{
			case BlockType::AcaciaStairs:                  a_Block = AcaciaStairs::AcaciaStairs(BlockFace, Inverse ? AcaciaStairs::Half::Top : AcaciaStairs::Half::Bottom, AcaciaStairs::Shape::Straight); break;
			case BlockType::AndesiteStairs:                a_Block = AndesiteStairs::AndesiteStairs(BlockFace, Inverse ? AndesiteStairs::Half::Top : AndesiteStairs::Half::Bottom, AndesiteStairs::Shape::Straight); break;
			case BlockType::BirchStairs:                   a_Block = BirchStairs::BirchStairs(BlockFace, Inverse ? BirchStairs::Half::Top : BirchStairs::Half::Bottom, BirchStairs::Shape::Straight); break;
			case BlockType::BlackstoneStairs:              a_Block = BlackstoneStairs::BlackstoneStairs(BlockFace, Inverse ? BlackstoneStairs::Half::Top : BlackstoneStairs::Half::Bottom, BlackstoneStairs::Shape::Straight); break;
			case BlockType::BrickStairs:                   a_Block = BrickStairs::BrickStairs(BlockFace, Inverse ? BrickStairs::Half::Top : BrickStairs::Half::Bottom, BrickStairs::Shape::Straight); break;
			case BlockType::CobblestoneStairs:             a_Block = CobblestoneStairs::CobblestoneStairs(BlockFace, Inverse ? CobblestoneStairs::Half::Top : CobblestoneStairs::Half::Bottom, CobblestoneStairs::Shape::Straight); break;
			case BlockType::CrimsonStairs:                 a_Block = CrimsonStairs::CrimsonStairs(BlockFace, Inverse ? CrimsonStairs::Half::Top : CrimsonStairs::Half::Bottom, CrimsonStairs::Shape::Straight); break;
			case BlockType::DarkOakStairs:                 a_Block = DarkOakStairs::DarkOakStairs(BlockFace, Inverse ? DarkOakStairs::Half::Top : DarkOakStairs::Half::Bottom, DarkOakStairs::Shape::Straight); break;
			case BlockType::DarkPrismarineStairs:          a_Block = DarkPrismarineStairs::DarkPrismarineStairs(BlockFace, Inverse ? DarkPrismarineStairs::Half::Top : DarkPrismarineStairs::Half::Bottom, DarkPrismarineStairs::Shape::Straight); break;
			case BlockType::DioriteStairs:                 a_Block = DioriteStairs::DioriteStairs(BlockFace, Inverse ? DioriteStairs::Half::Top : DioriteStairs::Half::Bottom, DioriteStairs::Shape::Straight); break;
			case BlockType::EndStoneBrickStairs:           a_Block = EndStoneBrickStairs::EndStoneBrickStairs(BlockFace, Inverse ? EndStoneBrickStairs::Half::Top : EndStoneBrickStairs::Half::Bottom, EndStoneBrickStairs::Shape::Straight); break;
			case BlockType::GraniteStairs:                 a_Block = GraniteStairs::GraniteStairs(BlockFace, Inverse ? GraniteStairs::Half::Top : GraniteStairs::Half::Bottom, GraniteStairs::Shape::Straight); break;
			case BlockType::JungleStairs:                  a_Block = JungleStairs::JungleStairs(BlockFace, Inverse ? JungleStairs::Half::Top : JungleStairs::Half::Bottom, JungleStairs::Shape::Straight); break;
			case BlockType::MossyCobblestoneStairs:        a_Block = MossyCobblestoneStairs::MossyCobblestoneStairs(BlockFace, Inverse ? MossyCobblestoneStairs::Half::Top : MossyCobblestoneStairs::Half::Bottom, MossyCobblestoneStairs::Shape::Straight); break;
			case BlockType::MossyStoneBrickStairs:         a_Block = MossyStoneBrickStairs::MossyStoneBrickStairs(BlockFace, Inverse ? MossyStoneBrickStairs::Half::Top : MossyStoneBrickStairs::Half::Bottom, MossyStoneBrickStairs::Shape::Straight); break;
			case BlockType::NetherBrickStairs:             a_Block = NetherBrickStairs::NetherBrickStairs(BlockFace, Inverse ? NetherBrickStairs::Half::Top : NetherBrickStairs::Half::Bottom, NetherBrickStairs::Shape::Straight); break;
			case BlockType::OakStairs:                     a_Block = OakStairs::OakStairs(BlockFace, Inverse ? OakStairs::Half::Top : OakStairs::Half::Bottom, OakStairs::Shape::Straight); break;
			case BlockType::PolishedAndesiteStairs:        a_Block = PolishedAndesiteStairs::PolishedAndesiteStairs(BlockFace, Inverse ? PolishedAndesiteStairs::Half::Top : PolishedAndesiteStairs::Half::Bottom, PolishedAndesiteStairs::Shape::Straight); break;
			case BlockType::PolishedBlackstoneBrickStairs: a_Block = PolishedBlackstoneBrickStairs::PolishedBlackstoneBrickStairs(BlockFace, Inverse ? PolishedBlackstoneBrickStairs::Half::Top : PolishedBlackstoneBrickStairs::Half::Bottom, PolishedBlackstoneBrickStairs::Shape::Straight); break;
			case BlockType::PolishedBlackstoneStairs:      a_Block = PolishedBlackstoneStairs::PolishedBlackstoneStairs(BlockFace, Inverse ? PolishedBlackstoneStairs::Half::Top : PolishedBlackstoneStairs::Half::Bottom, PolishedBlackstoneStairs::Shape::Straight); break;
			case BlockType::PolishedDioriteStairs:         a_Block = PolishedDioriteStairs::PolishedDioriteStairs(BlockFace, Inverse ? PolishedDioriteStairs::Half::Top : PolishedDioriteStairs::Half::Bottom, PolishedDioriteStairs::Shape::Straight); break;
			case BlockType::PolishedGraniteStairs:         a_Block = PolishedGraniteStairs::PolishedGraniteStairs(BlockFace, Inverse ? PolishedGraniteStairs::Half::Top : PolishedGraniteStairs::Half::Bottom, PolishedGraniteStairs::Shape::Straight); break;
			case BlockType::PrismarineBrickStairs:         a_Block = PrismarineBrickStairs::PrismarineBrickStairs(BlockFace, Inverse ? PrismarineBrickStairs::Half::Top : PrismarineBrickStairs::Half::Bottom, PrismarineBrickStairs::Shape::Straight); break;
			case BlockType::PrismarineStairs:              a_Block = PrismarineStairs::PrismarineStairs(BlockFace, Inverse ? PrismarineStairs::Half::Top : PrismarineStairs::Half::Bottom, PrismarineStairs::Shape::Straight); break;
			case BlockType::QuartzStairs:                  a_Block = QuartzStairs::QuartzStairs(BlockFace, Inverse ? QuartzStairs::Half::Top : QuartzStairs::Half::Bottom, QuartzStairs::Shape::Straight); break;
			case BlockType::PurpurStairs:                  a_Block = PurpurStairs::PurpurStairs(BlockFace, Inverse ? PurpurStairs::Half::Top : PurpurStairs::Half::Bottom, PurpurStairs::Shape::Straight); break;
			case BlockType::RedNetherBrickStairs:          a_Block = RedNetherBrickStairs::RedNetherBrickStairs(BlockFace, Inverse ? RedNetherBrickStairs::Half::Top : RedNetherBrickStairs::Half::Bottom, RedNetherBrickStairs::Shape::Straight); break;
			case BlockType::RedSandstoneStairs:            a_Block = RedSandstoneStairs::RedSandstoneStairs(BlockFace, Inverse ? RedSandstoneStairs::Half::Top : RedSandstoneStairs::Half::Bottom, RedSandstoneStairs::Shape::Straight); break;
			case BlockType::SandstoneStairs:               a_Block = SandstoneStairs::SandstoneStairs(BlockFace, Inverse ? SandstoneStairs::Half::Top : SandstoneStairs::Half::Bottom, SandstoneStairs::Shape::Straight); break;
			case BlockType::SmoothQuartzStairs:            a_Block = SmoothQuartzStairs::SmoothQuartzStairs(BlockFace, Inverse ? SmoothQuartzStairs::Half::Top : SmoothQuartzStairs::Half::Bottom, SmoothQuartzStairs::Shape::Straight); break;
			case BlockType::SmoothRedSandstoneStairs:      a_Block = SmoothRedSandstoneStairs::SmoothRedSandstoneStairs(BlockFace, Inverse ? SmoothRedSandstoneStairs::Half::Top : SmoothRedSandstoneStairs::Half::Bottom, SmoothRedSandstoneStairs::Shape::Straight); break;
			case BlockType::SmoothSandstoneStairs:         a_Block = SmoothSandstoneStairs::SmoothSandstoneStairs(BlockFace, Inverse ? SmoothSandstoneStairs::Half::Top : SmoothSandstoneStairs::Half::Bottom, SmoothSandstoneStairs::Shape::Straight); break;
			case BlockType::SpruceStairs:                  a_Block = SpruceStairs::SpruceStairs(BlockFace, Inverse ? SpruceStairs::Half::Top : SpruceStairs::Half::Bottom, SpruceStairs::Shape::Straight); break;
			case BlockType::StoneBrickStairs:              a_Block = StoneBrickStairs::StoneBrickStairs(BlockFace, Inverse ? StoneBrickStairs::Half::Top : StoneBrickStairs::Half::Bottom, StoneBrickStairs::Shape::Straight); break;
			case BlockType::StoneStairs:                   a_Block = StoneStairs::StoneStairs(BlockFace, Inverse ? StoneStairs::Half::Top : StoneStairs::Half::Bottom, StoneStairs::Shape::Straight); break;
			case BlockType::WarpedStairs:                  a_Block = WarpedStairs::WarpedStairs(BlockFace, Inverse ? WarpedStairs::Half::Top : WarpedStairs::Half::Bottom, WarpedStairs::Shape::Straight); break;
			default: return false;
		}
		return true;
	}





	virtual BlockState MirrorXZ(BlockState a_Block) const override
	{
		// TODO(12xx12)
		return a_Block;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaStairs:                  return 15;
			case BlockType::AndesiteStairs:                return 11;
			case BlockType::BirchStairs:                   return 2;
			case BlockType::BlackstoneStairs:              return 29;
			case BlockType::BrickStairs:                   return 28;
			case BlockType::CobblestoneStairs:             return 11;
			case BlockType::CrimsonStairs:                 return 35;
			case BlockType::DarkOakStairs:                 return 26;
			case BlockType::DarkPrismarineStairs:          return 31;
			case BlockType::DioriteStairs:                 return 14;
			case BlockType::EndStoneBrickStairs:           return 2;
			case BlockType::GraniteStairs:                 return 10;
			case BlockType::JungleStairs:                  return 10;
			case BlockType::MossyCobblestoneStairs:        return 11;
			case BlockType::MossyStoneBrickStairs:         return 11;
			case BlockType::NetherBrickStairs:             return 35;
			case BlockType::OakStairs:                     return 13;
			case BlockType::PolishedAndesiteStairs:        return 11;
			case BlockType::PolishedBlackstoneBrickStairs: return 29;
			case BlockType::PolishedBlackstoneStairs:      return 29;
			case BlockType::PolishedDioriteStairs:         return 11;
			case BlockType::PolishedGraniteStairs:         return 11;
			case BlockType::PrismarineBrickStairs:         return 23;
			case BlockType::PrismarineStairs:              return 23;
			case BlockType::QuartzStairs:                  return 8;
			case BlockType::PurpurStairs:                  return 16;
			case BlockType::RedNetherBrickStairs:          return 35;
			case BlockType::RedSandstoneStairs:            return 10;
			case BlockType::SandstoneStairs:               return 2;
			case BlockType::SmoothQuartzStairs:            return 8;
			case BlockType::SmoothRedSandstoneStairs:      return 10;
			case BlockType::SmoothSandstoneStairs:         return 2;
			case BlockType::SpruceStairs:                  return 34;
			case BlockType::StoneBrickStairs:              return 11;
			case BlockType::StoneStairs:                   return 11;
			case BlockType::WarpedStairs:                  return 23;
			default:
			{
				ASSERT(!"Unhandled blocktype in stairs handler!");
				return 0;
			}
		}
	}





	/** EXCEPTION a.k.a. why is this removed:
	This collision-detection is actually more accurate than the client, but since the client itself
	sends inaccurate / sparse data, it's easier to just err on the side of the client and keep the
	two in sync by assuming that if a player hit ANY of the stair's bounding cube, it counts as the ground. */
	#if 0
	bool IsInsideBlock(Vector3d a_RelPosition, const BLOCKTYPE a_BlockType, const NIBBLETYPE a_BlockMeta)
	{
		if (a_BlockMeta & 0x4)  // upside down
		{
			return true;
		}
		else if ((a_BlockMeta & 0x3) == 0)  // tall side is east (+X)
		{
			return (a_RelPosition.y < ((a_RelPosition.x > 0.5) ? 1.0 : 0.5));
		}
		else if ((a_BlockMeta & 0x3) == 1)  // tall side is west (-X)
		{
			return (a_RelPosition.y < ((a_RelPosition.x < 0.5) ? 1.0 : 0.5));
		}
		else if ((a_BlockMeta & 0x3) == 2)  // tall side is south (+Z)
		{
			return (a_RelPosition.y < ((a_RelPosition.z > 0.5) ? 1.0 : 0.5));
		}
		else if ((a_BlockMeta & 0x3) == 3)  // tall side is north (-Z)
		{
			return (a_RelPosition.y < ((a_RelPosition.z < 0.5) ? 1.0 : 0.5));
		}
		return false;
	}
	#endif

} ;




