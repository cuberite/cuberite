
#pragma once

#include "ItemGrid.h"





namespace Json
{
	class Value;
};

class cPlayer;




// tolua_begin

/** This class represents the player's inventory
The slots are divided into three areas:
- armor slots     (1 x 4)
- inventory slots (9 x 3)
- hotbar slots    (9 x 1)
The generic GetSlot(), SetSlot() and HowManyCanFit() functions take the index of the slots,
as if armor slots, inventory slots and then hotbar slots were put one after another.
You can use the invArmorOffset, invInventoryOffset and invHotbarOffset constants.
*/

class cInventory :
	// tolua_end
	public cItemGrid::cListener
	// tolua_begin
{
public:

	// Counts and offsets to individual parts of the inventory, as used by GetSlot() / SetSlot() / HowManyCanFit():
	enum
	{
		invArmorCount      = 4,
		invInventoryCount  = 9 * 3,
		invHotbarCount     = 9,
		invShieldCount     = 1,      // Number of slots in shield slots grid

		invArmorOffset     = 0,
		invInventoryOffset = invArmorOffset     + invArmorCount,
		invHotbarOffset    = invInventoryOffset + invInventoryCount,
		invShieldOffset    = invHotbarOffset    + invHotbarCount,     // Offset where shield slots start
		invNumSlots        = invShieldOffset    + invShieldCount
	} ;

	// tolua_end

	cInventory(cPlayer & a_Owner);

	virtual ~cInventory() override {}

	// tolua_begin

	/** Removes all items from the entire inventory */
	void Clear(void);

	/** Returns number of items out of a_ItemStack that can fit in the storage */
	int HowManyCanFit(const cItem & a_ItemStack, bool a_ConsiderEmptySlots = true);

	/** Returns how many items of the specified type would fit into the slot range specified */
	int HowManyCanFit(const cItem & a_ItemStack, int a_BeginSlotNum, int a_EndSlotNum, bool a_ConsiderEmptySlots = true);

	/** Adds as many items out of a_ItemStack as can fit.
	If a_AllowNewStacks is set to false, only existing stacks can be topped up;
	if a_AllowNewStacks is set to true, empty slots can be used for the rest.
	Fills existing stacks first and fills the hotbar before the main inventory.
	Returns the number of items that fit.
	*/
	int AddItem(const cItem & a_ItemStack, bool a_AllowNewStacks = true);

	/** Same as AddItem, but works on an entire list of item stacks.
	The a_ItemStackList is modified to reflect the leftover items.
	If a_AllowNewStacks is set to false, only existing stacks can be topped up;
	if a_AllowNewStacks is set to true, empty slots can be used for the rest.
	Returns the total number of items that fit.
	*/
	int AddItems(cItems & a_ItemStackList, bool a_AllowNewStacks);

	/** Removes the specified item from the inventory, as many as possible, up to a_ItemStack.m_ItemCount.
	Returns the number of items that were removed. */
	int RemoveItem(const cItem & a_ItemStack);

	/** Finds an item based on ItemType and ItemDamage (<- defines the itemType, too) */
	cItem * FindItem(const cItem & a_RecipeItem);

	/** Removes one item out of the currently equipped item stack, returns true if successful, false if empty-handed */
	bool RemoveOneEquippedItem(void);

	/** Removes one item from the the current equipped item stack, and attempts to add the specified item stack
	back to the same slot. If it is not possible to place the item in the same slot, optionally (default true) tries to
	place the specified item elsewhere in the inventory. Returns the number of items successfully added. If the
	currently equipped slot is empty, its contents are simply set to the given Item.
	*/
	int ReplaceOneEquippedItem(const cItem & a_Item, bool a_TryOtherSlots = true);

	/** Returns the number of items of type a_Item that are stored */
	int HowManyItems(const cItem & a_Item);

	/** Returns true if there are at least as many items of type a_ItemStack as in a_ItemStack */
	bool HasItems(const cItem & a_ItemStack);

	/** Sends the equipped item slot to the client */
	void SendEquippedSlot();

	/** Returns the cItemGrid object representing the armor slots */
	cItemGrid & GetArmorGrid(void) { return m_ArmorSlots; }

	/** Returns the cItemGrid object representing the main inventory slots */
	cItemGrid & GetInventoryGrid(void) { return m_InventorySlots; }

	/** Returns the cItemGrid object representing the hotbar slots */
	cItemGrid & GetHotbarGrid(void) { return m_HotbarSlots; }

	/** Returns the player associated with this inventory */
	cPlayer & GetOwner(void) { return m_Owner; }

	/** Copies the non-empty slots into a_ItemStacks; preserves the original a_Items contents */
	void CopyToItems(cItems & a_Items);

	// tolua_end

	/** Returns the player associated with this inventory (const version) */
	const cPlayer & GetOwner(void) const { return m_Owner; }

	// tolua_begin

	/** Returns current item in a_SlotNum slot */
	const cItem & GetSlot(int a_SlotNum) const;
	/** Returns current item in a_ArmorSlotNum in armor slots */
	const cItem & GetArmorSlot(int a_ArmorSlotNum) const;
	/** Returns current item in a_ArmorSlotNum in inventory slots */
	const cItem & GetInventorySlot(int a_InventorySlotNum) const;
	/** Returns current item in a_ArmorSlotNum in hotbar slots */
	const cItem & GetHotbarSlot(int a_HotBarSlotNum) const;
	/** Returns current item in shield slot */
	const cItem & GetShieldSlot() const;
	/** Returns current equiped item */
	const cItem & GetEquippedItem(void) const;
	/** Puts a_Item item in a_SlotNum slot number */
	void          SetSlot(int a_SlotNum, const cItem & a_Item);
	/** Puts a_Item item in a_ArmorSlotNum slot number in armor slots */
	void          SetArmorSlot(int a_ArmorSlotNum, const cItem & a_Item);
	/** Puts a_Item item in a_InventorySlotNum slot number in inventory slots */
	void          SetInventorySlot(int a_InventorySlotNum, const cItem & a_Item);
	/** Puts a_Item item in a_HotBarSlotNum slot number in hotbar slots */
	void          SetHotbarSlot(int a_HotBarSlotNum, const cItem & a_Item);
	/** Sets current item in shield slot */
	void          SetShieldSlot(const cItem & a_Item);
	/** Sets current item in the equipped hotbar slot */
	void          SetEquippedItem(const cItem & a_Item);
	/** Sets equiped item to the a_SlotNum slot number */
	void          SetEquippedSlotNum(int a_SlotNum);
	/** Returns slot number of equiped item */
	int           GetEquippedSlotNum(void) { return m_EquippedSlotNum; }

	/** Adds (or subtracts, if a_AddToCount is negative) to the count of items in the specified slot.
	If the slot is empty, ignores the call.
	Returns the new count, or -1 if the slot number is invalid.
	*/
	int ChangeSlotCount(int a_SlotNum, int a_AddToCount);

	/** Adds the specified damage to the specified item; deletes the item and returns true if the item broke. */
	bool DamageItem(int a_SlotNum, short a_Amount);

	/** Adds the specified damage to the currently held item; deletes the item and returns true if the item broke. */
	bool DamageEquippedItem(short a_Amount = 1);

	const cItem & GetEquippedHelmet    (void) const { return m_ArmorSlots.GetSlot(0); }
	const cItem & GetEquippedChestplate(void) const { return m_ArmorSlots.GetSlot(1); }
	const cItem & GetEquippedLeggings  (void) const { return m_ArmorSlots.GetSlot(2); }
	const cItem & GetEquippedBoots     (void) const { return m_ArmorSlots.GetSlot(3); }

	// tolua_end

	/** Sends the slot contents to the owner */
	void SendSlot(int a_SlotNum);

	/** Update items (e.g. Maps) */
	void UpdateItems(void);

	/** Converts an armor slot number into the ID for the EntityEquipment packet */
	static int ArmorSlotNumToEntityEquipmentID(short a_ArmorSlotNum);

	void SaveToJson(Json::Value & a_Value);
	bool LoadFromJson(Json::Value & a_Value);

protected:
	bool AddToBar(cItem & a_Item, const int a_Offset, const int a_Size, bool * a_bChangedSlots, int a_Mode = 0);

	cItemGrid m_ArmorSlots;
	cItemGrid m_InventorySlots;
	cItemGrid m_HotbarSlots;
	cItemGrid m_ShieldSlots;

	int m_EquippedSlotNum;

	cPlayer & m_Owner;

	/** Returns the ItemGrid and the (grid-local) slot number for a (global) slot number; return nullptr for invalid SlotNum */
	const cItemGrid * GetGridForSlotNum(int a_SlotNum, int & a_GridSlotNum) const;

	/** Returns the ItemGrid and the (grid-local) slot number for a (global) slot number; return nullptr for invalid SlotNum */
	cItemGrid * GetGridForSlotNum(int a_SlotNum, int & a_GridSlotNum);

	// cItemGrid::cListener override:
	virtual void OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum) override;
};  // tolua_export
