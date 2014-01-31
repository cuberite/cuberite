
#pragma once

#include "BlockHandler.h"
#include "../Entities/Player.h"





class cBlockDoorHandler :
	public cBlockHandler
{
public:
	cBlockDoorHandler(BLOCKTYPE a_BlockType);

	virtual void OnDestroyed(cChunkInterface * a_ChunkInterface, cWorldInterface * a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void OnUse(cChunkInterface * a_ChunkInterface, cWorldInterface * a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual const char * GetStepSound(void) override;
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface * a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
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
			!CanReplaceBlock(a_ChunkInterface->GetBlock(a_BlockX, a_BlockY, a_BlockZ)) ||
			!CanReplaceBlock(a_ChunkInterface->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ))
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
		cChunkInterface * a_ChunkInterface, cWorldInterface * a_WorldInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override;


	virtual bool IsUseable(void) override
	{
		return true;
	}


	virtual bool CanBeAt(cChunkInterface * a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
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


	/// Converts the player's yaw to placed door's blockmeta
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


	/// Returns true if the specified blocktype is any kind of door
	inline static bool IsDoor(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_WOODEN_DOOR) || (a_Block == E_BLOCK_IRON_DOOR);
	}


	/// Returns the metadata for the opposite door state (open vs closed)
	static NIBBLETYPE ChangeStateMetaData(NIBBLETYPE a_MetaData)
	{
		return a_MetaData ^ 4;
	}


	/// Changes the door at the specified coords from open to close or vice versa
	static void ChangeDoor(cChunkInterface * a_ChunkInterface, int a_X, int a_Y, int a_Z)
	{
		NIBBLETYPE OldMetaData = a_ChunkInterface->GetBlockMeta(a_X, a_Y, a_Z);
		
		a_ChunkInterface->SetBlockMeta(a_X, a_Y, a_Z, ChangeStateMetaData(OldMetaData));
		
		if (OldMetaData & 8)
		{
			// Current block is top of the door
			BLOCKTYPE  BottomBlock = a_ChunkInterface->GetBlock(a_X, a_Y - 1, a_Z);
			NIBBLETYPE BottomMeta  = a_ChunkInterface->GetBlockMeta(a_X, a_Y - 1, a_Z);

			if (IsDoor(BottomBlock) && !(BottomMeta & 8))
			{
				a_ChunkInterface->SetBlockMeta(a_X, a_Y - 1, a_Z, ChangeStateMetaData(BottomMeta));
			}
		}
		else
		{
			// Current block is bottom of the door
			BLOCKTYPE  TopBlock = a_ChunkInterface->GetBlock(a_X, a_Y + 1, a_Z);
			NIBBLETYPE TopMeta = a_ChunkInterface->GetBlockMeta(a_X, a_Y + 1, a_Z);

			if (IsDoor(TopBlock) && (TopMeta & 8))
			{
				a_ChunkInterface->SetBlockMeta(a_X, a_Y + 1, a_Z, ChangeStateMetaData(TopMeta));
			}
		}
	}
	
	
} ;




