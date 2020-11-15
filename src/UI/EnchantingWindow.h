
// EnchantingWindow.h

// Representing the UI window for the enchanting block





#pragma once

#include "Window.h"





class cSlotAreaEnchanting;





class cEnchantingWindow final :
	public cWindow
{
	using Super = cWindow;

public:

	cEnchantingWindow(Vector3i a_BlockPos, const AString & a_Title);

	/** Sends enchantment properties to the client.
	If the property represents a level requirement, stores it for later GetProperty retrieval. */
	virtual void SetProperty(size_t a_Property, short a_Value) override;

	/** Return the level requirement of the given enchantment slot. */
	short GetProperty(size_t a_Property);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

	cSlotAreaEnchanting * m_SlotArea;

protected:

	std::array<short, 3> m_PropertyValue;
	Vector3i m_BlockPos;
};
