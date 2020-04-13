
// LuaWindow.h

// Declares the cLuaWindow class representing a virtual window that plugins may create and open for the player





#pragma once

#include "LuaState.h"
#include "../UI/Window.h"
#include "../ItemGrid.h"


class cPlayer;
typedef cFunctionRef<bool(cPlayer &)> cPlayerListCallback;


/** A window that has been created by a Lua plugin and is handled entirely by that plugin
This object needs extra care with its lifetime management:
- It is created by Lua, so Lua expects to garbage-collect it later
- Normal cWindow objects are deleted in their ClosedByPlayer() function if the last player closes them
	To overcome this, this object overloads the Destroy functions, which doesn't let the ClosedByPlayer()
	delete the window, but rather leaves it dangling, with only Lua having the reference to it.
- Lua could GC the window while a player is still using it
	The object creates a Lua reference to itself when opened by a player and
	removes the reference when the last player closes the window.
*/
// tolua_begin
class cLuaWindow :
	public cWindow
	// tolua_end
	, public cItemGrid::cListener
{  // tolua_export
	using Super = cWindow;

public:
	/** Create a window of the specified type, with a slot grid of a_SlotsX * a_SlotsY size.
	Exported in ManualBindings.cpp */
	cLuaWindow(cLuaState & a_LuaState, cWindow::WindowType a_WindowType, int a_SlotsX, int a_SlotsY, const AString & a_Title);

	// tolua_begin
	virtual ~cLuaWindow() override;

	/** Returns the internal representation of the contents that are manipulated by Lua */
	cItemGrid & GetContents(void) { return m_Contents; }

	// tolua_end

	/** Sets the Lua callback to call when the player clicks on the window.
	The window can stop the click from propogating. */
	void SetOnClicked(cLuaState::cCallbackPtr && a_OnClicked);

	/** Sets the Lua callback function to call when the window is about to close */
	void SetOnClosing(cLuaState::cCallbackPtr && a_OnClosing);

	/** Sets the Lua callback function to call when a slot is changed */
	void SetOnSlotChanged(cLuaState::cCallbackPtr && a_OnSlotChanged);

protected:

	/** Contents of the non-inventory part */
	cItemGrid m_Contents;

	/** The canon Lua state that has opened the window and owns the m_LuaRef */
	cLuaState * m_LuaState;

	/** The Lua callback to call when the player clicked on a slot */
	cLuaState::cCallbackPtr m_OnClicked;

	/** The Lua callback to call when the window is closing for any player */
	cLuaState::cCallbackPtr m_OnClosing;

	/** The Lua callback to call when a slot has changed */
	cLuaState::cCallbackPtr m_OnSlotChanged;

	/** Number of players that are currently using the window.
	Used to manager the m_LuaRef lifetime. */
	std::atomic<int> m_PlayerCount;

	/** Reference to self, to keep Lua from GCing the object while a player is still using it.
	Created when the first player opens the window, destroyed when the last player closes the window. */
	cLuaState::cRef m_LuaRef;


	// cWindow overrides:
	virtual void OpenedByPlayer(cPlayer & a_Player) override;
	virtual void Clicked(
		cPlayer & a_Player, int a_WindowID,
		short a_SlotNum, eClickAction a_ClickAction,
		const cItem & a_ClickedItem
	) override;
	virtual bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse) override;
	virtual void Destroy(void) override;
	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) override;
} ;  // tolua_export





