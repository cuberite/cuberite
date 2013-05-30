
// LuaWindow.cpp

// Implements the cLuaWindow class representing a virtual window that plugins may create and open for the player

#include "Globals.h"
#include "LuaWindow.h"
#include "UI/SlotArea.h"
#include "Plugin_NewLua.h"
#include "lauxlib.h"  // Needed for LUA_REFNIL





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLuaWindow:

cLuaWindow::cLuaWindow(cWindow::WindowType a_WindowType, int a_SlotsX, int a_SlotsY, const AString & a_Title) :
	super(a_WindowType, a_Title),
	m_Contents(a_SlotsX, a_SlotsY),
	m_Plugin(NULL),
	m_LuaRef(LUA_REFNIL)
{
	m_SlotAreas.push_back(new cSlotAreaItemGrid(m_Contents, *this));
	
	// If appropriate, add an Armor slot area:
	switch (a_WindowType)
	{
		case cWindow::Inventory:
		case cWindow::Workbench:
		{
			m_SlotAreas.push_back(new cSlotAreaArmor(*this));
			break;
		}
	}
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





cLuaWindow::~cLuaWindow()
{
	ASSERT(m_OpenedBy.empty());
}





void cLuaWindow::SetLuaRef(cPlugin_NewLua * a_Plugin, int a_LuaRef)
{
	ASSERT(m_Plugin == NULL);
	ASSERT(m_LuaRef == LUA_REFNIL);
	m_Plugin = a_Plugin;
	m_LuaRef = a_LuaRef;
}





bool cLuaWindow::IsLuaReferenced(void) const
{
	return ((m_Plugin != NULL) && (m_LuaRef != LUA_REFNIL));
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




