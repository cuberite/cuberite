
// MinecartWithChestWindow.h

// Representing the UI window for the minecart chest entity





#pragma once

#include "Window.h"
#include "../Entities/Minecart.h"





class cMinecartWithChestWindow:
	public cWindow
{
	using Super = cWindow;

public:

	cMinecartWithChestWindow(cMinecartWithChest * a_ChestCart) :
		cWindow(wtChest, "Minecart with Chest"),
		m_ChestCart(a_ChestCart)
	{
		m_SlotAreas.push_back(new cSlotAreaMinecartWithChest(a_ChestCart, *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));

		a_ChestCart->GetWorld()->BroadcastSoundEffect("block.chest.open", a_ChestCart->GetPosition(), 1, 1);
	}

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override
	{
		cSlotAreas AreasInOrder;

		if (a_ClickedArea == m_SlotAreas[0])
		{
			// Chest Area
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
		}
		else
		{
			// Hotbar or Inventory
			AreasInOrder.push_back(m_SlotAreas[0]);  /* Chest */
			Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
		}
	}


	virtual ~cMinecartWithChestWindow() override
	{
		m_ChestCart->GetWorld()->BroadcastSoundEffect("block.chest.close", m_ChestCart->GetPosition(), 1, 1);
	}

private:
	cMinecartWithChest * m_ChestCart;
};




