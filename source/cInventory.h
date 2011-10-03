#pragma once

#include "MemoryLeak.h"
#include "cWindowOwner.h"
#include "FileDefine.h"

class cItem;
class cClientHandle;
class cPlayer;
class cPacket_WindowClick;
class cPacket_EntityEquipment;
class cInventory										//tolua_export
	: public cWindowOwner		
{														//tolua_export
public:
	cInventory(cPlayer* a_Owner);
	~cInventory();

	void Clear();										//tolua_export

	cItem* GetSlotsForType( int a_Type );
	int GetSlotCountForType( int a_Type );

	bool AddItem( cItem & a_Item );						//tolua_export
	bool RemoveItem( cItem & a_Item );					//tolua_export
	void DrawInventory();

	void WriteToFile(FILE* a_File);
	bool LoadFromFile(FILE* a_File);

	void SendWholeInventory( cClientHandle* a_Client );

	cItem* GetSlot( int a_SlotNum );					//tolua_export
	cItem* GetSlots() { return m_Slots; }
	cItem* GetFromHotBar( int a_SlotNum );				//tolua_export

	cItem & GetEquippedItem();							//tolua_export
	void SetEquippedSlot( int a_SlotNum );				//tolua_export

	void Clicked( cPacket_WindowClick* a_ClickPacket );

	void SendSlot( int a_SlotNum );						//tolua_export

	static const unsigned int c_NumSlots = 45;
	static const unsigned int c_MainSlots = 27;
	static const unsigned int c_HotSlots = 9;
	static const unsigned int c_CraftSlots = 4;
	static const unsigned int c_ArmorSlots = 4;

	static const unsigned int c_CraftOffset = 0;
	static const unsigned int c_ArmorOffset = 5;
	static const unsigned int c_MainOffset = 9;
	static const unsigned int c_HotOffset = 36;
private:
	bool AddToBar( cItem & a_Item, const int a_Offset, const int a_Size, bool* a_bChangedSlots, int a_Mode = 0 );

	cItem* m_Slots;
	cItem* m_MainSlots;
	cItem* m_CraftSlots;
	cItem* m_ArmorSlots;
	cItem* m_HotSlots;

	cItem* m_EquippedItem;
	short m_EquippedSlot;

	cPlayer* m_Owner;
};	//tolua_export