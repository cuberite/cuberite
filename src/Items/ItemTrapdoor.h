
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockTrapdoor.h"
#include "Registries/BlockItemConverter.h"
#include "Protocol/Palettes/Upgrade.h"





class cItemTrapdoorHandler final  :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		bool IsBottom = false;
		eBlockFace DestFacing = BLOCK_FACE_NONE;
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_NONE: return false;

				// Clicked one of the sides
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				if (a_CursorPosition.y < 8)
				{
					// Trapdoor is placed on a higher half of a vertical block.
					IsBottom = true;
				}
				DestFacing = MirrorBlockFaceY(a_ClickedBlockFace);
				break;
			}
			case BLOCK_FACE_YM:
			{
				IsBottom = true;
				DestFacing = RotationToBlockFace(a_Player.GetYaw());
				break;
			}
			case BLOCK_FACE_YP: break;
		}

		if (DestFacing == BLOCK_FACE_NONE)
		{
			return false;
		}

		BlockState BlockToPlace;

		using namespace Block;
		switch (BlockItemConverter::FromItem(a_HeldItem.m_ItemType))
		{
			case BlockType::AcaciaTrapdoor:
			{
				BlockToPlace = AcaciaTrapdoor::AcaciaTrapdoor(
					DestFacing,
					IsBottom ? AcaciaTrapdoor::Half::Bottom : AcaciaTrapdoor::Half::Top,
					false, false);
				break;
			}
			case BlockType::BirchTrapdoor:
			{
				BlockToPlace = BirchTrapdoor::BirchTrapdoor(
					DestFacing,
					IsBottom ? BirchTrapdoor::Half::Bottom : BirchTrapdoor::Half::Top,
					false, false);
				break;
			}
			case BlockType::CrimsonTrapdoor:
			{
				BlockToPlace = CrimsonTrapdoor::CrimsonTrapdoor(
					DestFacing,
					IsBottom ? CrimsonTrapdoor::Half::Bottom : CrimsonTrapdoor::Half::Top,
					false, false);
				break;
			}
			case BlockType::DarkOakTrapdoor:
			{
				BlockToPlace = DarkOakTrapdoor::DarkOakTrapdoor(
					DestFacing,
					IsBottom ? DarkOakTrapdoor::Half::Bottom : DarkOakTrapdoor::Half::Top,
					false, false);
				break;
			}
			case BlockType::IronTrapdoor:
			{
				BlockToPlace = IronTrapdoor::IronTrapdoor(
					DestFacing,
					IsBottom ? IronTrapdoor::Half::Bottom : IronTrapdoor::Half::Top,
					false, false);
				break;
			}
			case BlockType::JungleTrapdoor:
			{
				BlockToPlace = JungleTrapdoor::JungleTrapdoor(
					DestFacing,
					IsBottom ? JungleTrapdoor::Half::Bottom : JungleTrapdoor::Half::Top,
					false, false);
				break;
			}
			case BlockType::OakTrapdoor:
			{
				BlockToPlace = OakTrapdoor::OakTrapdoor(
					DestFacing,
					IsBottom ? OakTrapdoor::Half::Bottom : OakTrapdoor::Half::Top,
					false, false);
				break;
			}
			case BlockType::SpruceTrapdoor:
			{
				BlockToPlace = SpruceTrapdoor::SpruceTrapdoor(
					DestFacing,
					IsBottom ? SpruceTrapdoor::Half::Bottom : SpruceTrapdoor::Half::Top,
					false, false);
				break;
			}
			case BlockType::WarpedTrapdoor:
			{
				BlockToPlace = WarpedTrapdoor::WarpedTrapdoor(
					DestFacing,
					IsBottom ? WarpedTrapdoor::Half::Bottom : WarpedTrapdoor::Half::Top,
					false, false);
				break;
			}
			default: return false;
		}

		return a_Player.PlaceBlock(a_PlacePosition, BlockToPlace);
	}
};
