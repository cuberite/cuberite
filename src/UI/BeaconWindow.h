
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
	cBeaconWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconEntity * a_Beacon) :
		cWindow(wtBeacon, "Beacon"),
		m_Beacon(a_Beacon)
	{
		m_ShouldDistributeToHotbarFirst = true;
		m_SlotAreas.push_back(new cSlotAreaBeacon(m_Beacon, *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	}

	cBeaconEntity * GetBeaconEntity(void) const { return m_Beacon; }

	// cWindow Overrides:
	virtual void OpenedByPlayer(cPlayer & a_Player) override
	{
		super::OpenedByPlayer(a_Player);

		a_Player.GetClientHandle()->SendWindowProperty(*this, 0, m_Beacon->GetBeaconLevel());
		a_Player.GetClientHandle()->SendWindowProperty(*this, 1, m_Beacon->GetPrimaryEffect());
		a_Player.GetClientHandle()->SendWindowProperty(*this, 2, m_Beacon->GetSecondaryEffect());
	}

protected:
	cBeaconEntity * m_Beacon;
};




