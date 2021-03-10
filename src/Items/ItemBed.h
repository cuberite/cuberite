
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockBed.h"





class cItemBedHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBedHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}


	virtual bool GetBlocksToPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		sSetBlockVector & a_BlocksToPlace
	) override
	{
		const auto Rotation = RotationToBlockFace(a_Player.GetYaw());
		const auto HeadPosition = a_PlacedBlockPos + cBlockBedHandler::BlockFaceToDirection(Rotation);

		// Vanilla only allows beds to be placed into air
		// Check if there is empty space for the "head" block:
		if (a_World.GetBlock(HeadPosition) != Block::Air::Air())
		{
			return false;
		}

		using namespace Block;

		switch (a_Player.GetEquippedItem().m_ItemDamage)
		{
			// White
			case 0:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, WhiteBed::WhiteBed(Rotation, false, WhiteBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     WhiteBed::WhiteBed(Rotation, false, WhiteBed::Part::Head));
				break;
			}
			// Orange
			case 1:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, OrangeBed::OrangeBed(Rotation, false, OrangeBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     OrangeBed::OrangeBed(Rotation, false, OrangeBed::Part::Head));
				break;
			}
			// Magenta
			case 2:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, MagentaBed::MagentaBed(Rotation, false, MagentaBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     MagentaBed::MagentaBed(Rotation, false, MagentaBed::Part::Head));
				break;
			}
			// LightBlue
			case 3:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, LightBlueBed::LightBlueBed(Rotation, false, LightBlueBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     LightBlueBed::LightBlueBed(Rotation, false, LightBlueBed::Part::Head));
				break;
			}
			// Yellow
			case 4:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, YellowBed::YellowBed(Rotation, false, YellowBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     YellowBed::YellowBed(Rotation, false, YellowBed::Part::Head));
				break;
			}
			// Lime
			case 5:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, LimeBed::LimeBed(Rotation, false, LimeBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     LimeBed::LimeBed(Rotation, false, LimeBed::Part::Head));
				break;
			}
			// Pink
			case 6:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, PinkBed::PinkBed(Rotation, false, PinkBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     PinkBed::PinkBed(Rotation, false, PinkBed::Part::Head));
				break;
			}
			// Gray
			case 7:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, GrayBed::GrayBed(Rotation, false, GrayBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     GrayBed::GrayBed(Rotation, false, GrayBed::Part::Head));
				break;
			}
			// Light Gray
			case 8:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, LightGrayBed::LightGrayBed(Rotation, false, LightGrayBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     LightGrayBed::LightGrayBed(Rotation, false, LightGrayBed::Part::Head));
				break;
			}
			// Cyan
			case 9:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, CyanBed::CyanBed(Rotation, false, CyanBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     CyanBed::CyanBed(Rotation, false, CyanBed::Part::Head));
				break;
			}
			// Purple
			case 10:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, PurpleBed::PurpleBed(Rotation, false, PurpleBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     PurpleBed::PurpleBed(Rotation, false, PurpleBed::Part::Head));
				break;
			}
			// Blue
			case 11:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, BlueBed::BlueBed(Rotation, false, BlueBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     BlueBed::BlueBed(Rotation, false, BlueBed::Part::Head));
				break;
			}
			// Brown
			case 12:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, BrownBed::BrownBed(Rotation, false, BrownBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     BrownBed::BrownBed(Rotation, false, BrownBed::Part::Head));
				break;
			}
			// Green
			case 13:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, GreenBed::GreenBed(Rotation, false, GreenBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     GreenBed::GreenBed(Rotation, false, GreenBed::Part::Head));
				break;
			}
			// Red
			case 14:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, RedBed::RedBed(Rotation, false, RedBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     RedBed::RedBed(Rotation, false, RedBed::Part::Head));
				break;
			}
			case 15:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, BlackBed::BlackBed(Rotation, false, BlackBed::Part::Foot));
				a_BlocksToPlace.emplace_back(HeadPosition,     BlackBed::BlackBed(Rotation, false, BlackBed::Part::Head));
				break;
			}
			default: return false;
		}
		return true;
	}
};
