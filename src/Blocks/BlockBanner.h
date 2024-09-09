
// BlockBanner.h

#pragma once

#include "Blocks/BlockHandler.h"

#include "Blocks/BlockEntity.h"
#include "BlockInfo.h"





class cBlockBannerHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

	static constexpr unsigned char ROTATION_MAX = 16;
	static constexpr unsigned char ROTATION_STEP = 1;
	static constexpr unsigned char ROTATION_MIRROR = 8;
public:

	using Super::Super;

	static inline bool IsBlockBanner(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::BlackBanner:
			case BlockType::BlueBanner:
			case BlockType::BrownBanner:
			case BlockType::CyanBanner:
			case BlockType::GrayBanner:
			case BlockType::GreenBanner:
			case BlockType::LightBlueBanner:
			case BlockType::LightGrayBanner:
			case BlockType::LimeBanner:
			case BlockType::MagentaBanner:
			case BlockType::OrangeBanner:
			case BlockType::PinkBanner:
			case BlockType::PurpleBanner:
			case BlockType::RedBanner:
			case BlockType::WhiteBanner:
			case BlockType::YellowBanner:

			case BlockType::BlackWallBanner:
			case BlockType::BlueWallBanner:
			case BlockType::BrownWallBanner:
			case BlockType::CyanWallBanner:
			case BlockType::GrayWallBanner:
			case BlockType::GreenWallBanner:
			case BlockType::LightBlueWallBanner:
			case BlockType::LightGrayWallBanner:
			case BlockType::LimeWallBanner:
			case BlockType::MagentaWallBanner:
			case BlockType::OrangeWallBanner:
			case BlockType::PinkWallBanner:
			case BlockType::PurpleWallBanner:
			case BlockType::RedWallBanner:
			case BlockType::WhiteWallBanner:
			case BlockType::YellowWallBanner:
				return true;
			default: return false;
		}
	}

	static inline bool IsBannerStanding(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::BlackBanner:
			case BlockType::BlueBanner:
			case BlockType::BrownBanner:
			case BlockType::CyanBanner:
			case BlockType::GrayBanner:
			case BlockType::GreenBanner:
			case BlockType::LightBlueBanner:
			case BlockType::LightGrayBanner:
			case BlockType::LimeBanner:
			case BlockType::MagentaBanner:
			case BlockType::OrangeBanner:
			case BlockType::PinkBanner:
			case BlockType::PurpleBanner:
			case BlockType::RedBanner:
			case BlockType::WhiteBanner:
			case BlockType::YellowBanner:
				return true;
			default:
			{
				return false;
			}
		}
	}

	/** Get rotation from a standing banner. Don't call this on a wall banner. */
	static inline unsigned char GetRotation(BlockState a_Block)
	{
		ASSERT(IsBannerStanding(a_Block));
		switch (a_Block.Type())
		{
			case BlockType::BlackBanner:         return Block::BlackBanner::Rotation(a_Block);
			case BlockType::BlueBanner:          return Block::BlueBanner::Rotation(a_Block);
			case BlockType::BrownBanner:         return Block::BrownBanner::Rotation(a_Block);
			case BlockType::CyanBanner:          return Block::CyanBanner::Rotation(a_Block);
			case BlockType::GrayBanner:          return Block::GrayBanner::Rotation(a_Block);
			case BlockType::GreenBanner:         return Block::GreenBanner::Rotation(a_Block);
			case BlockType::LightBlueBanner:     return Block::LightBlueBanner::Rotation(a_Block);
			case BlockType::LightGrayBanner:     return Block::LightGrayBanner::Rotation(a_Block);
			case BlockType::LimeBanner:          return Block::LimeBanner::Rotation(a_Block);
			case BlockType::MagentaBanner:       return Block::MagentaBanner::Rotation(a_Block);
			case BlockType::OrangeBanner:        return Block::OrangeBanner::Rotation(a_Block);
			case BlockType::PinkBanner:          return Block::PinkBanner::Rotation(a_Block);
			case BlockType::PurpleBanner:        return Block::PurpleBanner::Rotation(a_Block);
			case BlockType::RedBanner:           return Block::RedBanner::Rotation(a_Block);
			case BlockType::WhiteBanner:         return Block::WhiteBanner::Rotation(a_Block);
			case BlockType::YellowBanner:        return Block::YellowBanner::Rotation(a_Block);
			default: return 0;
		}
	}

	/** Get Facing from a hanging banner. Don't call this on a standing banner. */
	static inline eBlockFace GetFacing(BlockState a_Block)
	{
		ASSERT(!IsBannerStanding(a_Block));
		switch (a_Block.Type())
		{
			case BlockType::BlackWallBanner:     return Block::BlackWallBanner::Facing(a_Block);
			case BlockType::BlueWallBanner:      return Block::BlueWallBanner::Facing(a_Block);
			case BlockType::BrownWallBanner:     return Block::BrownWallBanner::Facing(a_Block);
			case BlockType::CyanWallBanner:      return Block::CyanWallBanner::Facing(a_Block);
			case BlockType::GrayWallBanner:      return Block::GrayWallBanner::Facing(a_Block);
			case BlockType::GreenWallBanner:     return Block::GreenWallBanner::Facing(a_Block);
			case BlockType::LightBlueWallBanner: return Block::LightBlueWallBanner::Facing(a_Block);
			case BlockType::LightGrayWallBanner: return Block::LightGrayWallBanner::Facing(a_Block);
			case BlockType::LimeWallBanner:      return Block::LimeWallBanner::Facing(a_Block);
			case BlockType::MagentaWallBanner:   return Block::MagentaWallBanner::Facing(a_Block);
			case BlockType::OrangeWallBanner:    return Block::OrangeWallBanner::Facing(a_Block);
			case BlockType::PinkWallBanner:      return Block::PinkWallBanner::Facing(a_Block);
			case BlockType::PurpleWallBanner:    return Block::PurpleWallBanner::Facing(a_Block);
			case BlockType::RedWallBanner:       return Block::RedWallBanner::Facing(a_Block);
			case BlockType::WhiteWallBanner:     return Block::WhiteWallBanner::Facing(a_Block);
			case BlockType::YellowWallBanner:    return Block::YellowWallBanner::Facing(a_Block);
			default: return BLOCK_FACE_NONE;
		}
	}

	/** Rotates a given block meta counter-clockwise. */
	BlockState RotateCCW(BlockState a_Block) const
	{
		if (IsBannerStanding(a_Block))
		{
			switch (a_Block.Type())
			{
				// Todo: Is this the behaviour we want?
				// Rotation is done in 16 steps. If the rotation needs to be changed, change the constant ROTATION_STEP.
				case BlockType::BlackBanner:     return Block::BlackBanner::BlackBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::BlueBanner:      return Block::BlueBanner::BlueBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::BrownBanner:     return Block::BrownBanner::BrownBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::CyanBanner:      return Block::CyanBanner::CyanBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::GrayBanner:      return Block::GrayBanner::GrayBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::GreenBanner:     return Block::GreenBanner::GreenBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::LightBlueBanner: return Block::LightBlueBanner::LightBlueBanner(static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::LightGrayBanner: return Block::LightGrayBanner::LightGrayBanner(static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::LimeBanner:      return Block::LimeBanner::LimeBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::MagentaBanner:   return Block::MagentaBanner::MagentaBanner(    static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::OrangeBanner:    return Block::OrangeBanner::OrangeBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::PinkBanner:      return Block::PinkBanner::PinkBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::PurpleBanner:    return Block::PurpleBanner::PurpleBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::RedBanner:       return Block::RedBanner::RedBanner(            static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::WhiteBanner:     return Block::WhiteBanner::WhiteBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				case BlockType::YellowBanner:    return Block::YellowBanner::YellowBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
				default: return a_Block;
			}
		}

		auto Facing = RotateBlockFaceCCW(GetFacing(a_Block));
		switch (a_Block.Type())
		{
			case BlockType::BlackWallBanner:     return Block::BlackWallBanner::BlackWallBanner(Facing);
			case BlockType::BlueWallBanner:      return Block::BlueWallBanner::BlueWallBanner(Facing);
			case BlockType::BrownWallBanner:     return Block::BrownWallBanner::BrownWallBanner(Facing);
			case BlockType::CyanWallBanner:      return Block::CyanWallBanner::CyanWallBanner(Facing);
			case BlockType::GrayWallBanner:      return Block::GrayWallBanner::GrayWallBanner(Facing);
			case BlockType::GreenWallBanner:     return Block::GreenWallBanner::GreenWallBanner(Facing);
			case BlockType::LightBlueWallBanner: return Block::LightBlueWallBanner::LightBlueWallBanner(Facing);
			case BlockType::LightGrayWallBanner: return Block::LightGrayWallBanner::LightGrayWallBanner(Facing);
			case BlockType::LimeWallBanner:      return Block::LimeWallBanner::LimeWallBanner(Facing);
			case BlockType::MagentaWallBanner:   return Block::MagentaWallBanner::MagentaWallBanner(Facing);
			case BlockType::OrangeWallBanner:    return Block::OrangeWallBanner::OrangeWallBanner(Facing);
			case BlockType::PinkWallBanner:      return Block::PinkWallBanner::PinkWallBanner(Facing);
			case BlockType::PurpleWallBanner:    return Block::PurpleWallBanner::PurpleWallBanner(Facing);
			case BlockType::RedWallBanner:       return Block::RedWallBanner::RedWallBanner(Facing);
			case BlockType::WhiteWallBanner:     return Block::WhiteWallBanner::WhiteWallBanner(Facing);
			case BlockType::YellowWallBanner:    return Block::YellowWallBanner::YellowWallBanner(Facing);
			default: return a_Block;
		}
	}

	/** Rotates a given block meta clockwise. */
	BlockState RotateCW(BlockState a_Block) const
	{
		using namespace Block;
		if (IsBannerStanding(a_Block))
		{
			switch (a_Block.Type())
			{
				// Todo: Is this the behaviour we want?
				// Rotation is done in 16 steps. If the rotation needs to be changed, change the constant ROTATION_STEP.
				case BlockType::BlackBanner:     return BlackBanner::BlackBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::BlueBanner:      return BlueBanner::BlueBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::BrownBanner:     return BrownBanner::BrownBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::CyanBanner:      return CyanBanner::CyanBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::GrayBanner:      return GrayBanner::GrayBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::GreenBanner:     return GreenBanner::GreenBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::LightBlueBanner: return LightBlueBanner::LightBlueBanner(static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::LightGrayBanner: return LightGrayBanner::LightGrayBanner(static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::LimeBanner:      return LimeBanner::LimeBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::MagentaBanner:   return MagentaBanner::MagentaBanner(    static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::OrangeBanner:    return OrangeBanner::OrangeBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::PinkBanner:      return PinkBanner::PinkBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::PurpleBanner:    return PurpleBanner::PurpleBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::RedBanner:       return RedBanner::RedBanner(            static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::WhiteBanner:     return WhiteBanner::WhiteBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				case BlockType::YellowBanner:    return YellowBanner::YellowBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
				default: return a_Block;
			}
		}

		auto Facing = RotateBlockFaceCW(GetFacing(a_Block));
		switch (a_Block.Type())
		{
			case BlockType::BlackWallBanner:     return BlackWallBanner::BlackWallBanner(Facing);
			case BlockType::BlueWallBanner:      return BlueWallBanner::BlueWallBanner(Facing);
			case BlockType::BrownWallBanner:     return BrownWallBanner::BrownWallBanner(Facing);
			case BlockType::CyanWallBanner:      return CyanWallBanner::CyanWallBanner(Facing);
			case BlockType::GrayWallBanner:      return GrayWallBanner::GrayWallBanner(Facing);
			case BlockType::GreenWallBanner:     return GreenWallBanner::GreenWallBanner(Facing);
			case BlockType::LightBlueWallBanner: return LightBlueWallBanner::LightBlueWallBanner(Facing);
			case BlockType::LightGrayWallBanner: return LightGrayWallBanner::LightGrayWallBanner(Facing);
			case BlockType::LimeWallBanner:      return LimeWallBanner::LimeWallBanner(Facing);
			case BlockType::MagentaWallBanner:   return MagentaWallBanner::MagentaWallBanner(Facing);
			case BlockType::OrangeWallBanner:    return OrangeWallBanner::OrangeWallBanner(Facing);
			case BlockType::PinkWallBanner:      return PinkWallBanner::PinkWallBanner(Facing);
			case BlockType::PurpleWallBanner:    return PurpleWallBanner::PurpleWallBanner(Facing);
			case BlockType::RedWallBanner:       return RedWallBanner::RedWallBanner(Facing);
			case BlockType::WhiteWallBanner:     return WhiteWallBanner::WhiteWallBanner(Facing);
			case BlockType::YellowWallBanner:    return YellowWallBanner::YellowWallBanner(Facing);
			default: return a_Block;
		}
	}

	/** Mirrors a given block around the XY plane. */
	BlockState MirrorXY(BlockState a_Block) const
	{
		using namespace Block;
		if (IsBannerStanding(a_Block))
		{
			auto Rotation = GetRotation(a_Block);
			// If the banner is aligned with the X axis, it can't be mirrored:
			if ((Rotation == 0) || (Rotation == 8))
			{
				return a_Block;
			}
			switch (a_Block.Type())
			{
				case BlockType::BlackBanner:     return BlackBanner::BlackBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::BlueBanner:      return BlueBanner::BlueBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::BrownBanner:     return BrownBanner::BrownBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::CyanBanner:      return CyanBanner::CyanBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::GrayBanner:      return GrayBanner::GrayBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::GreenBanner:     return GreenBanner::GreenBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::LightBlueBanner: return LightBlueBanner::LightBlueBanner(static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::LightGrayBanner: return LightGrayBanner::LightGrayBanner(static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::LimeBanner:      return LimeBanner::LimeBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::MagentaBanner:   return MagentaBanner::MagentaBanner(    static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::OrangeBanner:    return OrangeBanner::OrangeBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::PinkBanner:      return PinkBanner::PinkBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::PurpleBanner:    return PurpleBanner::PurpleBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::RedBanner:       return RedBanner::RedBanner(            static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::WhiteBanner:     return WhiteBanner::WhiteBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::YellowBanner:    return YellowBanner::YellowBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				default: return a_Block;
			}
		}

		switch (a_Block.Type())
		{
			case BlockType::BlackWallBanner:     return BlackWallBanner::BlackWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::BlueWallBanner:      return BlueWallBanner::BlueWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::BrownWallBanner:     return BrownWallBanner::BrownWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::CyanWallBanner:      return CyanWallBanner::CyanWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::GrayWallBanner:      return GrayWallBanner::GrayWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::GreenWallBanner:     return GreenWallBanner::GreenWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::LightBlueWallBanner: return LightBlueWallBanner::LightBlueWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::LightGrayWallBanner: return LightGrayWallBanner::LightGrayWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::LimeWallBanner:      return LimeWallBanner::LimeWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::MagentaWallBanner:   return MagentaWallBanner::MagentaWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::OrangeWallBanner:    return OrangeWallBanner::OrangeWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::PinkWallBanner:      return PinkWallBanner::PinkWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::PurpleWallBanner:    return PurpleWallBanner::PurpleWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::RedWallBanner:       return RedWallBanner::RedWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::WhiteWallBanner:     return WhiteWallBanner::WhiteWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			case BlockType::YellowWallBanner:    return YellowWallBanner::YellowWallBanner(MirrorBlockFaceXY(GetFacing(a_Block)));
			default: return a_Block;
		}
	}

	/** Mirrors a given block meta around the YZ plane. */
	BlockState MirrorYZ(BlockState a_Block) const
	{
		using namespace Block;
		if (IsBannerStanding(a_Block))
		{
			auto Rotation = GetRotation(a_Block);
			// If the banner is aligned with the Z axis, it can't be mirrored:
			if ((Rotation == 4) || (Rotation == 12))
			{
				return a_Block;
			}
			switch (a_Block.Type())
			{
				case BlockType::BlackBanner:     return BlackBanner::BlackBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::BlueBanner:      return BlueBanner::BlueBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::BrownBanner:     return BrownBanner::BrownBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::CyanBanner:      return CyanBanner::CyanBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::GrayBanner:      return GrayBanner::GrayBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::GreenBanner:     return GreenBanner::GreenBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::LightBlueBanner: return LightBlueBanner::LightBlueBanner(static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::LightGrayBanner: return LightGrayBanner::LightGrayBanner(static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::LimeBanner:      return LimeBanner::LimeBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::MagentaBanner:   return MagentaBanner::MagentaBanner(    static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::OrangeBanner:    return OrangeBanner::OrangeBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::PinkBanner:      return PinkBanner::PinkBanner(          static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::PurpleBanner:    return PurpleBanner::PurpleBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::RedBanner:       return RedBanner::RedBanner(            static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::WhiteBanner:     return WhiteBanner::WhiteBanner(        static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				case BlockType::YellowBanner:    return YellowBanner::YellowBanner(      static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
				default: return a_Block;
			}
		}

		switch (a_Block.Type())
		{
			case BlockType::BlackWallBanner:     return BlackWallBanner::BlackWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::BlueWallBanner:      return BlueWallBanner::BlueWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::BrownWallBanner:     return BrownWallBanner::BrownWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::CyanWallBanner:      return CyanWallBanner::CyanWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::GrayWallBanner:      return GrayWallBanner::GrayWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::GreenWallBanner:     return GreenWallBanner::GreenWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::LightBlueWallBanner: return LightBlueWallBanner::LightBlueWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::LightGrayWallBanner: return LightGrayWallBanner::LightGrayWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::LimeWallBanner:      return LimeWallBanner::LimeWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::MagentaWallBanner:   return MagentaWallBanner::MagentaWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::OrangeWallBanner:    return OrangeWallBanner::OrangeWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::PinkWallBanner:      return PinkWallBanner::PinkWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::PurpleWallBanner:    return PurpleWallBanner::PurpleWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::RedWallBanner:       return RedWallBanner::RedWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::WhiteWallBanner:     return WhiteWallBanner::WhiteWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			case BlockType::YellowWallBanner:    return YellowWallBanner::YellowWallBanner(MirrorBlockFaceYZ(GetFacing(a_Block)));
			default: return a_Block;
		}

	}


	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		if (a_Position.y < 1)
		{
			return false;
		}

		return cBlockInfo::IsSolid(a_Chunk.GetBlock(a_Position.addedY(-1)));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;
