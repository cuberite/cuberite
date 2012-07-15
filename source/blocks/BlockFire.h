#pragma once
#include "Block.h"


class cBlockFireHandler : public cBlockHandler
{
public:
	cBlockFireHandler(BLOCKTYPE a_BlockID);
	virtual void OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z);
	virtual bool IsClickedThrough();
	virtual char GetDropCount();
	
	
};