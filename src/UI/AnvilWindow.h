
// AnvilWindow.h

// Representing the UI window for the anvil block





#pragma once

#include "Window.h"





class cAnvilWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cAnvilWindow(int a_BlockX, int a_BlockY, int a_BlockZ) :
		cWindow(wtAnvil, "Repair"),
		m_RepairedItemName(""),
		m_BlockX(a_BlockX),
		m_BlockY(a_BlockY),
		m_BlockZ(a_BlockZ)
	{
		m_AnvilSlotArea = new cSlotAreaAnvil(*this);
		m_SlotAreas.push_back(m_AnvilSlotArea);
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	}

	/** Gets the repaired item name. */
	AString GetRepairedItemName(void) const { return m_RepairedItemName; }

	/** Set the repaired item name. */
	void SetRepairedItemName(const AString & a_Name, cPlayer * a_Player)
	{
		m_RepairedItemName = a_Name;
		if (a_Player != nullptr)
		{
			m_AnvilSlotArea->UpdateResult(*a_Player);
		}
	}

	/** Gets the Position from the Anvil */
	void GetBlockPos(int & a_PosX, int & a_PosY, int & a_PosZ)
	{
		a_PosX = m_BlockX;
		a_PosY = m_BlockY;
		a_PosZ = m_BlockZ;
	}

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override
	{
		cSlotAreas AreasInOrder;

		if (a_ClickedArea == m_SlotAreas[0])
		{
			// Anvil Slot
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		}
		else
		{
			// Inventory or Hotbar
			AreasInOrder.push_back(m_SlotAreas[0]);  /* Anvil */
		}
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}

protected:
	cSlotAreaAnvil * m_AnvilSlotArea;
	AString m_RepairedItemName;
	int m_BlockX, m_BlockY, m_BlockZ;
};




