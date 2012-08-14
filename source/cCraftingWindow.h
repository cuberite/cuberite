
#pragma once

#include "cWindow.h"




// fwd:
class cWindowOwner;





class cCraftingWindow : public cWindow
{
public:
	enum
	{
		SLOT_CRAFTING_RESULT = 0,
		SLOT_CRAFTING_MIN = 1,
		SLOT_CRAFTING_MAX = 9,
	} ;
	
	cCraftingWindow(cWindowOwner * a_Owner, bool a_bInventoryVisible);

	virtual void Clicked(cPacket_WindowClick * a_ClickPacket, cPlayer & a_Player);
	virtual void Close(cPlayer & a_Player);
	
	void ShiftClicked(cPacket_WindowClick * a_ClickPacket, cPlayer & a_Player);
	void ShiftClickedCraftingResult(short a_Slot, cPlayer & a_Player);
	void ShiftClickedCraftingGrid  (short a_Slot, cPlayer & a_Player);
};




