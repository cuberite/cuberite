
// ItemGrid.h

// Declares the cItemGrid class representing a storage for items in a XY grid (chests, dispensers, inventory etc.)




#pragma once

#include "Item.h"
#include "LazyArray.h"





// tolua_begin
class cItemGrid
{
public:
	// tolua_end

	/** This class is used as a callback for when a slot changes */
	class cListener
	{
	public:
		virtual ~cListener() {}

		/** Called whenever a slot changes */
		virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) = 0;
	} ;
	typedef std::vector<cListener *> cListeners;

	cItemGrid(int a_Width, int a_Height);

	// tolua_begin
	int GetWidth   (void) const { return m_Width; }
	int GetHeight  (void) const { return m_Height; }
	int GetNumSlots(void) const { return m_Slots.size(); }

	/** Converts XY coords into slot number; returns -1 on invalid coords */
	int GetSlotNum(int a_X, int a_Y) const;

	// tolua_end

	/** Converts slot number into XY coords; sets coords to -1 on invalid slot number. Exported in ManualBindings.cpp */
	void GetSlotCoords(int a_SlotNum, int & a_X, int & a_Y) const;

	/** Copies all items from a_Src to this grid.
	Doesn't copy the listeners. */
	void CopyFrom(const cItemGrid & a_Src);

	// tolua_begin

	// Retrieve slots by coords or slot number; Logs warning and returns the first slot on invalid coords / slotnum
	const cItem & GetSlot(int a_X, int a_Y) const;
	const cItem & GetSlot(int a_SlotNum) const;

	// Set slot by coords or slot number; Logs warning and doesn't set on invalid coords / slotnum
	void SetSlot(int a_X, int a_Y, const cItem & a_Item);
	void SetSlot(int a_X, int a_Y, short a_ItemType, char a_ItemCount, short a_ItemDamage);
	void SetSlot(int a_SlotNum, const cItem & a_Item);
	void SetSlot(int a_SlotNum, short a_ItemType, char a_ItemCount, short a_ItemDamage);

	// Empty the specified slot; Logs warning and doesn't set on invalid coords / slotnum
	void EmptySlot(int a_X, int a_Y);
	void EmptySlot(int a_SlotNum);

	/** Returns true if the specified slot is empty or the slot doesn't exist */
	bool IsSlotEmpty(int a_SlotNum) const;

	/** Returns true if the specified slot is empty or the slot doesn't exist */
	bool IsSlotEmpty(int a_X, int a_Y) const;

	/** Sets all items as empty */
	void Clear(void);

	/** Returns number of items out of a_ItemStack that can fit in the storage */
	int HowManyCanFit(const cItem & a_ItemStack, bool a_AllowNewStacks = true);

	/** Adds as many items out of a_ItemStack as can fit.
	If a_AllowNewStacks is set to false, only existing stacks can be topped up;
	If a_AllowNewStacks is set to true, empty slots can be used for the rest.
	If a_PrioritySlot is set to a positive value, then the corresponding slot will be used  first (if empty or compatible with added items).
	If a_PrioritySlot is set to -1, regular order applies.
	Returns the number of items that fit.
	*/
	int AddItem(cItem & a_ItemStack, bool a_AllowNewStacks = true, int a_PrioritySlot = -1);

	/** Same as AddItem, but works on an entire list of item stacks.
	The a_ItemStackList is modified to reflect the leftover items.
	If a_AllowNewStacks is set to false, only existing stacks can be topped up;
	If a_AllowNewStacks is set to true, empty slots can be used for the rest.
	If a_PrioritySlot is set to a positive value, then the corresponding slot will be used first (if empty or compatible with added items).
	If a_PrioritySlot is set to -1, regular order applies.
	Returns the total number of items that fit.
	*/
	int AddItems(cItems & a_ItemStackList, bool a_AllowNewStacks = true, int a_PrioritySlot = -1);

	/** Removes the specified item from the grid, as many as possible, up to a_ItemStack.m_ItemCount.
	Returns the number of items that were removed. */
	int RemoveItem(const cItem & a_ItemStack);

	/** Finds an item based on ItemType and ItemDamage (<- defines the itemType, too) */
	cItem * FindItem(const cItem & a_RecipeItem);

	/** Adds (or subtracts, if a_AddToCount is negative) to the count of items in the specified slot.
	If the slot is empty, ignores the call.
	Returns the new count.
	*/
	int ChangeSlotCount(int a_SlotNum, int a_AddToCount);

	/** Adds (or subtracts, if a_AddToCount is negative) to the count of items in the specified slot.
	If the slot is empty, ignores the call.
	Returns the new count.
	*/
	int ChangeSlotCount(int a_X, int a_Y, int a_AddToCount);

	/** Removes one item from the stack in the specified slot, and returns it.
	If the slot was empty, returns an empty item
	*/
	cItem RemoveOneItem(int a_SlotNum);

	/** Removes one item from the stack in the specified slot, and returns it.
	If the slot was empty, returns an empty item
	*/
	cItem RemoveOneItem(int a_X, int a_Y);

	/** Returns the number of items of type a_Item that are stored */
	int HowManyItems(const cItem & a_Item);

	/** Returns true if there are at least as many items of type a_ItemStack as in a_ItemStack */
	bool HasItems(const cItem & a_ItemStack);

	/** Returns the index of the first empty slot; -1 if all full */
	int GetFirstEmptySlot(void) const;

	/** Returns the index of the first non-empty slot; -1 if all empty */
	int GetFirstUsedSlot(void) const;

	/** Returns the index of the last empty slot; -1 if all full */
	int GetLastEmptySlot(void) const;

	/** Returns the index of the last used slot; -1 if all empty */
	int GetLastUsedSlot(void) const;

	/** Returns the index of the first empty slot following a_StartFrom (a_StartFrom is not checked) */
	int GetNextEmptySlot(int a_StartFrom) const;

	/** Returns the index of the first used slot following a_StartFrom (a_StartFrom is not checked) */
	int GetNextUsedSlot(int a_StartFrom) const;

	/** Copies the contents into a cItems object; preserves the original a_Items contents */
	void CopyToItems(cItems & a_Items) const;

	/** Adds the specified damage to the specified item; returns true if the item broke (but the item is left intact) */
	bool DamageItem(int a_SlotNum, short a_Amount);

	/** Adds the specified damage to the specified item; returns true if the item broke (but the item is left intact) */
	bool DamageItem(int a_X, int a_Y, short a_Amount);

	// tolua_end


	/** Returns true if slot coordinates lie within the grid. */
	bool IsValidSlotCoords(int a_X, int a_Y) const;

	/** Returns true if slot number is within the grid. */
	bool IsValidSlotNum(int a_SlotNum) const;

	/** Generates random loot from the specified loot probability table, with a chance of enchanted books added.
	A total of a_NumSlots are taken by the loot.
	Cannot export to Lua due to raw array a_LootProbabs. TODO: Make this exportable / export through ManualBindings.cpp with a Lua table as LootProbabs
	*/
	void GenerateRandomLootWithBooks(const cLootProbab * a_LootProbabs, size_t a_CountLootProbabs, int a_NumSlots, int a_Seed);

	/** Adds a callback that gets called whenever a slot changes. Must not be called from within the listener callback! */
	void AddListener(cListener & a_Listener);

	/** Removes a slot-change-callback. Must not be called from within the listener callback! */
	void RemoveListener(cListener & a_Listener);

	// tolua_begin

protected:
	int     m_Width;
	int     m_Height;
	cLazyArray<cItem> m_Slots;

	cListeners       m_Listeners;    ///< Listeners which should be notified on slot changes; the pointers are not owned by this object
	cCriticalSection m_CSListeners;  ///< CS that guards the m_Listeners against multi-thread access
	bool             m_IsInTriggerListeners;  ///< Set to true while TriggerListeners is running, to detect attempts to manipulate listener list while triggerring

	/** Calls all m_Listeners for the specified slot number */
	void TriggerListeners(int a_SlotNum);

	/** Adds up to a_Num items out of a_ItemStack, as many as can fit, in specified slot
	Returns the number of items that did fit.
	*/
	int AddItemToSlot(const cItem & a_ItemStack, int a_Slot, int a_Num, int a_MaxStack);
} ;
// tolua_end
