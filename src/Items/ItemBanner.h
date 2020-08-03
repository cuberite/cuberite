

#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockBanner.h"





class cItemBannerHandler: public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBannerHandler(int a_ItemType): Super(a_ItemType)
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
		/*
			may have 16 colors
			can be placed in item frame - just shows the item model
			If renamed retains its name on adding and removing pattern
			keeps its name on building and breaking

			Banners can be copied using blank banner
			may be crated together with shield to display the banner

			can be used for 1.5 items as fuel
		*/

		NIBBLETYPE BlockMeta = 0x00;
		LOG(std::to_string(a_EquippedItem.m_ItemDamage));
		double Rotation = a_Player.GetYaw();
		if (a_ClickedBlockFace == BLOCK_FACE_BOTTOM)
		{
			return false;
		}
		// Placing on the floor
		else if (a_ClickedBlockFace == BLOCK_FACE_TOP)
		{
			if ((Rotation >= - 11.25f) && (Rotation < 11.25f))
			{
				// South
				BlockMeta |= 0x00;
			}
			else if ((Rotation >= 11.25f) && (Rotation < 33.75f))
			{
				// SouthSouthWest
				BlockMeta |= 0x01;
			}
			else if ((Rotation >= 23.75f) && (Rotation < 56.25f))
			{
				// SouthWest
				BlockMeta |= 0x02;
			}
			else if ((Rotation >= 56.25f) && (Rotation < 78.75f))
			{
				// WestSouthWest
				BlockMeta |= 0x03;
			}
			else if ((Rotation >= 78.75f) && (Rotation < 101.25f))
			{
				// West
				BlockMeta |= 0x04;
			}
			else if ((Rotation >= 101.25f) && (Rotation < 123.75f))
			{
				// WestNorthWest
				BlockMeta |= 0x05;
			}
			else if ((Rotation >= 123.75f) && (Rotation < 146.25f))
			{
				// NorthWest
				BlockMeta |= 0x06;
			}
			else if ((Rotation >= 146.25f) && (Rotation < 168.75f))
			{
				// NorthNorthWest
				BlockMeta |= 0x07;
			}
			else if ((Rotation >= -168.75f) && (Rotation < -146.25f))
			{
				// NorthNorthEast
				BlockMeta |= 0x09;
			}
			else if ((Rotation >= -146.25f) && (Rotation < -123.75f))
			{
				// NorthEast
				BlockMeta |= 0x0a;
			}
			else if ((Rotation >= -123.75f) && (Rotation < -101.25f))
			{
				// EastNorthEast
				BlockMeta |= 0x0b;
			}
			else if ((Rotation >= -101.25) && (Rotation < -78.75f))
			{
				// East
				BlockMeta |= 0x0c;
			}
			else if ((Rotation >= -78.75) && (Rotation < -56.25f))
			{
				// EastSouthEast
				BlockMeta |= 0x0d;
			}
			else if ((Rotation >= -56.25f) && (Rotation < -33.75f))
			{
				// SouthEast
				BlockMeta |= 0x0e;
			}
			else if ((Rotation >= -33.75f) && (Rotation < -11.25f))
			{
				// SouthSouthEast
				BlockMeta |= 0x0f;
			}
			else  // degrees jumping from 180 to -180
			{
				// North
				BlockMeta |= 0x08;
			}
			a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_STANDING_BANNER, BlockMeta);
		}
		// placing on the sides
		else if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			NIBBLETYPE BlockMeta = 0x00;
			if ((Rotation >= -135) && (Rotation < -45))
			{
				BlockMeta |= 0x00;
			}
			else if ((Rotation >= -45) && (Rotation < 45))
			{
				BlockMeta |= 0x04;
			}
			else if ((Rotation >= 45) && (Rotation < 135))
			{
				BlockMeta |= 0x08;
			}
			else  // degrees jumping from 180 to -180
			{
				BlockMeta |= 0x0ab;
			}
			a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_WALL_BANNER, BlockMeta);
		}
		else
		{
			return false;
		}
		return true;
	}
};
