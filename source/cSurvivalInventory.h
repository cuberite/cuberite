#pragma once

#include "cInventory.h"

class cSurvivalInventory										//tolua_export
	: public cInventory	
	, public cWindowOwner	
{														//tolua_export
public:
	cSurvivalInventory(cPlayer* a_Owner);
	~cSurvivalInventory();

	virtual void Clicked( cPacket* a_ClickPacket );
};	//tolua_export
