#pragma once

#ifndef _WIN32
#include "BlockID.h"
#else
enum ENUM_ITEM_ID;
#endif

class cBlockToPickup
{
public:
	static ENUM_ITEM_ID ToPickup( unsigned char a_BlockID, ENUM_ITEM_ID a_UsedItemID );
	static char PickupCount(unsigned char a_BlockID);
};
