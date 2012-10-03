
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Sign.h"
#include "../Player.h"





class cBlockBedHandler :
	public cBlockHandler
{
public:
	cBlockBedHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override;
	virtual void OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override;


	virtual bool IsUseable(void) override
	{
		return true;
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_ITEM_BED, 1, 0));
	}


	virtual bool DoesAllowBlockOnTop() override
	{
		return false;
	}




	// Bed specific helper functions
	static NIBBLETYPE RotationToMetaData(float a_Rotation)
	{
		a_Rotation += 180 + (180/4); // So its not aligned with axis
		if( a_Rotation > 360.f ) a_Rotation -= 360.f;

		a_Rotation = (a_Rotation/360) * 4;

		return ((char)a_Rotation+2) % 4;
	}


	static Vector3i MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0: // south  +z
				return Vector3i(0, 0, 1);
			case 1: // west   -x
				return Vector3i(-1, 0, 0);
			case 2: // north  -z
				return Vector3i(0, 0, -1);
			case 3: // east   +x
				return Vector3i(1, 0, 0);
		};
		return Vector3i();
	}
} ;




