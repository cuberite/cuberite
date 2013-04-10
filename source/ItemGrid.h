
// ItemGrid.h

// Declares the cItemGrid class representing a storage for items in a XY grid (chests, dispensers, inventory etc.)




#pragma once

#include "Item.h"





// tolua_begin
class cItemGrid
{
public:
	// tolua_end
	cItemGrid(int a_Width, int a_Height);
	
	~cItemGrid();
	
	// tolua_begin
	int GetWidth   (void) const { return m_Width; }
	int GetHeight  (void) const { return m_Height; }
	int GetNumSlots(void) const { return m_NumSlots; }
	
	/// Converts XY coords into slot number; returns -1 on invalid coords
	int GetSlotNum(int a_X, int a_Y) const;
	
	// tolua_end
	/// Converts slot number into XY coords; sets coords to -1 on invalid slot number. Exported in ManualBindings.cpp
	void GetSlotCoords(int a_SlotNum, int & a_X, int & a_Y) const;
	// tolua_begin
	
	// Retrieve slots by coords or slot number; Logs warning and returns the first slot on invalid coords / slotnum
	const cItem & GetSlot(int a_X, int a_Y) const;
	cItem &       GetSlot(int a_X, int a_Y);
	const cItem & GetSlot(int a_SlotNum) const;
	cItem &       GetSlot(int a_SlotNum);
	
	// Set slot by coords or slot number; Logs warning and doesn't set on invalid coords / slotnum
	void SetSlot(int a_X, int a_Y, const cItem & a_Item);
	void SetSlot(int a_X, int a_Y, short a_ItemType, char a_ItemCount, short a_ItemDamage);
	void SetSlot(int a_SlotNum, const cItem & a_Item);
	void SetSlot(int a_SlotNum, short a_ItemType, char a_ItemCount, short a_ItemDamage);
	
	/// Sets all items as empty
	void Clear(void);
	
	/// Returns number of items out of a_ItemStack that can fit in the storage
	int HowManyCanFit(const cItem & a_ItemStack);
	
	/// Adds as many items out of a_ItemStack as can fit; the rest is left in a_ItemStack; returns true if any items fit.
	bool AddItem(cItem & a_ItemStack);
	
	/// Same as AddItem, but works on an entire list of item stacks
	bool AddItems(cItems & a_ItemStackList);
	
	/// Returns the index of the first empty slot; -1 if all full
	int GetFirstEmptySlot(void) const;

	/// Returns the index of the last empty slot; -1 if all full
	int GetLastEmptySlot(void) const;
	
	/// Returns the index of the first empty slot following a_StartFrom (a_StartFrom is not checked)
	int GetNextEmptySlot(int a_StartFrom) const;
	
	/// Copies the contents into a cItems object
	void CopyToItems(cItems & a_Items) const;

	// tolua_end
	
	/** Generates random loot from the specified loot probability table, with a chance of enchanted books added.
	A total of a_NumSlots are taken by the loot.
	Cannot export to Lua due to raw array a_LootProbabs.
	*/
	void GenerateRandomLootWithBooks(const cLootProbab * a_LootProbabs, int a_CountLootProbabs, int a_NumSlots, int a_Seed);

	// tolua_begin
	
protected:
	int     m_Width;
	int     m_Height;
	int     m_NumSlots;  // m_Width * m_Height, for easier validity checking in the access functions
	cItem * m_Slots;  // x + m_Width * y
} ;
// tolua_end



