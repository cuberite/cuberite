
// InventoryWindow.h

// Representing the UI window for the player inventory





#pragma once

#include "Window.h"





class cInventoryWindow:
	public cWindow
{
	using Super = cWindow;

public:

	cInventoryWindow(cPlayer & a_Player);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

	/** Loads the given Recipe into the crafting grid */
	void LoadRecipe(cPlayer & a_Player, UInt32 a_RecipeId);
protected:
	cPlayer & m_Player;
};
