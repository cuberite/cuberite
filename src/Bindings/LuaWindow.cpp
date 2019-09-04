// LuaWindow.cpp

// Implements the cLuaWindow class representing a virtual window that plugins may create and open for the player

#include "Globals.h"
#include "LuaWindow.h"
#include "../Entities/Player.h"
#include "../UI/SlotArea.h"
#include "PluginLua.h"
#include "lua/src/lauxlib.h"  // Needed for LUA_REFNIL
#include "../Root.h"
#include "../ClientHandle.h"




////////////////////////////////////////////////////////////////////////////////
// cLuaWindow:

cLuaWindow::cLuaWindow(cLuaState & a_LuaState, cWindow::WindowType a_WindowType, int a_SlotsX, int a_SlotsY, const AString & a_Title) :
	Super(a_WindowType, a_Title),
	m_Contents(a_SlotsX, a_SlotsY),
	m_LuaState(a_LuaState.QueryCanonLuaState())
{
	ASSERT(m_LuaState != nullptr);  // We must have a valid Lua state; this assert fails only if there was no Canon Lua state

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

	// Close open lua window from players, to avoid dangling pointers
	cRoot::Get()->ForEachPlayer([this](cPlayer & a_Player)
		{
			if (a_Player.GetWindow() == this)
			{
				a_Player.CloseWindow(false);
			}
			return false;
		}
	);

	// Must delete slot areas now, because they are referencing this->m_Contents and would try to access it in cWindow's
	// destructor, when the member is already gone.
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_SlotAreas.clear();

	ASSERT(m_OpenedBy.empty());
}





void cLuaWindow::SetOnClicked(cLuaState::cCallbackPtr && a_OnClicked)
{
	// Only one Lua state can be a cLuaWindow object callback:
	ASSERT(a_OnClicked->IsSameLuaState(*m_LuaState));

	// Store the new reference, releasing the old one if appropriate:
	m_OnClicked = std::move(a_OnClicked);
}





void cLuaWindow::SetOnClosing(cLuaState::cCallbackPtr && a_OnClosing)
{
	// Only one Lua state can be a cLuaWindow object callback:
	ASSERT(a_OnClosing->IsSameLuaState(*m_LuaState));

	// Store the new reference, releasing the old one if appropriate:
	m_OnClosing = std::move(a_OnClosing);
}





void cLuaWindow::SetOnSlotChanged(cLuaState::cCallbackPtr && a_OnSlotChanged)
{
	// Only one Lua state can be a cLuaWindow object callback:
	ASSERT(a_OnSlotChanged->IsSameLuaState(*m_LuaState));

	// Store the new reference, releasing the old one if appropriate:
	m_OnSlotChanged = std::move(a_OnSlotChanged);
}





void cLuaWindow::OpenedByPlayer(cPlayer & a_Player)
{
	// If the first player is opening the window, create a Lua Reference to the window object so that Lua will not GC it until the last player closes the window:
	if (m_PlayerCount == 0)
	{
		m_LuaRef.CreateFromObject(*m_LuaState, this);
	}

	++m_PlayerCount;
	Super::OpenedByPlayer(a_Player);
}





bool cLuaWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	// First notify the plugin through the registered callback:
	if (m_OnClosing != nullptr)
	{
		bool res;
		if (
			m_OnClosing->Call(this, &a_Player, a_CanRefuse, cLuaState::Return, res) &&  // The callback succeeded
			res                                                                         // The callback says not to close the window
		)
		{
			// The callback disagrees (the higher levels check the CanRefuse flag compliance)
			return false;
		}
	}

	// If the last player has closed the window, release the Lua reference, so that Lua may GC the object:
	--m_PlayerCount;
	if (m_PlayerCount == 0)
	{
		m_LuaRef.UnRef();
	}

	return Super::ClosedByPlayer(a_Player, a_CanRefuse);
}





void cLuaWindow::Destroy(void)
{
	Super::Destroy();

	// Lua will take care of this object, it will garbage-collect it, so we must not delete it!
	m_IsDestroyed = false;
}





void cLuaWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas Areas;
	for (auto && Area : m_SlotAreas)
	{
		if (Area != a_ClickedArea)
		{
			Areas.push_back(Area);
		}
	}

	Super::DistributeStackToAreas(a_ItemStack, a_Player, Areas, a_ShouldApply, false);
}





void cLuaWindow::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	if (a_ItemGrid != &m_Contents)
	{
		ASSERT(!"Invalid ItemGrid in callback");
		return;
	}

	// If an OnSlotChanged callback has been registered, call it:
	if (m_OnSlotChanged != nullptr)
	{
		m_OnSlotChanged->Call(this, a_SlotNum);
	}
}





void cLuaWindow::Clicked(cPlayer & a_Player, int a_WindowID, short a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	if (m_OnClicked != nullptr)
	{
		// Plugin can stop a click
		bool res;
		if (m_OnClicked->Call(this, &a_Player, a_SlotNum, a_ClickAction, a_ClickedItem, cLuaState::Return, res) && res)
		{
			// Tell the client the actual state of the window
			a_Player.GetClientHandle()->SendInventorySlot(-1, -1, a_Player.GetDraggingItem());
			BroadcastWholeWindow();
			return;
		}
	}

	cWindow::Clicked(a_Player, a_WindowID, a_SlotNum, a_ClickAction, a_ClickedItem);
}




