
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockBed.h"
#include "BlockEntities/BedEntity.h"





class cItemBedHandler final :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		const auto Rotation = RotationToBlockFace(a_Player.GetYaw());
		const auto HeadPosition = a_PlacePosition + cBlockBedHandler::BlockFaceToDirection(Rotation);
		auto & World = *a_Player.GetWorld();
		auto BlockToReplace = World.GetBlock(a_PlacePosition);

		// Vanilla only allows beds to be placed into air.
		// Check if there is empty space for the "head" block:
		if (!cBlockHandler::For(World.GetBlock(HeadPosition).Type()).DoesIgnoreBuildCollision(World, a_HeldItem, HeadPosition, BlockToReplace, a_ClickedBlockFace, false))
		{
			return false;
		}

		using namespace Block;
		bool Success = false;

		switch (a_Player.GetEquippedItem().m_ItemDamage)
		{
			// White
			case 0:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, WhiteBed::WhiteBed(Rotation, false, WhiteBed::Part::Foot) },
					{ HeadPosition,    WhiteBed::WhiteBed(Rotation, false, WhiteBed::Part::Head) }
				});
				break;
			}
			// Orange
			case 1:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, OrangeBed::OrangeBed(Rotation, false, OrangeBed::Part::Foot) },
					{ HeadPosition,    OrangeBed::OrangeBed(Rotation, false, OrangeBed::Part::Head) }
				});
				break;
			}
			// Magenta
			case 2:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, MagentaBed::MagentaBed(Rotation, false, MagentaBed::Part::Foot) },
					{ HeadPosition,    MagentaBed::MagentaBed(Rotation, false, MagentaBed::Part::Head) }
				});
				break;
			}
			// LightBlue
			case 3:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, LightBlueBed::LightBlueBed(Rotation, false, LightBlueBed::Part::Foot) },
					{ HeadPosition,    LightBlueBed::LightBlueBed(Rotation, false, LightBlueBed::Part::Head) }
				});
				break;
			}
			// Yellow
			case 4:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, YellowBed::YellowBed(Rotation, false, YellowBed::Part::Foot) },
					{ HeadPosition,    YellowBed::YellowBed(Rotation, false, YellowBed::Part::Head) }
				});
				break;
			}
			// Lime
			case 5:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, LimeBed::LimeBed(Rotation, false, LimeBed::Part::Foot) },
					{ HeadPosition,    LimeBed::LimeBed(Rotation, false, LimeBed::Part::Head) }
				});
				break;
			}
			// Pink
			case 6:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, PinkBed::PinkBed(Rotation, false, PinkBed::Part::Foot) },
					{ HeadPosition,    PinkBed::PinkBed(Rotation, false, PinkBed::Part::Head) }
				});
				break;
			}
			// Gray
			case 7:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, GrayBed::GrayBed(Rotation, false, GrayBed::Part::Foot) },
					{ HeadPosition,    GrayBed::GrayBed(Rotation, false, GrayBed::Part::Head) }
				});
				break;
			}
			// Light Gray
			case 8:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, LightGrayBed::LightGrayBed(Rotation, false, LightGrayBed::Part::Foot) },
					{ HeadPosition,    LightGrayBed::LightGrayBed(Rotation, false, LightGrayBed::Part::Head) }
				});
				break;
			}
			// Cyan
			case 9:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, CyanBed::CyanBed(Rotation, false, CyanBed::Part::Foot) },
					{ HeadPosition,    CyanBed::CyanBed(Rotation, false, CyanBed::Part::Head) }
				});
				break;
			}
			// Purple
			case 10:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, PurpleBed::PurpleBed(Rotation, false, PurpleBed::Part::Foot) },
					{ HeadPosition,    PurpleBed::PurpleBed(Rotation, false, PurpleBed::Part::Head) }
				});
				break;
			}
			// Blue
			case 11:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, BlueBed::BlueBed(Rotation, false, BlueBed::Part::Foot) },
					{ HeadPosition,    BlueBed::BlueBed(Rotation, false, BlueBed::Part::Head) }
				});
				break;
			}
			// Brown
			case 12:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, BrownBed::BrownBed(Rotation, false, BrownBed::Part::Foot) },
					{ HeadPosition,    BrownBed::BrownBed(Rotation, false, BrownBed::Part::Head) }
				});
				break;
			}
			// Green
			case 13:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, GreenBed::GreenBed(Rotation, false, GreenBed::Part::Foot) },
					{ HeadPosition,    GreenBed::GreenBed(Rotation, false, GreenBed::Part::Head) }
				});
				break;
			}
			// Red
			case 14:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, RedBed::RedBed(Rotation, false, RedBed::Part::Foot) },
					{ HeadPosition,    RedBed::RedBed(Rotation, false, RedBed::Part::Head) }
				});
				break;
			}
			// Black
			case 15:
			{
				Success = a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, BlackBed::BlackBed(Rotation, false, BlackBed::Part::Foot) },
					{ HeadPosition,    BlackBed::BlackBed(Rotation, false, BlackBed::Part::Head) }
				});
				break;
			}
			default: return false;
		}

		if (!Success)
		{
			return false;
		}

		auto SetColor = [&a_HeldItem](cBlockEntity & a_BlockEntity)
		{
			ASSERT(cBlockBedHandler::IsBlockBed(a_BlockEntity.GetBlock()));

			static_cast<cBedEntity &>(a_BlockEntity).SetColor(a_HeldItem.m_ItemDamage);
			return false;
		};
		World.DoWithBlockEntityAt(a_PlacePosition, SetColor);
		World.DoWithBlockEntityAt(HeadPosition, SetColor);

		return true;
	}
};
