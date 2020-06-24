
// AnvilWindow.h

// Representing the UI window for the anvil block





#pragma once

#include "Window.h"





class cAnvilWindow:
	public cWindow
{
	using Super = cWindow;

public:

	cAnvilWindow(Vector3i a_BlockPos);

	/** Gets the repaired item name. */
	AString GetRepairedItemName(void) const;

	/** Set the repaired item name. */
	void SetRepairedItemName(const AString & a_Name, cPlayer * a_Player);

	/** Gets the Position from the Anvil */
	const Vector3i & GetBlockPos() { return m_BlockPos; }

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

protected:
	cSlotAreaAnvil * m_AnvilSlotArea;
	AString m_RepairedItemName;
	Vector3i m_BlockPos;
};




