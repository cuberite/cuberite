#pragma once

#include "cInventory.h"

class cCreativeInventory								//tolua_export
	: public cInventory
{														//tolua_export
public:
	cCreativeInventory(cPlayer* a_Owner);
	~cCreativeInventory();

	virtual void Clicked( cPacket* a_ClickPacket );
};	//tolua_export
