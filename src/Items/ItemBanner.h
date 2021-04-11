

#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockEntities/BannerEntity.h"
#include "../Blocks/ChunkInterface.h"





class cItemBannerHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBannerHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}

private:

	virtual bool IsPlaceable(void) override
	{
		return true;
	}




	static bool TryPlaceBanner(cPlayer & a_Player, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace)
	{
		const auto Rotation = a_Player.GetYaw();

		// Placing on the floor:
		if (a_ClickedBlockFace == BLOCK_FACE_TOP)
		{
			NIBBLETYPE Meta;

			if ((Rotation >= -11.25f) && (Rotation < 11.25f))
			{
				// South
				Meta = 0x08;
			}
			else if ((Rotation >= 11.25f) && (Rotation < 33.75f))
			{
				// SouthSouthWest
				Meta = 0x09;
			}
			else if ((Rotation >= 23.75f) && (Rotation < 56.25f))
			{
				// SouthWest
				Meta = 0x0a;
			}
			else if ((Rotation >= 56.25f) && (Rotation < 78.75f))
			{
				// WestSouthWest
				Meta = 0x0b;
			}
			else if ((Rotation >= 78.75f) && (Rotation < 101.25f))
			{
				// West
				Meta = 0x0c;
			}
			else if ((Rotation >= 101.25f) && (Rotation < 123.75f))
			{
				// WestNorthWest
				Meta = 0x0d;
			}
			else if ((Rotation >= 123.75f) && (Rotation < 146.25f))
			{
				// NorthWest
				Meta = 0x0e;
			}
			else if ((Rotation >= 146.25f) && (Rotation < 168.75f))
			{
				// NorthNorthWest
				Meta = 0x0f;
			}
			else if ((Rotation >= -168.75f) && (Rotation < -146.25f))
			{
				// NorthNorthEast
				Meta = 0x01;
			}
			else if ((Rotation >= -146.25f) && (Rotation < -123.75f))
			{
				// NorthEast
				Meta = 0x02;
			}
			else if ((Rotation >= -123.75f) && (Rotation < -101.25f))
			{
				// EastNorthEast
				Meta = 0x03;
			}
			else if ((Rotation >= -101.25) && (Rotation < -78.75f))
			{
				// East
				Meta = 0x04;
			}
			else if ((Rotation >= -78.75) && (Rotation < -56.25f))
			{
				// EastSouthEast
				Meta = 0x05;
			}
			else if ((Rotation >= -56.25f) && (Rotation < -33.75f))
			{
				// SouthEast
				Meta = 0x06;
			}
			else if ((Rotation >= -33.75f) && (Rotation < -11.25f))
			{
				// SouthSouthEast
				Meta = 0x07;
			}
			else  // degrees jumping from 180 to -180
			{
				// North
				Meta = 0x00;
			}

			return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_STANDING_BANNER, Meta);
		}

		// We must be placing on the side of a block.

		NIBBLETYPE Meta;

		if (a_ClickedBlockFace == BLOCK_FACE_EAST)
		{
			Meta = 0x05;
		}
		else if (a_ClickedBlockFace == BLOCK_FACE_WEST)
		{
			Meta = 0x04;
		}
		else if (a_ClickedBlockFace == BLOCK_FACE_NORTH)
		{
			Meta = 0x02;
		}
		else  // degrees jumping from 180 to -180
		{
			Meta = 0x03;
		}

		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_WALL_BANNER, Meta);
	}





	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
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

		a_Player.GetWorld()->DoWithBlockEntityAt(a_PlacePosition, [&a_HeldItem](cBlockEntity & a_BlockEntity)
		{
			ASSERT((a_BlockEntity.GetBlockType() == E_BLOCK_STANDING_BANNER) || (a_BlockEntity.GetBlockType() == E_BLOCK_WALL_BANNER));

			static_cast<cBannerEntity &>(a_BlockEntity).SetBaseColor(static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage));
			return false;
		});

		return true;
	}
};
