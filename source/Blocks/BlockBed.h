
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cBlockBedHandler :
	public cBlockHandler
{
public:
	cBlockBedHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void OnPlacedByPlayer(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;


	virtual bool IsUseable(void) override
	{
		return true;
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_ITEM_BED, 1, 0));
	}


	// Bed specific helper functions
	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 180 + (180 / 4);  // So its not aligned with axis
		if (a_Rotation > 360) a_Rotation -= 360;

		a_Rotation = (a_Rotation / 360) * 4;

		return ((char)a_Rotation + 2) % 4;
	}


	static Vector3i MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0: return Vector3i(0, 0, 1);
			case 1: return Vector3i(-1, 0, 0);
			case 2: return Vector3i(0, 0, -1);
			case 3: return Vector3i(1, 0, 0);
		}
		return Vector3i();
	}
} ;




