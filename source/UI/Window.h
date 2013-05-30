
// Window.h

// Interfaces to the cWindow class representing a UI window for a specific block





#pragma once

#include "../ItemGrid.h"





class cPlayer;
class cWindowOwner;
class cClientHandle;
class cChestEntity;
class cDropSpenserEntity;
class cFurnaceEntity;
class cSlotArea;
class cWorld;

typedef std::list<cPlayer *> cPlayerList;
typedef std::vector<cSlotArea *> cSlotAreas;





// tolua_begin

/** 
Represents a UI window.

Each window has a list of players that are currently using it
When there's no player using a window, it is destroyed.
A window consists of several areas of slots with similar functionality - for example the crafting grid area, or 
the inventory area. Each area knows what its slots are (GetSlot() function) and can handle mouse clicks.
The window acts only as a top-level container for those areas, redirecting the click events to the correct areas.
Inventory painting, introduced in 1.5, is handled by the window, too
*/
class cWindow
{
public:
	enum WindowType
	{
		Inventory   = -1,  // This value is never actually sent to a client
		Chest       = 0,
		Workbench   = 1,
		Furnace     = 2,
		DropSpenser = 3,  // Dropper or Dispenser
		Enchantment = 4,
		Brewery     = 5,
		NPCTrade    = 6,
		Beacon      = 7,
		Anvil       = 8,
		Hopper      = 9,
	};
	
	// tolua_end
	
	static const int c_NumInventorySlots = 36;

	cWindow(WindowType a_WindowType, const AString & a_WindowTitle);
	virtual ~cWindow();

	char GetWindowID(void) const { return m_WindowID; }
	int GetWindowType(void) const { return m_WindowType; }  // tolua_export

	cWindowOwner * GetOwner(void) { return m_Owner; }
	void SetOwner( cWindowOwner * a_Owner ) { m_Owner = a_Owner; }
	
	int GetNumSlots(void) const;
	
	// tolua_begin
	
	/// Returns the item at the specified slot for the specified player. Returns NULL if invalid SlotNum requested
	const cItem * GetSlot(cPlayer & a_Player, int a_SlotNum) const;
	
	/// Sets the item to the specified slot for the specified player
	void SetSlot(cPlayer & a_Player, int a_SlotNum, const cItem & a_Item);
	
	// tolua_end
	
	/// Fills a_Slots with the slots read from m_SlotAreas[], for the specified player
	void GetSlots(cPlayer & a_Player, cItems & a_Slots) const;

	/// Handles a click event from a player
	void Clicked(
		cPlayer & a_Player, int a_WindowID, 
		short a_SlotNum, eClickAction a_ClickAction,
		const cItem & a_ClickedItem
	);

	void OpenedByPlayer(cPlayer & a_Player);
	void ClosedByPlayer(cPlayer & a_Player);

	void SendWholeWindow(cClientHandle & a_Client);
	void BroadcastWholeWindow(void);
	void BroadcastInventoryProgress(short a_Progressbar, short a_Value);

	// tolua_begin
	
	const AString & GetWindowTitle() const { return m_WindowTitle; }
	void SetWindowTitle(const AString & a_WindowTitle ) { m_WindowTitle = a_WindowTitle; }
	
	// tolua_end

	void OwnerDestroyed(void);
	
	/// Calls the callback safely for each player that has this window open; returns true if all players have been enumerated
	bool ForEachPlayer(cItemCallback<cPlayer> & a_Callback);

	/// Calls the callback safely for each client that has this window open; returns true if all clients have been enumerated
	bool ForEachClient(cItemCallback<cClientHandle> & a_Callback);
	
	/** Called on shift-clicking to distribute the stack into other areas; Modifies a_ItemStack as it is distributed!
	if a_ShouldApply is true, the changes are written into the slots;
	if a_ShouldApply is false, only a_ItemStack is modified to reflect the number of fits (for fit-testing purposes)
	*/
	void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, cSlotArea * a_ExcludeArea, bool a_ShouldApply);
	
	/// Used by cSlotAreas to send individual slots to clients, a_RelativeSlotNum is the slot number relative to a_SlotArea
	void SendSlot(cPlayer & a_Player, cSlotArea * a_SlotArea, int a_RelativeSlotNum);
	
protected:
	cSlotAreas m_SlotAreas;
	
	char    m_WindowID;
	int     m_WindowType;
	AString m_WindowTitle;

	cCriticalSection m_CS;
	cPlayerList      m_OpenedBy;
	
	bool m_IsDestroyed;
	bool m_ShouldDistributeToHotbarFirst;  ///< If set (default), shift+click tries to distribute to hotbar first, then other areas. False for doublechests
	
	cWindowOwner * m_Owner;
	
	static char m_WindowIDCounter;

	void Destroy(void);
	
	/** Returns the correct slot area for the specified window-global SlotNum
	Also returns the area-local SlotNum corresponding to the GlobalSlotNum
	If the global SlotNum is out of range, returns NULL
	*/
	cSlotArea * GetSlotArea(int a_GlobalSlotNum, int & a_LocalSlotNum);
	
	/** Returns the correct slot area for the specified window-global SlotNum
	Also returns the area-local SlotNum corresponding to the GlobalSlotNum
	If the global SlotNum is out of range, returns NULL.
	Const version.
	*/
	const cSlotArea * GetSlotArea(int a_GlobalSlotNum, int & a_LocalSlotNum) const;
	
	/// Prepares the internal structures for inventory painting from the specified player
	void OnPaintBegin(cPlayer & a_Player);
	
	/// Adds the slot to the internal structures for inventory painting by the specified player
	void OnPaintProgress(cPlayer & a_Player, int a_SlotNum);
	
	/// Processes the entire action stored in the internal structures for inventory painting; distributes as many items as possible
	void OnLeftPaintEnd(cPlayer & a_Player);

	/// Processes the entire action stored in the internal structures for inventory painting; distributes one item into each slot
	void OnRightPaintEnd(cPlayer & a_Player);
} ;





class cCraftingWindow :
	public cWindow
{
public:
	cCraftingWindow(int a_BlockX, int a_BlockY, int a_BlockZ);
} ;





class cFurnaceWindow :
	public cWindow
{
public:
	cFurnaceWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceEntity * a_Furnace);
} ;





class cDropSpenserWindow :
	public cWindow
{
public:
	cDropSpenserWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserEntity * a_Dispenser);
} ;





class cChestWindow :
	public cWindow
{
public:
	cChestWindow(cChestEntity * a_Chest);
	cChestWindow(cChestEntity * a_PrimaryChest, cChestEntity * a_SecondaryChest);
	~cChestWindow();
	
protected:
	cWorld * m_World;
	int m_BlockX, m_BlockY, m_BlockZ;  // Position of the chest, for the window-close packet
} ;





class cInventoryWindow :
	public cWindow
{
public:
	cInventoryWindow(cPlayer & a_Player);
	
protected:
	cPlayer & m_Player;

} ;






// tolua_begin

/// A window that has been created by a Lua plugin and is handled entirely by that plugin
class cLuaWindow :
	public cWindow
{
public:
	/// Create a window of the specified type, with a slot grid of a_SlotsX * a_SlotsY size
	cLuaWindow(cWindow::WindowType a_WindowType, int a_SlotsX, int a_SlotsY, const AString & a_Title);
	
	// tolua_end
	
protected:
	/// Contents of the non-inventory part
	cItemGrid m_Contents;
} ;




