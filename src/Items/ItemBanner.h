

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
			can be placed in item frame - just shows the item model
			If renamed retains its name on adding and removing pattern
			keeps its name on building and breaking

			Banners can be copied using blank banner
			may be crated together with shield to display the banner

			can be used for 1.5 items as fuel
		*/

		NIBBLETYPE BlockMeta = 0x00;
		double Rotation = a_Player.GetYaw();
		// Placing on the floor
		if (a_ClickedBlockFace == BLOCK_FACE_TOP)
		{
			if ((Rotation >= - 11.25f) && (Rotation < 11.25f))
			{
				// South
				BlockMeta |= 0x08;
			}
			else if ((Rotation >= 11.25f) && (Rotation < 33.75f))
			{
				// SouthSouthWest
				BlockMeta |= 0x09;
			}
			else if ((Rotation >= 23.75f) && (Rotation < 56.25f))
			{
				// SouthWest
				BlockMeta |= 0x0a;
			}
			else if ((Rotation >= 56.25f) && (Rotation < 78.75f))
			{
				// WestSouthWest
				BlockMeta |= 0x0b;
			}
			else if ((Rotation >= 78.75f) && (Rotation < 101.25f))
			{
				// West
				BlockMeta |= 0x0c;
			}
			else if ((Rotation >= 101.25f) && (Rotation < 123.75f))
			{
				// WestNorthWest
				BlockMeta |= 0x0d;
			}
			else if ((Rotation >= 123.75f) && (Rotation < 146.25f))
			{
				// NorthWest
				BlockMeta |= 0x0e;
			}
			else if ((Rotation >= 146.25f) && (Rotation < 168.75f))
			{
				// NorthNorthWest
				BlockMeta |= 0x0f;
			}
			else if ((Rotation >= -168.75f) && (Rotation < -146.25f))
			{
				// NorthNorthEast
				BlockMeta |= 0x01;
			}
			else if ((Rotation >= -146.25f) && (Rotation < -123.75f))
			{
				// NorthEast
				BlockMeta |= 0x02;
			}
			else if ((Rotation >= -123.75f) && (Rotation < -101.25f))
			{
				// EastNorthEast
				BlockMeta |= 0x03;
			}
			else if ((Rotation >= -101.25) && (Rotation < -78.75f))
			{
				// East
				BlockMeta |= 0x04;
			}
			else if ((Rotation >= -78.75) && (Rotation < -56.25f))
			{
				// EastSouthEast
				BlockMeta |= 0x05;
			}
			else if ((Rotation >= -56.25f) && (Rotation < -33.75f))
			{
				// SouthEast
				BlockMeta |= 0x06;
			}
			else if ((Rotation >= -33.75f) && (Rotation < -11.25f))
			{
				// SouthSouthEast
				BlockMeta |= 0x07;
			}
			else  // degrees jumping from 180 to -180
			{
				// North
				BlockMeta |= 0x00;
			}
			a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_STANDING_BANNER, BlockMeta);
		}
		// placing on the sides
		else if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			if (a_ClickedBlockFace == BLOCK_FACE_EAST)
			{
				BlockMeta |= 0x05;
			}
			else if (a_ClickedBlockFace == BLOCK_FACE_WEST)
			{
				BlockMeta |= 0x04;
			}
			else if (a_ClickedBlockFace == BLOCK_FACE_NORTH)
			{
				BlockMeta |= 0x02;
			}
			else  // degrees jumping from 180 to -180
			{
				BlockMeta |= 0x03;
			}
			a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_WALL_BANNER, BlockMeta);
		}
		else
		{
			return false;
		}
		return true;
	}





	virtual bool OnPlayerPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) override
	{
		cItem ItemCopy(a_EquippedItem);  // Make a copy in case this is the player's last banner item and OnPlayerPlace removes it

		// Cannot place a banner at "no face" and from the bottom:
		if ((a_ClickedBlockFace == BLOCK_FACE_NONE) || (a_ClickedBlockFace == BLOCK_FACE_BOTTOM))
		{
			return true;
		}
		if (!Super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_ClickedBlockPos, a_ClickedBlockFace, a_ClickedBlockPos))
		{
			return false;
		}
		// Checks if the banner replaced the block
		BLOCKTYPE ClickedBlockType;
		NIBBLETYPE ClickedBlockMeta;
		a_World.GetBlockTypeMeta(a_ClickedBlockPos, ClickedBlockType, ClickedBlockMeta);
		cChunkInterface ChunkInterface(a_World.GetChunkMap());
		bool IsReplacingClickedBlock = BlockHandler(ClickedBlockType)->DoesIgnoreBuildCollision(ChunkInterface, a_ClickedBlockPos, a_Player, ClickedBlockMeta);
		auto BannerPos = IsReplacingClickedBlock ? a_ClickedBlockPos : AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		return PlaceBannerEntity(a_World, a_Player, ItemCopy, BannerPos);
	}




	/** Tries to place a Banner BlockEntity to the given position */
	bool PlaceBannerEntity(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		Vector3i a_PlacePos
		)
	{
		return a_World.DoWithBannerAt(a_PlacePos.x, a_PlacePos.y, a_PlacePos.z, [&](cBannerEntity & a_BlockEntity)
			{
				if (!((a_BlockEntity.GetBlockType() == E_BLOCK_STANDING_BANNER) || (a_BlockEntity.GetBlockType() == E_BLOCK_WALL_BANNER)))
				{
					return false;
				}
				a_BlockEntity.SetBaseColor(static_cast<short>(a_EquippedItem.m_ItemDamage));
				a_BlockEntity.GetWorld()->BroadcastBlockEntity(a_BlockEntity.GetPos());
				return true;
			}
		);
	}
};
