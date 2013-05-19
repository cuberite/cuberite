
#pragma once

#include "Item.h"





namespace Json
{
	class Value;
};

class cClientHandle;
class cPlayer;





class cInventory										// tolua_export	
{														// tolua_export
public:
	cInventory(cPlayer & a_Owner);
	~cInventory();

	void Clear();										// tolua_export

	// cItem * GetSlotsForType( int a_Type );
	// int GetSlotCountForType( int a_Type );

	bool AddItem( cItem & a_Item );						// tolua_export
	bool AddItemAnyAmount( cItem & a_Item );			// tolua_export
	bool RemoveItem( cItem & a_Item );					// tolua_export

	void SaveToJson(Json::Value & a_Value);
	bool LoadFromJson(Json::Value & a_Value);

	void SendWholeInventory(cClientHandle & a_Client);

	const cItem * GetSlots(void) const { return m_Slots; }

	// tolua_begin
	
	const cItem & GetSlot(int a_SlotNum) const;
	const cItem & GetHotBarSlot(int a_HotBarSlotNum) const;
	const cItem & GetEquippedItem(void) const;
	void          SetSlot(int a_SlotNum, const cItem & a_Item);
	void          SetHotBarSlot(int a_HotBarSlotNum, const cItem & a_Item);
	
	void          SetEquippedSlotNum(int a_SlotNum);
	int           GetEquippedSlotNum(void) { return m_EquippedSlotNum; }
	
	/// Adds the specified damage to the specified item; deletes the item and returns true if the item broke.
	bool cInventory::DamageItem(int a_SlotNum, short a_Amount);

	/// Adds the specified damage to the currently held item; deletes the item and returns true if the item broke.
	bool DamageEquippedItem(short a_Amount = 1);
	
	const cItem & GetEquippedHelmet    (void) const { return m_Slots[c_ArmorOffset]; }
	const cItem & GetEquippedChestplate(void) const { return m_Slots[c_ArmorOffset + 1]; }
	const cItem & GetEquippedLeggings  (void) const { return m_Slots[c_ArmorOffset + 2]; }
	const cItem & GetEquippedBoots     (void) const { return m_Slots[c_ArmorOffset + 3]; }

	// tolua_end

	void SendSlot( int a_SlotNum );						// tolua_export
	
	/// Returns how many items of the specified type would fit into the slot range specified
	int HowManyCanFit(short a_ItemType, short a_ItemDamage, int a_BeginSlot, int a_EndSlot);
	
	/// Moves items, fitting them into the slot range specified, up to a_Count items. Returns the number of items moved
	int MoveItem(short a_ItemType, short a_ItemDamage, int a_Count, int a_BeginSlot, int a_EndSlot);

	static const unsigned int c_NumSlots = 45;
	static const unsigned int c_MainSlots = 27;
	static const unsigned int c_HotSlots = 9;
	static const unsigned int c_CraftSlots = 4;
	static const unsigned int c_ArmorSlots = 4;

	static const unsigned int c_CraftOffset = 0;
	static const unsigned int c_ArmorOffset = 5;
	static const unsigned int c_MainOffset = 9;
	static const unsigned int c_HotOffset = 36;
	
	/// Converts a slot number into the ID for the EntityEquipment packet
	static int SlotNumToEntityEquipmentID(short a_SlotNum);

protected:
	bool AddToBar( cItem & a_Item, const int a_Offset, const int a_Size, bool* a_bChangedSlots, int a_Mode = 0 );

	cItem m_Slots[c_NumSlots];
	
	cItem * m_MainSlots;
	cItem * m_CraftSlots;
	cItem * m_ArmorSlots;
	cItem * m_HotSlots;

	int m_EquippedSlotNum;

	cPlayer & m_Owner;
};	// tolua_export




