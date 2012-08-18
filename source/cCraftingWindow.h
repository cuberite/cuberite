
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

	virtual void Clicked(
		cPlayer & a_Player, 
		int a_WindowID, short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, 
		const cItem & a_HeldItem
	) override;
	
	virtual void Close(cPlayer & a_Player) override;
	
	void ShiftClicked(cPlayer & a_Player, short a_SlotNum);
	void ShiftClickedCraftingResult(cPlayer & a_Player, short a_SlotNum);
	void ShiftClickedCraftingGrid  (cPlayer & a_Player, short a_SlotNum);
};




