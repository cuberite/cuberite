
// BeaconWindow.h

// Representing the UI window for the beacon block





#pragma once

#include "Window.h"
#include "../Entities/Player.h"





class cBeaconWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cBeaconWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconEntity * a_Beacon);

	cBeaconEntity * GetBeaconEntity(void) const { return m_Beacon; }
	
	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

	// cWindow Overrides:
	virtual void OpenedByPlayer(cPlayer & a_Player) override;

protected:
	cBeaconEntity * m_Beacon;
};




