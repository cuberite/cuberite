
#pragma once

#include "ItemHandler.h"
#include "SimplePlaceableItemHandler.h"
#include "Blocks/BlockBed.h"
#include "BlockEntities/BedEntity.h"





class cItemBedHandler final :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		const auto Rotation = RotationToBlockFace(a_Player.GetYaw());
		const auto HeadPosition = a_PlacePosition + cBlockBedHandler::BlockFaceToDirection(Rotation);
		auto & World = *a_Player.GetWorld();
		BlockState BlockToReplace;
		if (World.GetBlock(a_PlacePosition, BlockToReplace))
		{
			return false;
		}

		// Vanilla only allows beds to be placed into air.
		// Check if there is empty space for the "head" block:
		if (!cBlockHandler::For(World.GetBlock(HeadPosition).Type()).DoesIgnoreBuildCollision(World, a_HeldItem, HeadPosition, BlockToReplace, a_ClickedBlockFace, false))
		{
			return false;
		}
#define PLACE_BED(BedType)\
	Success = a_Player.PlaceBlocks({\
		{ a_PlacePosition, BedType::BedType(Rotation, false, BedType::Part::Foot) }, \
		{ HeadPosition,    BedType::BedType(Rotation, false, BedType::Part::Head) }\
	});\
	break;\

		using namespace Block;
		bool Success = false;

		switch (a_Player.GetEquippedItem().m_ItemType)
		{
			case Item::BlackBed: PLACE_BED(BlackBed)
			case Item::WhiteBed: PLACE_BED(WhiteBed)
			case Item::LightGrayBed: PLACE_BED(LightGrayBed)
			case Item::GrayBed: PLACE_BED(GrayBed)
			case Item::BrownBed: PLACE_BED(BrownBed)
			case Item::RedBed: PLACE_BED(RedBed)
			case Item::OrangeBed: PLACE_BED(OrangeBed)
			case Item::YellowBed: PLACE_BED(YellowBed)
			case Item::LimeBed: PLACE_BED(LimeBed)
			case Item::GreenBed: PLACE_BED(GreenBed)
			case Item::CyanBed: PLACE_BED(CyanBed)
			case Item::LightBlueBed: PLACE_BED(LightBlueBed)
			case Item::BlueBed: PLACE_BED(BlueBed)
			case Item::PurpleBed: PLACE_BED(PurpleBed)
			case Item::MagentaBed: PLACE_BED(MagentaBed)
			case Item::PinkBed: PLACE_BED(PinkBed)
			default: return false;
		}

		if (!Success)
		{
			return false;
		}

		auto SetColor = [&a_HeldItem](cBlockEntity & a_BlockEntity)
		{
			ASSERT(cBlockBedHandler::IsBlockBed(a_BlockEntity.GetBlock()));

			static_cast<cBedEntity &>(a_BlockEntity).SetColor(a_HeldItem.m_ItemDamage);  // Broken
			return false;
		};
		World.DoWithBlockEntityAt(a_PlacePosition, SetColor);
		World.DoWithBlockEntityAt(HeadPosition, SetColor);

		return true;
	}
};
