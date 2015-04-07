
// AnvilWindow.h

// Representing the UI window for the anvil block





#pragma once

#include "Window.h"





class cAnvilWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cAnvilWindow(int a_BlockX, int a_BlockY, int a_BlockZ);

	/** Gets the repaired item name. */
	AString GetRepairedItemName(void) const;

	/** Set the repaired item name. */
	void SetRepairedItemName(const AString & a_Name, cPlayer * a_Player);

	/** Gets the Position from the Anvil */
	void GetBlockPos(int & a_PosX, int & a_PosY, int & a_PosZ);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

protected:
	cSlotAreaAnvil * m_AnvilSlotArea;
	AString m_RepairedItemName;
	int m_BlockX, m_BlockY, m_BlockZ;
};




