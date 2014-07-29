
// Window.h

// Interfaces to the cWindow class representing a UI window for a specific block





#pragma once

#include "../ItemGrid.h"





class cPlayer;
class cWindowOwner;
class cClientHandle;
class cChestEntity;
class cDropSpenserEntity;
class cEnderChestEntity;
class cFurnaceEntity;
class cHopperEntity;
class cSlotArea;
class cSlotAreaAnvil;
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
		wtInventory   = -1,  // This value is never actually sent to a client
		wtChest       = 0,
		wtWorkbench   = 1,
		wtFurnace     = 2,
		wtDropSpenser = 3,  // Dropper or Dispenser
		wtEnchantment = 4,
		wtBrewery     = 5,
		wtNPCTrade    = 6,
		wtBeacon      = 7,
		wtAnvil       = 8,
		wtHopper      = 9,
		// Unknown: 10
		wtAnimalChest = 11,
	};
	
	// tolua_end
	
	static const int c_NumInventorySlots = 36;

	cWindow(WindowType a_WindowType, const AString & a_WindowTitle);
	virtual ~cWindow();

	char GetWindowID(void) const { return m_WindowID; }  // tolua_export
	int GetWindowType(void) const { return m_WindowType; }  // tolua_export

	cWindowOwner * GetOwner(void) { return m_Owner; }
	void SetOwner( cWindowOwner * a_Owner) { m_Owner = a_Owner; }
	
	/// Returns the total number of slots
	int GetNumSlots(void) const;
	
	/// Returns the number of slots, excluding the player's inventory (used for network protocols)
	int GetNumNonInventorySlots(void) const { return GetNumSlots() - c_NumInventorySlots; }
	
	// tolua_begin
	
	/// Returns the item at the specified slot for the specified player. Returns NULL if invalid SlotNum requested
	const cItem * GetSlot(cPlayer & a_Player, int a_SlotNum) const;
	
	/// Sets the item to the specified slot for the specified player
	void SetSlot(cPlayer & a_Player, int a_SlotNum, const cItem & a_Item);
	
	/// Returns true if the specified slot is in the Player Main Inventory slotarea
	bool IsSlotInPlayerMainInventory(int a_SlotNum) const;
	
	/// Returns true if the specified slot is in the Player Hotbar slotarea
	bool IsSlotInPlayerHotbar(int a_SlotNum) const;
	
	/// Returns true if the specified slot is in the Player Main Inventory or Hotbar slotareas. Note that returns false for Armor.
	bool IsSlotInPlayerInventory(int a_SlotNum) const;
	
	// tolua_end
	
	/// Fills a_Slots with the slots read from m_SlotAreas[], for the specified player
	void GetSlots(cPlayer & a_Player, cItems & a_Slots) const;

	/// Handles a click event from a player
	void Clicked(
		cPlayer & a_Player, int a_WindowID,
		short a_SlotNum, eClickAction a_ClickAction,
		const cItem & a_ClickedItem
	);

	virtual void OpenedByPlayer(cPlayer & a_Player);
	
	/// Called when a player closes this window; notifies all slot areas. Returns true if close accepted
	virtual bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse);

	/// Sends the specified slot's contents to all clients of this window; the slot is specified as local in an area
	void BroadcastSlot(cSlotArea * a_Area, int a_LocalSlotNum);
	
	/// Sends the contents of the whole window to the specified client
	void SendWholeWindow(cClientHandle & a_Client);
	
	/// Sends the contents of the whole window to all clients of this window.
	void BroadcastWholeWindow(void);
	
	/// Sends the progressbar to all clients of this window (same as SetProperty)
	void BroadcastProgress(int a_Progressbar, int a_Value);

	// tolua_begin
	
	const AString & GetWindowTitle() const { return m_WindowTitle; }
	void SetWindowTitle(const AString & a_WindowTitle) { m_WindowTitle = a_WindowTitle; }
	
	/// Sends the UpdateWindowProperty (0x69) packet to all clients of the window
	virtual void SetProperty(int a_Property, int a_Value);
	
	/// Sends the UpdateWindowPropert(0x69) packet to the specified player
	virtual void SetProperty(int a_Property, int a_Value, cPlayer & a_Player);

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
	
	/// Called on DblClicking to collect all stackable items from all areas into hand, starting with the specified area.
	/// The items are accumulated in a_Dragging and removed from the SlotAreas immediately.
	/// If a_CollectFullStacks is false, slots with full stacks in the area are skipped while collecting.
	/// Returns true if full stack has been collected, false if there's space remaining to fill.
	bool CollectItemsToHand(cItem & a_Dragging, cSlotArea & a_Area, cPlayer & a_Player, bool a_CollectFullStacks);
	
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

	/// Sets the internal flag as "destroyed"; notifies the owner that the window is destroying
	virtual void Destroy(void);
	
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
	
	/// Distributes a_NumToEachSlot items into the slots specified in a_SlotNums; returns the total number of items distributed
	int DistributeItemToSlots(cPlayer & a_Player, const cItem & a_Item, int a_NumToEachSlot, const cSlotNums & a_SlotNums);
} ;  // tolua_export





class cCraftingWindow :
	public cWindow
{
	typedef cWindow super;
public:
	cCraftingWindow(int a_BlockX, int a_BlockY, int a_BlockZ);
} ;





class cAnvilWindow :
	public cWindow
{
	typedef cWindow super;
public:
	cAnvilWindow(int a_BlockX, int a_BlockY, int a_BlockZ);

	/** Gets the repaired item name. */
	AString GetRepairedItemName(void) const { return m_RepairedItemName; }

	/** Set the repaired item name. */
	void SetRepairedItemName(const AString & a_Name, cPlayer * a_Player);

	/** Gets the Position from the Anvil */
	void GetBlockPos(int & a_PosX, int & a_PosY, int & a_PosZ);

protected:
	cSlotAreaAnvil * m_AnvilSlotArea;
	AString m_RepairedItemName;
	int m_BlockX, m_BlockY, m_BlockZ;
} ;





class cEnchantingWindow :
	public cWindow
{
	typedef cWindow super;
public:
	cEnchantingWindow(int a_BlockX, int a_BlockY, int a_BlockZ);
	virtual void SetProperty(int a_Property, int a_Value, cPlayer & a_Player) override;
	virtual void SetProperty(int a_Property, int a_Value) override;

	/** Return the Value of a Property */
	int GetPropertyValue(int a_Property);

	/** Get the Position from the Enchantment Table */
	void GetBlockPos(int & a_PosX, int & a_PosY, int & a_PosZ);

	cSlotArea * m_SlotArea;

protected:
	int m_PropertyValue[3];
	int m_BlockX, m_BlockY, m_BlockZ;
};





class cFurnaceWindow :
	public cWindow
{
	typedef cWindow super;
public:
	cFurnaceWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceEntity * a_Furnace);
} ;





class cDropSpenserWindow :
	public cWindow
{
	typedef cWindow super;
public:
	cDropSpenserWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserEntity * a_Dispenser);
} ;





class cHopperWindow :
	public cWindow
{
	typedef cWindow super;
public:
	cHopperWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cHopperEntity * a_Hopper);
} ;





class cChestWindow :
	public cWindow
{
public:
	cChestWindow(cChestEntity * a_Chest);
	cChestWindow(cChestEntity * a_PrimaryChest, cChestEntity * a_SecondaryChest);
	~cChestWindow();

	virtual bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse) override;
	virtual void OpenedByPlayer(cPlayer & a_Player) override;
	
protected:
	cWorld * m_World;
	int m_BlockX, m_BlockY, m_BlockZ;  // Position of the chest, for the window-close packet
	cChestEntity * m_PrimaryChest;
	cChestEntity * m_SecondaryChest;
} ;





class cEnderChestWindow :
	public cWindow
{
public:
	cEnderChestWindow(cEnderChestEntity * a_EnderChest);
	~cEnderChestWindow();

protected:
	cWorld * m_World;
	int m_BlockX, m_BlockY, m_BlockZ;  // Position of the enderchest, for the window-close packet
};





class cInventoryWindow :
	public cWindow
{
public:
	cInventoryWindow(cPlayer & a_Player);
	
protected:
	cPlayer & m_Player;

} ;





