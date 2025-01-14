
#pragma once

#include "BlockHandler.h"
#include "Mixins/Mixins.h"




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
			case BlockType::BambooMosaicStairs:
			case BlockType::BambooStairs:
			case BlockType::CherryStairs:
			case BlockType::CobbledDeepslateStairs:
			case BlockType::TuffStairs:
			case BlockType::TuffBrickStairs:
			case BlockType::PolishedTuffStairs:
			case BlockType::MudBrickStairs:
			case BlockType::MangroveStairs:
			case BlockType::DeepslateTileStairs:
			case BlockType::DeepslateBrickStairs:
			case BlockType::PolishedDeepslateStairs:
			case BlockType::CutCopperStairs:
			case BlockType::ExposedCutCopperStairs:
			case BlockType::WeatheredCutCopperStairs:
			case BlockType::OxidizedCutCopperStairs:
			case BlockType::WaxedCutCopperStairs:
			case BlockType::WaxedExposedCutCopperStairs:
			case BlockType::WaxedWeatheredCutCopperStairs:
			case BlockType::WaxedOxidizedCutCopperStairs:
			case BlockType::PaleOakStairs:
			case BlockType::ResinBrickStairs:
				return true;
			default: return false;
		}
	}

#define IS_STAIR_TOP_HALF(StairType) \
	return StairType::Half(a_Block) == StairType::Half::Top;

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
			case BlockType::BambooMosaicStairs:            IS_STAIR_TOP_HALF(BambooMosaicStairs)
			case BlockType::BambooStairs:                  IS_STAIR_TOP_HALF(BambooStairs)
			case BlockType::CherryStairs:                  IS_STAIR_TOP_HALF(BambooMosaicStairs)
			case BlockType::CobbledDeepslateStairs:        IS_STAIR_TOP_HALF(CobbledDeepslateStairs)
			case BlockType::TuffStairs:                    IS_STAIR_TOP_HALF(TuffStairs)
			case BlockType::TuffBrickStairs:               IS_STAIR_TOP_HALF(TuffBrickStairs)
			case BlockType::PolishedTuffStairs:            IS_STAIR_TOP_HALF(PolishedTuffStairs)
			case BlockType::MudBrickStairs:                IS_STAIR_TOP_HALF(MudBrickStairs)
			case BlockType::MangroveStairs:                IS_STAIR_TOP_HALF(MangroveStairs)
			case BlockType::DeepslateTileStairs:           IS_STAIR_TOP_HALF(DeepslateTileStairs)
			case BlockType::DeepslateBrickStairs:          IS_STAIR_TOP_HALF(DeepslateBrickStairs)
			case BlockType::PolishedDeepslateStairs:       IS_STAIR_TOP_HALF(PolishedDeepslateStairs)
			case BlockType::CutCopperStairs:               IS_STAIR_TOP_HALF(CutCopperStairs)
			case BlockType::ExposedCutCopperStairs:        IS_STAIR_TOP_HALF(ExposedCutCopperStairs)
			case BlockType::WeatheredCutCopperStairs:      IS_STAIR_TOP_HALF(WeatheredCutCopperStairs)
			case BlockType::OxidizedCutCopperStairs:       IS_STAIR_TOP_HALF(OxidizedCutCopperStairs)
			case BlockType::WaxedCutCopperStairs:          IS_STAIR_TOP_HALF(WaxedCutCopperStairs)
			case BlockType::WaxedExposedCutCopperStairs:   IS_STAIR_TOP_HALF(WaxedExposedCutCopperStairs)
			case BlockType::WaxedWeatheredCutCopperStairs: IS_STAIR_TOP_HALF(WaxedWeatheredCutCopperStairs)
			case BlockType::WaxedOxidizedCutCopperStairs:  IS_STAIR_TOP_HALF(WaxedOxidizedCutCopperStairs)
			case BlockType::PaleOakStairs:                 IS_STAIR_TOP_HALF(PaleOakStairs)
			case BlockType::ResinBrickStairs:              IS_STAIR_TOP_HALF(ResinBrickStairs)
			default: return false;
		}
	}
	// TODO: get rid of this function
	static inline bool IsAnyStairType(BlockState a_Block)
	{
		return IsBlockStairs(a_Block);
	}

	static inline ENUM_BLOCK_META GetRotation(BlockState a_Block)
	{
		UNREACHABLE("unimplemented");
	}

	static inline bool IsStairsUpsideDown(BlockState a_Block)
	{
		UNREACHABLE("unimplemented");
	}

private:

	// TODO(12xx12)
	/*
	virtual BlockState MirrorXZ(BlockState a_Block) const override
	{

		return a_Block;
	}
	*/

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
			case BlockType::BambooMosaicStairs:
			case BlockType::BambooStairs:
			case BlockType::CherryStairs:
			case BlockType::CobbledDeepslateStairs:
			case BlockType::TuffStairs:
			case BlockType::TuffBrickStairs:
			case BlockType::PolishedTuffStairs:
			case BlockType::MudBrickStairs:
			case BlockType::MangroveStairs:
			case BlockType::DeepslateTileStairs:
			case BlockType::DeepslateBrickStairs:
			case BlockType::PolishedDeepslateStairs:
			case BlockType::CutCopperStairs:
			case BlockType::ExposedCutCopperStairs:
			case BlockType::WeatheredCutCopperStairs:
			case BlockType::OxidizedCutCopperStairs:
			case BlockType::WaxedCutCopperStairs:
			case BlockType::WaxedExposedCutCopperStairs:
			case BlockType::WaxedWeatheredCutCopperStairs:
			case BlockType::WaxedOxidizedCutCopperStairs:
			case BlockType::PaleOakStairs:
			case BlockType::ResinBrickStairs: return 0;
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




