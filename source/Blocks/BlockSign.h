
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Player.h"





class cBlockSignHandler :
	public cBlockHandler
{
public:
	cBlockSignHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SIGN, 1, 0));
	}

	
	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}


	static char RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 180 + (180 / 16);  // So it's not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}
		
		a_Rotation = (a_Rotation / 360) * 16;
		
		return ((char)a_Rotation) % 16;
	}
	
	
	static char DirectionToMetaData(char a_Direction)
	{
		switch (a_Direction)
		{
			case 0x2: return 0x2;
			case 0x3: return 0x3;
			case 0x4: return 0x4;
			case 0x5: return 0x5;
			default:
			{
				break;
			}
		}
		return 0x2;
	}	
} ;




