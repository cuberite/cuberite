

#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../BlockEntities/BannerEntity.h"
#include "../Blocks/ChunkInterface.h"
#include "../Blocks/BlockHandler.h"
#include "../Blocks/BlockStandingBanner.h"





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
			ASSERT(cBlockStandingBannerHandler::IsBlockBanner(BlockToPlaceEntity.GetBlock()));

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
				auto Rotation = RotationToFineFace(a_Player.GetYaw());
				switch (a_Player.GetEquippedItem().m_ItemType)
				{
					case Item::BlackBanner:      BlockToPlace = BlackBanner::BlackBanner(Rotation);         break;
					case Item::RedBanner:        BlockToPlace = RedBanner::RedBanner(Rotation);             break;
					case Item::GreenBanner:      BlockToPlace = GreenBanner::GreenBanner(Rotation);         break;
					case Item::BrownBanner:      BlockToPlace = BrownBanner::BrownBanner(Rotation);         break;
					case Item::BlueBanner:       BlockToPlace = BlueBanner::BlueBanner(Rotation);           break;
					case Item::PurpleBanner:     BlockToPlace = PurpleBanner::PurpleBanner(Rotation);       break;
					case Item::CyanBanner:       BlockToPlace = CyanBanner::CyanBanner(Rotation);           break;
					case Item::LightGrayBanner:  BlockToPlace = LightGrayBanner::LightGrayBanner(Rotation); break;
					case Item::GrayBanner:       BlockToPlace = GrayBanner::GrayBanner(Rotation);           break;
					case Item::PinkBanner:       BlockToPlace = PinkBanner::PinkBanner(Rotation);           break;
					case Item::LimeBanner:       BlockToPlace = LimeBanner::LimeBanner(Rotation);           break;
					case Item::YellowBanner:     BlockToPlace = YellowBanner::YellowBanner(Rotation);       break;
					case Item::LightBlueBanner:  BlockToPlace = LightBlueBanner::LightBlueBanner(Rotation); break;
					case Item::MagentaBanner:    BlockToPlace = MagentaBanner::MagentaBanner(Rotation);     break;
					case Item::OrangeBanner:     BlockToPlace = OrangeBanner::OrangeBanner(Rotation);       break;
					case Item::WhiteBanner:      BlockToPlace = WhiteBanner::WhiteBanner(Rotation);         break;
				}
				break;
			}
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				// placing on the sides
				auto Facing = a_ClickedBlockFace;//RotationToBlockFace(a_Player.GetYaw());
				switch (a_Player.GetEquippedItem().m_ItemType)
				{
					case Item::BlackBanner:      BlockToPlace = BlackWallBanner::BlackWallBanner(Facing);         break;
					case Item::RedBanner:        BlockToPlace = RedWallBanner::RedWallBanner(Facing);             break;
					case Item::GreenBanner:      BlockToPlace = GreenWallBanner::GreenWallBanner(Facing);         break;
					case Item::BrownBanner:      BlockToPlace = BrownWallBanner::BrownWallBanner(Facing);         break;
					case Item::BlueBanner:       BlockToPlace = BlueWallBanner::BlueWallBanner(Facing);           break;
					case Item::PurpleBanner:     BlockToPlace = PurpleWallBanner::PurpleWallBanner(Facing);       break;
					case Item::CyanBanner:       BlockToPlace = CyanWallBanner::CyanWallBanner(Facing);           break;
					case Item::LightGrayBanner:  BlockToPlace = LightGrayWallBanner::LightGrayWallBanner(Facing); break;
					case Item::GrayBanner:       BlockToPlace = GrayWallBanner::GrayWallBanner(Facing);           break;
					case Item::PinkBanner:       BlockToPlace = PinkWallBanner::PinkWallBanner(Facing);           break;
					case Item::LimeBanner:       BlockToPlace = LimeWallBanner::LimeWallBanner(Facing);           break;
					case Item::YellowBanner:     BlockToPlace = YellowWallBanner::YellowWallBanner(Facing);       break;
					case Item::LightBlueBanner:  BlockToPlace = LightBlueWallBanner::LightBlueWallBanner(Facing); break;
					case Item::MagentaBanner:    BlockToPlace = MagentaWallBanner::MagentaWallBanner(Facing);     break;
					case Item::OrangeBanner:     BlockToPlace = OrangeWallBanner::OrangeWallBanner(Facing);       break;
					case Item::WhiteBanner:      BlockToPlace = WhiteWallBanner::WhiteWallBanner(Facing);         break;
				}
				break;
			}
			default: return false;
		}
		a_Player.PlaceBlock(a_PlacePosition, BlockToPlace);
		return true;
	}
};
