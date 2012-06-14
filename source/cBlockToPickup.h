#pragma once

#ifndef _WIN32
	#include "BlockID.h"
#else
	enum ENUM_ITEM_ID;
#endif

#include "cItem.h"





class cBlockToPickup      // tolua_export
{                         // tolua_export
public:
	/// For a given block and tool, returns the list of drops generated
	static void ToPickup(BLOCKTYPE a_BlockID, NIBBLETYPE a_BlockMeta, const cItem & a_UsedItem, cItems & a_Drops);  // tolua_export
	
	/// Returns true if the tool used for the block is the right one for the job. cClientHandle uses this to determine whether to decrease tool durability twice as much
	static bool IsRightTool(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, ENUM_ITEM_ID a_UsedTool);  // tolua_export
	
};      // tolua_export




