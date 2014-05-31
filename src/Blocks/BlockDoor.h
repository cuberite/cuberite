
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
			return 0x0;
		}
		else if ((a_Yaw >= 180) && (a_Yaw < 270))
		{
			return 0x2;
		}
		else if ((a_Yaw >= 90) && (a_Yaw < 180))
		{
			return 0x1;
		}
		else
		{
			return 0x3;
		}
	}


	/** Returns true if the specified blocktype is any kind of door */
	inline static bool IsDoor(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_WOODEN_DOOR) || (a_Block == E_BLOCK_IRON_DOOR);
	}


	static NIBBLETYPE IsOpen(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		NIBBLETYPE Meta = GetTrueDoorMeta(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
		return ((Meta & 0x4) != 0);
	}


	/** Read the meta from the true part of the door and returns a meta with all infos include. */
	static NIBBLETYPE GetTrueDoorMeta(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

		if ((Meta & 0x8) != 0)
		{
			NIBBLETYPE DownMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ);
			return (DownMeta & 0x7) | 0x8 | (((Meta & 0x1) != 0) ? 16 : 0);
		}
		else
		{
			NIBBLETYPE UpMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY + 1, a_BlockZ);
			return (Meta & 0x7) | (((UpMeta & 0x1) != 0) ? 16 : 0);
		}
	}


	static void SetOpen(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_Open)
	{
		BLOCKTYPE Block = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		NIBBLETYPE Meta = GetTrueDoorMeta(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);

		if (!IsDoor(Block))
		{
			return;
		}

		bool Opened = (Meta & 0x4) != 0;
		if (Opened == a_Open)
		{
			return;
		}

		// Change the door
		NIBBLETYPE NewMeta = (Meta & 0x7) ^ 0x4;
		if ((Meta & 0x8) == 0)
		{
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, NewMeta);
		}
		else
		{
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ, NewMeta);
		}
	}


	/** Changes the door at the specified coords from open to close or vice versa */
	static void ChangeDoor(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		SetOpen(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ, !IsOpen(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ));
	}
} ;




