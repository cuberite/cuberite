// LuaWindow.cpp

// Implements the cLuaWindow class representing a virtual window that plugins may create and open for the player

#include "Globals.h"
#include "LuaWindow.h"
#include "../UI/SlotArea.h"
#include "PluginLua.h"
#include "../Entities/Player.h"
#include "lua/src/lauxlib.h"  // Needed for LUA_REFNIL





////////////////////////////////////////////////////////////////////////////////
// cLuaWindow:

cLuaWindow::cLuaWindow(cWindow::WindowType a_WindowType, int a_SlotsX, int a_SlotsY, const AString & a_Title) :
	super(a_WindowType, a_Title),
	m_Contents(a_SlotsX, a_SlotsY),
	m_Plugin(NULL),
	m_LuaRef(LUA_REFNIL),
	m_OnClosingFnRef(LUA_REFNIL),
	m_OnSlotChangedFnRef(LUA_REFNIL)
{
	m_Contents.AddListener(*this);
	m_SlotAreas.push_back(new cSlotAreaItemGrid(m_Contents, *this));
	
	// If appropriate, add an Armor slot area:
	switch (a_WindowType)
	{
		case cWindow::wtInventory:
		case cWindow::wtWorkbench:
		{
			m_SlotAreas.push_back(new cSlotAreaArmor(*this));
			break;
		}
		default:
		{
			break;
		}
	}
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





cLuaWindow::~cLuaWindow()
{
	m_Contents.RemoveListener(*this);

	// Must delete slot areas now, because they are referencing this->m_Contents and would try to access it in cWindow's
	// destructor, when the member is already gone.
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_SlotAreas.clear();

	ASSERT(m_OpenedBy.empty());
}





void cLuaWindow::SetLuaRef(cPluginLua * a_Plugin, int a_LuaRef)
{
	// Either m_Plugin is not set or equal to the passed plugin; only one plugin can use one cLuaWindow object
	ASSERT((m_Plugin == NULL) || (m_Plugin == a_Plugin));
	ASSERT(m_LuaRef == LUA_REFNIL);
	m_Plugin = a_Plugin;
	m_LuaRef = a_LuaRef;
}





bool cLuaWindow::IsLuaReferenced(void) const
{
	return ((m_Plugin != NULL) && (m_LuaRef != LUA_REFNIL));
}





void cLuaWindow::SetOnClosing(cPluginLua * a_Plugin, int a_FnRef)
{
	// Either m_Plugin is not set or equal to the passed plugin; only one plugin can use one cLuaWindow object
	ASSERT((m_Plugin == NULL) || (m_Plugin == a_Plugin));
	
	// If there already was a function, unreference it first
	if (m_OnClosingFnRef != LUA_REFNIL)
	{
		m_Plugin->Unreference(m_OnClosingFnRef);
	}
	
	// Store the new reference
	m_Plugin = a_Plugin;
	m_OnClosingFnRef = a_FnRef;
}





void cLuaWindow::SetOnSlotChanged(cPluginLua * a_Plugin, int a_FnRef)
{
	// Either m_Plugin is not set or equal to the passed plugin; only one plugin can use one cLuaWindow object
	ASSERT((m_Plugin == NULL) || (m_Plugin == a_Plugin));
	
	// If there already was a function, unreference it first
	if (m_OnSlotChangedFnRef != LUA_REFNIL)
	{
		m_Plugin->Unreference(m_OnSlotChangedFnRef);
	}
	
	// Store the new reference
	m_Plugin = a_Plugin;
	m_OnSlotChangedFnRef = a_FnRef;
}





bool cLuaWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	// First notify the plugin through the registered callback:
	if (m_OnClosingFnRef != LUA_REFNIL)
	{
		ASSERT(m_Plugin != NULL);
		if (m_Plugin->CallbackWindowClosing(m_OnClosingFnRef, *this, a_Player, a_CanRefuse))
		{
			// The callback disagrees (the higher levels check the CanRefuse flag compliance)
			return false;
		}
	}
	
	return super::ClosedByPlayer(a_Player, a_CanRefuse);
}





void cLuaWindow::Destroy(void)
{
	super::Destroy();
	
	if ((m_LuaRef != LUA_REFNIL) && (m_Plugin != NULL))
	{
		// The object is referenced by Lua, un-reference it
		m_Plugin->Unreference(m_LuaRef);
	}
	
	// Lua will take care of this object, it will garbage-collect it, so we *must not* delete it!
	m_IsDestroyed = false;
}





void cLuaWindow::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	if (a_ItemGrid != &m_Contents)
	{
		ASSERT(!"Invalid ItemGrid in callback");
		return;
	}
	
	// If an OnSlotChanged callback has been registered, call it:
	if (m_OnSlotChangedFnRef != LUA_REFNIL)
	{
		m_Plugin->CallbackWindowSlotChanged(m_OnSlotChangedFnRef, *this, a_SlotNum);
	}
}




