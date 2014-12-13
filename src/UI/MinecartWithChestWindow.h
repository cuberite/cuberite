
// MinecartWithChestWindow.h

// Representing the UI window for the minecart chest entity





#pragma once

#include "Window.h"
#include "../Entities/Minecart.h"





class cMinecartWithChestWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cMinecartWithChestWindow(cMinecartWithChest * a_ChestCart) :
		cWindow(wtChest, "Minecart with Chest"),
		m_ChestCart(a_ChestCart)
	{
		m_SlotAreas.push_back(new cSlotAreaMinecartWithChest(a_ChestCart, *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));

		a_ChestCart->GetWorld()->BroadcastSoundEffect("random.chestopen", a_ChestCart->GetPosX(), a_ChestCart->GetPosY(), a_ChestCart->GetPosZ(), 1, 1);
	}

	~cMinecartWithChestWindow()
	{
		m_ChestCart->GetWorld()->BroadcastSoundEffect("random.chestclosed", m_ChestCart->GetPosX(), m_ChestCart->GetPosY(), m_ChestCart->GetPosZ(), 1, 1);
	}

private:
	cMinecartWithChest * m_ChestCart;
};




