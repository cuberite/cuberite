
#pragma once

#include "BlockHandler.h"
#include "../Entities/Player.h"
#include "Chunk.h"
#include "MetaRotator.h"




class cBlockDoorHandler :
	public cMetaRotator<cBlockHandler, 0x03, 0x01, 0x02, 0x03, 0x00, true>
{
	typedef cMetaRotator<cBlockHandler, 0x03, 0x01, 0x02, 0x03, 0x00, true> super;
public:
	cBlockDoorHandler(BLOCKTYPE a_BlockType);

	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override;
	virtual const char * GetStepSound(void) override;

	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override;
	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta)  override;
	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta)  override;
	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta)  override;

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// If clicking a bottom face, place the door one block lower:
		if (a_BlockFace == BLOCK_FACE_BOTTOM)
		{
			a_BlockY--;
		}

		if (
			!CanReplaceBlock(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ)) ||
			!CanReplaceBlock(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ))
		)
		{
			return false;
		}
		
		a_BlockType = m_BlockType;
		a_BlockMeta = PlayerYawToMetaData(a_Player->GetYaw());
		return true;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem((m_BlockType == E_BLOCK_WOODEN_DOOR) ? E_ITEM_WOODEN_DOOR : E_ITEM_IRON_DOOR, 1, 0));
	}


	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override;


	virtual bool IsUseable(void) override
	{
		return true;
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR));
	}


	bool CanReplaceBlock(BLOCKTYPE a_BlockType)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_AIR:
			case E_BLOCK_TALL_GRASS:
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER:
			case E_BLOCK_LAVA:
			case E_BLOCK_STATIONARY_LAVA:
			case E_BLOCK_SNOW:
			case E_BLOCK_FIRE:
			{
				return true;
			}
		}
		return false;
	}


	/** Converts the player's yaw to placed door's blockmeta */
	inline static NIBBLETYPE PlayerYawToMetaData(double a_Yaw)
	{
		ASSERT((a_Yaw >= -180) && (a_Yaw < 180));
		
		a_Yaw += 90 + 45;
		if (a_Yaw > 360)
		{
			a_Yaw -= 360;
		}
		if ((a_Yaw >= 0) && (a_Yaw < 90))
		{
			return 0x00;
		}
		else if ((a_Yaw >= 180) && (a_Yaw < 270))
		{
			return 0x02;
		}
		else if ((a_Yaw >= 90) && (a_Yaw < 180))
		{
			return 0x01;
		}
		else
		{
			return 0x03;
		}
	}


	/** Returns true if the specified blocktype is any kind of door */
	inline static bool IsDoor(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_WOODEN_DOOR) || (a_Block == E_BLOCK_IRON_DOOR);
	}


	static NIBBLETYPE IsOpen(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		NIBBLETYPE Meta = GetCompleteDoorMeta(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
		return ((Meta & 0x04) != 0);
	}


	/** Returns the complete meta composed from the both parts of the door as (TopMeta << 4) | BottomMeta
	The coords may point to either part of the door.
	The returned value has bit 3 (0x08) set iff the coords point to the top part of the door.
	Fails gracefully for (invalid) doors on the world's top and bottom. */
	static NIBBLETYPE GetCompleteDoorMeta(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

		if ((Meta & 0x08) != 0)
		{
			// The coords are pointing at the top part of the door
			if (a_BlockX > 0)
			{
				NIBBLETYPE DownMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ);
				return (NIBBLETYPE) ((DownMeta & 0x07) | 0x08 | (Meta << 4));
			}
			// This is the top part of the door at the bottommost layer of the world, there's no bottom:
			return (NIBBLETYPE) (0x08 | (Meta << 4));
		}
		else
		{
			// The coords are pointing at the bottom part of the door
			if (a_BlockY < cChunkDef::Height - 1)
			{
				NIBBLETYPE UpMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY + 1, a_BlockZ);
				return (NIBBLETYPE) (Meta | (UpMeta << 4));
			}
			// This is the bottom part of the door at the topmost layer of the world, there's no top:
			return Meta;
		}
	}


	/** Sets the door to the specified state. If the door is already in that state, does nothing. */
	static void SetOpen(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_Open)
	{
		BLOCKTYPE Block = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		if (!IsDoor(Block))
		{
			return;
		}

		NIBBLETYPE Meta = GetCompleteDoorMeta(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
		bool IsOpened = ((Meta & 0x04) != 0);
		if (IsOpened == a_Open)
		{
			return;
		}

		// Change the door
		NIBBLETYPE NewMeta = (Meta & 0x07) ^ 0x04;  // Flip the "IsOpen" bit (0x04)
		if ((Meta & 0x08) == 0)
		{
			// The block is the bottom part of the door
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, NewMeta);
		}
		else
		{
			// The block is the top part of the door, set the meta to the corresponding top part
			if (a_BlockY > 0)
			{
				a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ, NewMeta);
			}
		}
	}


	/** Changes the door at the specified coords from open to close or vice versa */
	static void ChangeDoor(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		SetOpen(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ, !IsOpen(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ));
	}
} ;




