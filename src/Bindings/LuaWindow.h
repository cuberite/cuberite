
// LuaWindow.h

// Declares the cLuaWindow class representing a virtual window that plugins may create and open for the player





#pragma once

#include "../UI/Window.h"
#include "../ItemGrid.h"





// fwd: PluginLua.h
class cPluginLua;





/** A window that has been created by a Lua plugin and is handled entirely by that plugin
This object needs extra care with its lifetime management:
- It is created by Lua, so Lua expects to garbage-collect it later
- normal cWindow objects are deleted in their ClosedByPlayer() function if the last player closes them
To overcome this, this object overloads the Destroy functions, which doesn't let the ClosedByPlayer()
delete the window, but rather leaves it dangling, with only Lua having the reference to it.
Additionally, to forbid Lua from deleting this object while it is used by players, the manual bindings for
cPlayer:OpenWindow check if the window is of this class, and if so, make a global Lua reference for this object.
This reference needs to be unreferenced in the Destroy() function. */
// tolua_begin
class cLuaWindow :
	public cWindow
	// tolua_end
	, public cItemGrid::cListener
	// tolua_begin
{
	typedef cWindow super;
	
public:
	/** Create a window of the specified type, with a slot grid of a_SlotsX * a_SlotsY size */
	cLuaWindow(cWindow::WindowType a_WindowType, int a_SlotsX, int a_SlotsY, const AString & a_Title);
	
	virtual ~cLuaWindow();
	
	/** Returns the internal representation of the contents that are manipulated by Lua */
	cItemGrid & GetContents(void) { return m_Contents; }
	
	// tolua_end
	
	/** Sets the plugin reference and the internal Lua object reference index
	used for preventing Lua's GC to collect this class while the window is open. */
	void SetLuaRef(cPluginLua * a_Plugin, int a_LuaRef);
	
	/** Returns true if SetLuaRef() has been called */
	bool IsLuaReferenced(void) const;
	
	/** Sets the callback function (Lua reference) to call when the window is about to close */
	void SetOnClosing(cPluginLua * a_Plugin, int a_FnRef);
	
	/** Sets the callback function (Lua reference) to call when a slot is changed */
	void SetOnSlotChanged(cPluginLua * a_Plugin, int a_FnRef);
	
protected:
	/** Contents of the non-inventory part */
	cItemGrid m_Contents;
	
	/** The plugin that has opened the window and owns the m_LuaRef */
	cPluginLua * m_Plugin;
	
	/** The Lua object reference, used for keeping the object alive as long as any player has the window open */
	int m_LuaRef;
	
	/** The Lua reference for the callback to call when the window is closing for any player */
	int m_OnClosingFnRef;
	
	/** The Lua reference for the callback to call when a slot has changed */
	int m_OnSlotChangedFnRef;
	
	// cWindow overrides:
	virtual bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse) override;
	virtual void Destroy(void) override;
	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;
	
	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) override;
} ;  // tolua_export





