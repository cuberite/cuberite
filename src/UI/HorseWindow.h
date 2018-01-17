
// HorseWindow.h

// Representing the UI window for a horse entity

#pragma once

#include "Window.h"

class cHorse;





class cHorseWindow :
	public cWindow
{
	using Super = cWindow;
public:
	cHorseWindow(cHorse & a_Horse);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

	/** Returns the horse's entity ID. */
	UInt32 GetHorseID() const;

private:
	cHorse & m_Horse;
};




