
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

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea* a_ClickedArea, bool a_ShouldApply) override
	{
		cSlotAreas AreasInOrder;

		if (a_ClickedArea == m_SlotAreas[0])
		{
			// Chest Area
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
		}
		else
		{
			// Hotbar or Inventory
			AreasInOrder.push_back(m_SlotAreas[0]);  /* Chest */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
		}
	}


	~cMinecartWithChestWindow()
	{
		m_ChestCart->GetWorld()->BroadcastSoundEffect("random.chestclosed", m_ChestCart->GetPosX(), m_ChestCart->GetPosY(), m_ChestCart->GetPosZ(), 1, 1);
	}

private:
	cMinecartWithChest * m_ChestCart;
};




