
// SlotArea.h

// Interfaces to the cSlotArea class representing a contiguous area of slots in a UI window




#pragma once

#include "../Item.h"



class cWindow;
class cPlayer;
class cChestEntity;
class cDispenserEntity;
class cFurnaceEntity;
class cCraftingRecipe;





class cSlotArea
{
public:
	cSlotArea(int a_NumSlots, cWindow & a_ParentWindow);
	virtual ~cSlotArea() {}  // force a virtual destructor in all subclasses
	
	int GetNumSlots(void) const { return m_NumSlots; }
	
	/// Called to retrieve an item in the specified slot for the specified player. Must return a valid cItem.
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) = 0;
	
	/// Called to set an item in the specified slot for the specified player
	virtual void SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) = 0;
	
	/// Called when a player clicks in the window. Parameters taken from the click packet.
	virtual void Clicked(cPlayer & a_Player, int a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, const cItem & a_ClickedItem);
	
	/// Called from Clicked if it is a valid shiftclick
	virtual void ShiftClicked(cPlayer & a_Player, int a_SlotNum, const cItem & a_ClickedItem);
	
	/// Called when a new player opens the same parent window. The window already tracks the player. CS-locked.
	virtual void OnPlayerAdded(cPlayer & a_Player) {} ;
	
	/// Called when one of the players closes the parent window. The window already doesn't track the player. CS-locked.
	virtual void OnPlayerRemoved(cPlayer & a_Player) {} ;
	
	/** Called to store as much of a_ItemStack in the area as possible. a_ItemStack is modified to reflect the change.
	The default implementation searches each slot for available space and distributes the stack there.
	if a_ShouldApply is true, the changes are written into the slots;
	if a_ShouldApply is false, only a_ItemStack is modified to reflect the number of fits (for fit-testing purposes)
	If a_KeepEmptySlots is true, empty slots will be skipped and won't be filled
	*/
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots);
	
protected:
	int       m_NumSlots;
	cWindow & m_ParentWindow;
} ;





/// Handles any part of the inventory, using parameters in constructor to distinguish between the parts
class cSlotAreaInventoryBase :
	public cSlotArea
{
	typedef cSlotArea super;
	
public:
	cSlotAreaInventoryBase(int a_NumSlots, int a_SlotOffset, cWindow & a_ParentWindow);
	
	// Creative inventory's click handling is somewhat different from survival inventory's, handle that here:
	virtual void Clicked(cPlayer & a_Player, int a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, const cItem & a_ClickedItem) override;

	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;
	
protected:
	int m_SlotOffset;  // Index that this area's slot 0 has in the underlying cInventory
} ;





/// Handles the "inner" inventory of each player, excluding the armor and hotbar
class cSlotAreaInventory :
	public cSlotAreaInventoryBase
{
	typedef cSlotAreaInventoryBase super;
	
public:
	cSlotAreaInventory(cWindow & a_ParentWindow) :
		cSlotAreaInventoryBase(27, 9, a_ParentWindow)  // 27 slots, starting at inventory index 9
	{
	}
} ;





/// Handles the "outer" inevntory of each player - the hotbar
class cSlotAreaHotBar :
	public cSlotAreaInventoryBase
{
	typedef cSlotAreaInventoryBase super;
	
public:
	cSlotAreaHotBar(cWindow & a_ParentWindow)	:
		cSlotAreaInventoryBase(9, 36, a_ParentWindow)
	{
	}
} ;





/// Handles the armor area of the inventory
class cSlotAreaArmor :
	public cSlotAreaInventoryBase
{
public:
	cSlotAreaArmor(cWindow & a_ParentWindow)	:
		cSlotAreaInventoryBase(4, 5, a_ParentWindow)
	{
	}

	// Distributing the stack is allowed only for compatible items (helmets into helmet slot etc.)
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots) override;
} ;





/** A cSlotArea with items layout that is private to each player and is temporary, such as
a crafting grid or an enchantment table.
This common ancestor stores the items in a per-player map. It also implements tossing items from the map.
*/
class cSlotAreaTemporary :
	public cSlotArea
{
	typedef cSlotArea super;
	
public:
	cSlotAreaTemporary(int a_NumSlots, cWindow & a_ParentWindow);
	
	// cSlotArea overrides:
	virtual const cItem * GetSlot        (int a_SlotNum, cPlayer & a_Player) override;
	virtual void          SetSlot        (int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;
	virtual void          OnPlayerAdded  (cPlayer & a_Player) override;
	virtual void          OnPlayerRemoved(cPlayer & a_Player) override;
	
	/// Tosses the player's items in slots [a_Begin, a_End) (ie. incl. a_Begin, but excl. a_End)
	void TossItems(cPlayer & a_Player, int a_Begin, int a_End);
	
protected:
	typedef std::map<int, std::vector<cItem> > cItemMap;  // Maps EntityID -> items
	
	cItemMap m_Items;
	
	/// Returns the pointer to the slot array for the player specified.
	cItem * GetPlayerSlots(cPlayer & a_Player);
} ;





class cSlotAreaCrafting :
	public cSlotAreaTemporary
{
	typedef cSlotAreaTemporary super;
	
public:
	/// a_GridSize is allowed to be only 2 or 3
	cSlotAreaCrafting(int a_GridSize, cWindow & a_ParentWindow);

	// cSlotAreaTemporary overrides:
	virtual void Clicked        (cPlayer & a_Player, int a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, const cItem & a_ClickedItem) override;
	virtual void OnPlayerRemoved(cPlayer & a_Player) override;
	
	// Distributing items into this area is completely disabled
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots) override {}

protected:
	/// Maps player's EntityID -> current recipe; not a std::map because cCraftingGrid needs proper constructor params
	typedef std::list<std::pair<int, cCraftingRecipe> > cRecipeMap;
	
	int        m_GridSize;
	cRecipeMap m_Recipes;
	
	/// Handles a click in the result slot. Crafts using the current recipe, if possible
	void ClickedResult(cPlayer & a_Player);
	
	/// Handles a shift-click in the result slot. Crafts using the current recipe until it changes or no more space for result.
	void ShiftClickedResult(cPlayer & a_Player);
	
	/// Updates the current recipe and result slot based on the ingredients currently in the crafting grid of the specified player
	void UpdateRecipe(cPlayer & a_Player);
	
	/// Retrieves the recipe for the specified player from the map, or creates one if not found
	cCraftingRecipe & GetRecipeForPlayer(cPlayer & a_Player);
} ;





class cSlotAreaChest :
	public cSlotArea
{
public:
	cSlotAreaChest(cChestEntity * a_Chest, cWindow & a_ParentWindow);
	
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;
	
protected:
	cChestEntity * m_Chest;
} ;





class cSlotAreaDispenser :
	public cSlotArea
{
	typedef cSlotArea super;
	
public:
	cSlotAreaDispenser(cDispenserEntity * a_Dispenser, cWindow & a_ParentWindow);
	
	virtual void          Clicked(cPlayer & a_Player, int a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, const cItem & a_ClickedItem) override;
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;
	
protected:
	cDispenserEntity * m_Dispenser;
} ;





class cSlotAreaFurnace :
	public cSlotArea
{
	typedef cSlotArea super;
	
public:
	cSlotAreaFurnace(cFurnaceEntity * a_Furnace, cWindow & a_ParentWindow);
	
	virtual void          Clicked(cPlayer & a_Player, int a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, const cItem & a_ClickedItem) override;
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;
	
protected:
	cFurnaceEntity * m_Furnace;
} ;




