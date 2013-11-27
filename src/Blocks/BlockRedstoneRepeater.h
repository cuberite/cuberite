
#pragma once

#include "BlockHandler.h"





class cBlockRedstoneRepeaterHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockType);
	virtual void OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_REPEATER, 1, 0));
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
	
	
	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR));
	}
	

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override;


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}


	inline static NIBBLETYPE RepeaterRotationToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45; // So its not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}

		if ((a_Rotation >= 0) && (a_Rotation < 90))
		{
			return 0x1;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return 0x3;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return 0x2;
		}
		else
		{
			return 0x0;
		}
	}
} ;




