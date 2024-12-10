
// BlockStandingBanner.h

#pragma once

#include "BlockHandler.h"
#include "../BlockInfo.h"
#include "BlockEntity.h"
#include "Mixins/SolidSurfaceUnderneath.h"





class cBlockStandingBannerHandler final :
	public cSolidSurfaceUnderneath<cBlockEntityHandler>
{
	using Super = cSolidSurfaceUnderneath<cBlockEntityHandler>;

public:
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




	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		if (a_Position.y < 1)
		{
			return false;
		}

		return cBlockInfo::IsSolid(a_Chunk.GetBlock(a_Position.addedY(-1)));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
