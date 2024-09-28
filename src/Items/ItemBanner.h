

#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../BlockEntities/BannerEntity.h"
#include "../Blocks/ChunkInterface.h"
#include "../Blocks/BlockHandler.h"
#include "../Blocks/BlockBanner.h"





class cItemBannerHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		// Cannot place a banner at "no face" and from the bottom:
		if ((a_ClickedBlockFace == BLOCK_FACE_NONE) || (a_ClickedBlockFace == BLOCK_FACE_BOTTOM))
		{
			return false;
		}

		if (!TryPlaceBanner(a_Player, a_PlacePosition, a_ClickedBlockFace))
		{
			return false;
		}

		a_Player.GetWorld()->DoWithBlockEntityAt(a_PlacePosition, [&a_HeldItem](cBlockEntity & BlockToPlaceEntity)
		{
			ASSERT(cBlockBannerHandler::IsBlockBanner(BlockToPlaceEntity.GetBlock()));

			static_cast<cBannerEntity &>(BlockToPlaceEntity).SetBaseColor(static_cast<unsigned char>(a_HeldItem.m_ItemDamage));
			return false;
		});

		return true;
	}





	virtual bool IsPlaceable(void) const override
	{
		return true;
	}





	static bool TryPlaceBanner(cPlayer & a_Player, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace)
	{
		using namespace Block;
		BlockState BlockToPlace;
		switch (a_ClickedBlockFace)
		{
			// Placing on the floor
			case BLOCK_FACE_YP:
			{
				auto Rotation = static_cast<unsigned char>(RotationToBlockFace(a_Player.GetYaw()));
				switch (a_Player.GetEquippedItem().m_ItemDamage)
				{
					case E_META_BANNER_BLACK:      BlockToPlace = BlackBanner::BlackBanner(Rotation);         break;
					case E_META_BANNER_RED:        BlockToPlace = RedBanner::RedBanner(Rotation);             break;
					case E_META_BANNER_GREEN:      BlockToPlace = GreenBanner::GreenBanner(Rotation);         break;
					case E_META_BANNER_BROWN:      BlockToPlace = BrownBanner::BrownBanner(Rotation);         break;
					case E_META_BANNER_BLUE:       BlockToPlace = BlueBanner::BlueBanner(Rotation);           break;
					case E_META_BANNER_PURPLE:     BlockToPlace = PurpleBanner::PurpleBanner(Rotation);       break;
					case E_META_BANNER_CYAN:       BlockToPlace = CyanBanner::CyanBanner(Rotation);           break;
					case E_META_BANNER_LIGHTGRAY:  BlockToPlace = LightGrayBanner::LightGrayBanner(Rotation); break;
					case E_META_BANNER_GRAY:       BlockToPlace = GrayBanner::GrayBanner(Rotation);           break;
					case E_META_BANNER_PINK:       BlockToPlace = PinkBanner::PinkBanner(Rotation);           break;
					case E_META_BANNER_LIGHTGREEN: BlockToPlace = LimeBanner::LimeBanner(Rotation);           break;
					case E_META_BANNER_YELLOW:     BlockToPlace = YellowBanner::YellowBanner(Rotation);       break;
					case E_META_BANNER_LIGHTBLUE:  BlockToPlace = LightBlueBanner::LightBlueBanner(Rotation); break;
					case E_META_BANNER_MAGENTA:    BlockToPlace = MagentaBanner::MagentaBanner(Rotation);     break;
					case E_META_BANNER_ORANGE:     BlockToPlace = OrangeBanner::OrangeBanner(Rotation);       break;
					case E_META_BANNER_WHITE:      BlockToPlace = WhiteBanner::WhiteBanner(Rotation);         break;
				}
				break;
			}
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				// placing on the sides
				auto Facing = RotationToBlockFace(a_Player.GetYaw());
				switch (a_Player.GetEquippedItem().m_ItemDamage)
				{
					case E_META_BANNER_BLACK:      BlockToPlace = BlackWallBanner::BlackWallBanner(Facing);         break;
					case E_META_BANNER_RED:        BlockToPlace = RedWallBanner::RedWallBanner(Facing);             break;
					case E_META_BANNER_GREEN:      BlockToPlace = GreenWallBanner::GreenWallBanner(Facing);         break;
					case E_META_BANNER_BROWN:      BlockToPlace = BrownWallBanner::BrownWallBanner(Facing);         break;
					case E_META_BANNER_BLUE:       BlockToPlace = BlueWallBanner::BlueWallBanner(Facing);           break;
					case E_META_BANNER_PURPLE:     BlockToPlace = PurpleWallBanner::PurpleWallBanner(Facing);       break;
					case E_META_BANNER_CYAN:       BlockToPlace = CyanWallBanner::CyanWallBanner(Facing);           break;
					case E_META_BANNER_LIGHTGRAY:  BlockToPlace = LightGrayWallBanner::LightGrayWallBanner(Facing); break;
					case E_META_BANNER_GRAY:       BlockToPlace = GrayWallBanner::GrayWallBanner(Facing);           break;
					case E_META_BANNER_PINK:       BlockToPlace = PinkWallBanner::PinkWallBanner(Facing);           break;
					case E_META_BANNER_LIGHTGREEN: BlockToPlace = LimeWallBanner::LimeWallBanner(Facing);           break;
					case E_META_BANNER_YELLOW:     BlockToPlace = YellowWallBanner::YellowWallBanner(Facing);       break;
					case E_META_BANNER_LIGHTBLUE:  BlockToPlace = LightBlueWallBanner::LightBlueWallBanner(Facing); break;
					case E_META_BANNER_MAGENTA:    BlockToPlace = MagentaWallBanner::MagentaWallBanner(Facing);     break;
					case E_META_BANNER_ORANGE:     BlockToPlace = OrangeWallBanner::OrangeWallBanner(Facing);       break;
					case E_META_BANNER_WHITE:      BlockToPlace = WhiteWallBanner::WhiteWallBanner(Facing);         break;
				}
				break;
			}
			default: return false;
		}
		a_Player.PlaceBlock(a_PlacePosition, BlockToPlace);
		return true;
	}
};
