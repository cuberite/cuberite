

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





	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockMeta = 0x00;
		const double Rotation = a_Player->GetYaw();

		// Placing on the floor
		if (a_ClickedBlockFace == BLOCK_FACE_TOP)
		{
			if ((Rotation >= - 11.25f) && (Rotation < 11.25f))
			{
				// South
				a_BlockMeta |= 0x08;
			}
			else if ((Rotation >= 11.25f) && (Rotation < 33.75f))
			{
				// SouthSouthWest
				a_BlockMeta |= 0x09;
			}
			else if ((Rotation >= 23.75f) && (Rotation < 56.25f))
			{
				// SouthWest
				a_BlockMeta |= 0x0a;
			}
			else if ((Rotation >= 56.25f) && (Rotation < 78.75f))
			{
				// WestSouthWest
				a_BlockMeta |= 0x0b;
			}
			else if ((Rotation >= 78.75f) && (Rotation < 101.25f))
			{
				// West
				a_BlockMeta |= 0x0c;
			}
			else if ((Rotation >= 101.25f) && (Rotation < 123.75f))
			{
				// WestNorthWest
				a_BlockMeta |= 0x0d;
			}
			else if ((Rotation >= 123.75f) && (Rotation < 146.25f))
			{
				// NorthWest
				a_BlockMeta |= 0x0e;
			}
			else if ((Rotation >= 146.25f) && (Rotation < 168.75f))
			{
				// NorthNorthWest
				a_BlockMeta |= 0x0f;
			}
			else if ((Rotation >= -168.75f) && (Rotation < -146.25f))
			{
				// NorthNorthEast
				a_BlockMeta |= 0x01;
			}
			else if ((Rotation >= -146.25f) && (Rotation < -123.75f))
			{
				// NorthEast
				a_BlockMeta |= 0x02;
			}
			else if ((Rotation >= -123.75f) && (Rotation < -101.25f))
			{
				// EastNorthEast
				a_BlockMeta |= 0x03;
			}
			else if ((Rotation >= -101.25) && (Rotation < -78.75f))
			{
				// East
				a_BlockMeta |= 0x04;
			}
			else if ((Rotation >= -78.75) && (Rotation < -56.25f))
			{
				// EastSouthEast
				a_BlockMeta |= 0x05;
			}
			else if ((Rotation >= -56.25f) && (Rotation < -33.75f))
			{
				// SouthEast
				a_BlockMeta |= 0x06;
			}
			else if ((Rotation >= -33.75f) && (Rotation < -11.25f))
			{
				// SouthSouthEast
				a_BlockMeta |= 0x07;
			}
			else  // degrees jumping from 180 to -180
			{
				// North
				a_BlockMeta |= 0x00;
			}
			a_BlockType = E_BLOCK_STANDING_BANNER;
		}
		// placing on the sides
		else if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			if (a_ClickedBlockFace == BLOCK_FACE_EAST)
			{
				a_BlockMeta |= 0x05;
			}
			else if (a_ClickedBlockFace == BLOCK_FACE_WEST)
			{
				a_BlockMeta |= 0x04;
			}
			else if (a_ClickedBlockFace == BLOCK_FACE_NORTH)
			{
				a_BlockMeta |= 0x02;
			}
			else  // degrees jumping from 180 to -180
			{
				a_BlockMeta |= 0x03;
			}
			a_BlockType = E_BLOCK_WALL_BANNER;
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
		// Cannot place a banner at "no face" and from the bottom:
		if ((a_ClickedBlockFace == BLOCK_FACE_NONE) || (a_ClickedBlockFace == BLOCK_FACE_BOTTOM))
		{
			return true;
		}

		// Checks if the banner replaced the block
		BLOCKTYPE ClickedBlockType;
		NIBBLETYPE ClickedBlockMeta;
		a_World.GetBlockTypeMeta(a_ClickedBlockPos, ClickedBlockType, ClickedBlockMeta);
		cChunkInterface ChunkInterface(a_World.GetChunkMap());
		bool IsReplacingClickedBlock = cBlockHandler::For(ClickedBlockType).DoesIgnoreBuildCollision(ChunkInterface, a_ClickedBlockPos, a_Player, ClickedBlockMeta);
		if (IsReplacingClickedBlock)
		{
			// TODO: There is a bug in the network which prevents the client from receiving the new block entity on placement
			// For now the replaced blocks are disabled
			return false;
		}

		// saving the color of the banner in case it's the players last one
		NIBBLETYPE Color = static_cast<NIBBLETYPE>(a_EquippedItem.m_ItemDamage);

		if (!Super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_ClickedBlockPos, a_ClickedBlockFace, a_ClickedBlockPos))
		{
			return false;
		}

		const auto BannerPos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		a_World.DoWithBlockEntityAt(BannerPos, [Color](cBlockEntity & a_BlockEntity)
		{
			ASSERT((a_BlockEntity.GetBlockType() == E_BLOCK_STANDING_BANNER) || (a_BlockEntity.GetBlockType() == E_BLOCK_WALL_BANNER));

			auto & Banner = static_cast<cBannerEntity &>(a_BlockEntity);
			Banner.SetBaseColor(Color);
			return false;
		});

		return true;
	}
};
