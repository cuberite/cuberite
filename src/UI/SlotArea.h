
// SlotArea.h

// Interfaces to the cSlotArea class representing a contiguous area of slots in a UI window




#pragma once

#include "../Inventory.h"





class cHorse;
class cWindow;
class cPlayer;
class cBeaconEntity;
class cBrewingstandEntity;
class cChestEntity;
class cEnderChestEntity;
class cFurnaceEntity;
class cMinecartWithChest;
class cCraftingRecipe;
class cWorld;





class cSlotArea
{
public:
	cSlotArea(int a_NumSlots, cWindow & a_ParentWindow);
	virtual ~cSlotArea() {}  // force a virtual destructor in all subclasses

	int GetNumSlots(void) const { return m_NumSlots; }

	/** Called to retrieve an item in the specified slot for the specified player. Must return a valid cItem. */
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const = 0;

	/** Called to set an item in the specified slot for the specified player */
	virtual void SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) = 0;

	/** Called when a player clicks in the window. Parameters taken from the click packet. */
	virtual void Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem);

	/** Called from Clicked when the action is a shiftclick (left or right) */
	virtual void ShiftClicked(cPlayer & a_Player, int a_SlotNum, const cItem & a_ClickedItem);

	/** Called from Clicked when the action is a caDblClick */
	virtual void DblClicked(cPlayer & a_Player, int a_SlotNum);

	/** Called from Clicked when the action is a middleclick */
	virtual void MiddleClicked(cPlayer & a_Player, int a_SlotNum);

	/** Called from Clicked when the action is a drop click. */
	virtual void DropClicked(cPlayer & a_Player, int a_SlotNum, bool a_DropStack);

	/** Called from Clicked when the action is a number click. */
	virtual void NumberClicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction);

	/** Called when a new player opens the same parent window. The window already tracks the player. CS-locked. */
	virtual void OnPlayerAdded(cPlayer & a_Player);

	/** Called when one of the players closes the parent window. The window already doesn't track the player. CS-locked. */
	virtual void OnPlayerRemoved(cPlayer & a_Player);

	/** Called to store as much of a_ItemStack in the area as possible. a_ItemStack is modified to reflect the change.
	The default implementation searches each slot for available space and distributes the stack there.
	if a_ShouldApply is true, the changes are written into the slots;
	if a_ShouldApply is false, only a_ItemStack is modified to reflect the number of fits (for fit-testing purposes)
	If a_KeepEmptySlots is true, empty slots will be skipped and won't be filled */
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill);

	/** Called on DblClicking to collect all stackable items into hand.
	The items are accumulated in a_Dragging and removed from the slots immediately.
	If a_CollectFullStacks is false, slots with full stacks are skipped while collecting.
	Returns true if full stack has been collected in a_Dragging, false if there's space remaining to fill. */
	virtual bool CollectItemsToHand(cItem & a_Dragging, cPlayer & a_Player, bool a_CollectFullStacks);

protected:
	int       m_NumSlots;
	cWindow & m_ParentWindow;
} ;





/** Handles any part of the inventory, using parameters in constructor to distinguish between the parts */
class cSlotAreaInventoryBase:
	public cSlotArea
{
	using Super = cSlotArea;

public:

	cSlotAreaInventoryBase(int a_NumSlots, int a_SlotOffset, cWindow & a_ParentWindow);

	// Creative inventory's click handling is somewhat different from survival inventory's, handle that here:
	virtual void Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;

	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

protected:
	int m_SlotOffset;  // Index that this area's slot 0 has in the underlying cInventory
} ;





/** Handles the main inventory of each player, excluding the armor and hotbar */
class cSlotAreaInventory:
	public cSlotAreaInventoryBase
{
	using Super = cSlotAreaInventoryBase;

public:

	cSlotAreaInventory(cWindow & a_ParentWindow):
		Super(cInventory::invInventoryCount, cInventory::invInventoryOffset, a_ParentWindow)
	{
	}
} ;





/** Handles the hotbar of each player */
class cSlotAreaHotBar:
	public cSlotAreaInventoryBase
{
	using Super = cSlotAreaInventoryBase;

public:
	cSlotAreaHotBar(cWindow & a_ParentWindow):
		Super(cInventory::invHotbarCount, cInventory::invHotbarOffset, a_ParentWindow)
	{
	}
} ;





/** Handles the shield of each player */
class cSlotAreaShield:
	public cSlotAreaInventoryBase
{
	using Super = cSlotAreaInventoryBase;

public:

	cSlotAreaShield(cWindow & a_ParentWindow):
		Super(cInventory::invShieldCount, cInventory::invShieldOffset, a_ParentWindow)
	{
	}
};





/** Handles the armor area of the player's inventory */
class cSlotAreaArmor:
	public cSlotAreaInventoryBase
{
	using Super = cSlotAreaInventoryBase;

public:

	cSlotAreaArmor(cWindow & a_ParentWindow):
		Super(cInventory::invArmorCount, cInventory::invArmorOffset, a_ParentWindow)
	{
	}

	/** Distributing the stack is allowed only for compatible items (helmets into helmet slot etc.) */
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill) override;

	/** Called when a player clicks in the window. Parameters taken from the click packet. */
	virtual void Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;

	static bool CanPlaceArmorInSlot(int a_SlotNum, const cItem & a_Item);
} ;





/** Handles any slot area that is representing a cItemGrid; same items for all the players */
class cSlotAreaItemGrid:
	public cSlotArea,
	public cItemGrid::cListener
{
	using Super = cSlotArea;

public:

	cSlotAreaItemGrid(cItemGrid & a_ItemGrid, cWindow & a_ParentWindow);

	virtual ~cSlotAreaItemGrid() override;

	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

protected:
	cItemGrid & m_ItemGrid;

	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) override;
} ;





/** A cSlotArea with items layout that is private to each player and is temporary, such as
a crafting grid or an enchantment table.
This common ancestor stores the items in a per-player map. It also implements tossing items from the map. */
class cSlotAreaTemporary:
	public cSlotArea
{
	using Super = cSlotArea;

public:

	cSlotAreaTemporary(int a_NumSlots, cWindow & a_ParentWindow);

	// cSlotArea overrides:
	virtual const cItem * GetSlot        (int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot        (int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;
	virtual void          OnPlayerAdded  (cPlayer & a_Player) override;
	virtual void          OnPlayerRemoved(cPlayer & a_Player) override;

	/** Tosses the player's items in slots [a_Begin, a_End) (ie. incl. a_Begin, but excl. a_End) */
	void TossItems(cPlayer & a_Player, int a_Begin, int a_End);

protected:
	using cItemMap = std::map<UInt32, std::vector<cItem> >;  // Maps EntityID -> items

	cItemMap m_Items;

	/** Returns the pointer to the slot array for the player specified. */
	cItem * GetPlayerSlots(cPlayer & a_Player);
} ;





class cSlotAreaCrafting:
	public cSlotAreaTemporary
{
	using Super = cSlotAreaTemporary;

public:

	/** a_GridSize is allowed to be only 2 or 3 */
	cSlotAreaCrafting(int a_GridSize, cWindow & a_ParentWindow);

	// cSlotAreaTemporary overrides:
	virtual void Clicked        (cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;
	virtual void DblClicked     (cPlayer & a_Player, int a_SlotNum) override;
	virtual void OnPlayerRemoved(cPlayer & a_Player) override;
	virtual void SetSlot        (int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

	// Distributing items into this area is completely disabled
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill) override;

	/** Clear the crafting grid */
	void ClearCraftingGrid(cPlayer & a_Player);

	/** Loads the given Recipe into the crafting grid */
	void LoadRecipe(cPlayer & a_Player, UInt32 a_RecipeId);

protected:
	/** Maps player's EntityID -> current recipe.
	Not a std::map because cCraftingGrid needs proper constructor params. */
	typedef std::list<std::pair<UInt32, cCraftingRecipe> > cRecipeMap;

	int        m_GridSize;
	cRecipeMap m_Recipes;

	/** Handles a click in the result slot.
	Crafts using the current recipe, if possible. */
	void ClickedResult(cPlayer & a_Player);

	/** Handles a shift-click in the result slot.
	Crafts using the current recipe until it changes or no more space for result. */
	void ShiftClickedResult(cPlayer & a_Player);

	/** Handles a drop-click in the result slot. */
	void DropClickedResult(cPlayer & a_Player);

	/** Updates the current recipe and result slot based on the ingredients currently in the crafting grid of the specified player. */
	void UpdateRecipe(cPlayer & a_Player);

	/** Retrieves the recipe for the specified player from the map, or creates one if not found. */
	cCraftingRecipe & GetRecipeForPlayer(cPlayer & a_Player);

	/** Called after an item has been crafted to handle statistics e.t.c. */
	void HandleCraftItem(const cItem & a_Result, cPlayer & a_Player);
} ;





class cSlotAreaAnvil:
	public cSlotAreaTemporary
{
	using Super = cSlotAreaTemporary;

public:

	cSlotAreaAnvil(cWindow & a_ParentWindow);

	// cSlotArea overrides:
	virtual void Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;
	virtual void ShiftClicked(cPlayer & a_Player, int a_SlotNum, const cItem & a_ClickedItem) override;
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill) override;

	// cSlotAreaTemporary overrides:
	virtual void OnPlayerRemoved(cPlayer & a_Player) override;

	/** Can the player take the item from the slot? */
	bool CanTakeResultItem(cPlayer & a_Player);

	/** This function will call, when the player take the item from the slot. */
	void OnTakeResult(cPlayer & a_Player);

	/** Handles a click in the item slot. */
	void UpdateResult(cPlayer & a_Player);

protected:
	/** The maximum cost of repairing / renaming in the anvil. */
	int m_MaximumCost;

	/** The stack size of the second item where was used for repair */
	char m_StackSizeToBeUsedInRepair;
} ;





class cSlotAreaBeacon:
	public cSlotArea,
	public cItemGrid::cListener
{
	using Super = cSlotArea;

public:

	cSlotAreaBeacon(cBeaconEntity * a_Beacon, cWindow & a_ParentWindow);
	virtual ~cSlotAreaBeacon() override;

	static bool IsPlaceableItem(short a_ItemType);

	virtual void          Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;
	virtual void          DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill) override;
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

protected:
	cBeaconEntity * m_Beacon;

	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) override;
} ;





class cSlotAreaEnchanting final :
	public cSlotAreaTemporary
{
	using Super = cSlotAreaTemporary;

public:

	cSlotAreaEnchanting(cWindow & a_ParentWindow, Vector3i a_BlockPos);

	// cSlotArea overrides:
	virtual void Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill) override;
	virtual void SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

	// cSlotAreaTemporary overrides:
	virtual void OnPlayerAdded  (cPlayer & a_Player) override;
	virtual void OnPlayerRemoved(cPlayer & a_Player) override;

	/* Get the number of bookshelves which are near the enchanting table */
	unsigned GetBookshelvesCount(cWorld & a_World);

	/* Return the enchanted item matching the chosen option (0, 1, 2)
	Ownership of the cItem is transferred to the caller. */
	cItem SelectEnchantedOption(size_t a_EnchantOption);

protected:

	/** Handles a click in the item slot. */
	void UpdateResult(cPlayer & a_Player);

	Vector3i m_BlockPos;
	std::array<cItem, 3> m_EnchantedItemOptions;
};





class cSlotAreaChest :
	public cSlotArea
{
public:
	cSlotAreaChest(cChestEntity * a_Chest, cWindow & a_ParentWindow);

	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

protected:
	cChestEntity * m_Chest;
} ;





class cSlotAreaDoubleChest :
	public cSlotArea
{
public:
	cSlotAreaDoubleChest(cChestEntity * a_TopChest, cChestEntity * a_BottomChest, cWindow & a_ParentWindow);

	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

protected:
	cChestEntity * m_TopChest;
	cChestEntity * m_BottomChest;
} ;





class cSlotAreaEnderChest :
	public cSlotArea
{
public:
	cSlotAreaEnderChest(cEnderChestEntity * a_EnderChest, cWindow & a_ParentWindow);

	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

protected:
	cEnderChestEntity * m_EnderChest;
};





class cSlotAreaFurnace:
	public cSlotArea,
	public cItemGrid::cListener
{
	using Super = cSlotArea;

public:

	cSlotAreaFurnace(cFurnaceEntity * a_Furnace, cWindow & a_ParentWindow);

	virtual ~cSlotAreaFurnace() override;

	virtual void          Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;
	virtual void          DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill) override;
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

protected:
	cFurnaceEntity * m_Furnace;

	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) override;

	/** Called after an item has been smelted to handle statistics etc. */
	void HandleSmeltItem(const cItem & a_Result, cPlayer & a_Player);
} ;





class cSlotAreaBrewingstand:
	public cSlotArea,
	public cItemGrid::cListener
{
	using Super = cSlotArea;

public:

	cSlotAreaBrewingstand(cBrewingstandEntity * a_Brewingstand, cWindow & a_ParentWindow);

	virtual ~cSlotAreaBrewingstand() override;

	virtual void          Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;
	virtual void          DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill) override;
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;
protected:
	cBrewingstandEntity * m_Brewingstand;

	// cItemGrid::cListener overrides:
	virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) override;

	/** Called after an item has been brewed to handle statistics etc. */
	void HandleBrewedItem(cPlayer & a_Player, const cItem & a_ClickedItem);
} ;





class cSlotAreaMinecartWithChest :
	public cSlotArea
{
public:
	cSlotAreaMinecartWithChest(cMinecartWithChest * a_ChestCart, cWindow & a_ParentWindow);

	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void          SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;

protected:
	cMinecartWithChest * m_Chest;
};





/** Slot area holding horse saddle and armor. */
class cSlotAreaHorse:
	public cSlotArea
{
public:
	enum
	{
		SaddleSlot,
		ArmorSlot
	};

	cSlotAreaHorse(cHorse & a_Horse, cWindow & a_ParentWindow);
	virtual void Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;
	virtual const cItem * GetSlot(int a_SlotNum, cPlayer & a_Player) const override;
	virtual void SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item) override;
	virtual void DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill) override;
private:
	cHorse & m_Horse;
};
