#pragma once
#include "Block.h"


class cBlockWorkbenchHandler : public cBlockHandler
{
public:
	cBlockWorkbenchHandler(BLOCKTYPE a_BlockID);
	virtual void OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir);
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z);
	
	virtual void OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z);
	virtual bool IsUseable()
	{
		return true;
	}
	
	
};