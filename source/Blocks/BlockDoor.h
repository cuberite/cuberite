
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Doors.h"
#include "../Entities/Player.h"





class cBlockDoorHandler :
	public cBlockHandler
{
public:
	cBlockDoorHandler(BLOCKTYPE a_BlockType);

	virtual void OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual const char * GetStepSound(void) override;
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
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
			!CanReplaceBlock(a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ)) ||
			!CanReplaceBlock(a_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ))
		)
		{
			return false;
		}
		
		a_BlockType = m_BlockType;
		a_BlockMeta = cDoors::RotationToMetaData(a_Player->GetRotation());
		return true;
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem((m_BlockType == E_BLOCK_WOODEN_DOOR) ? E_ITEM_WOODEN_DOOR : E_ITEM_IRON_DOOR, 1, 0));
	}


	virtual void OnPlacedByPlayer(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override;


	virtual bool IsUseable(void) override
	{
		return true;
	}


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
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
} ;




