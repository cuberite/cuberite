
#pragma once

#include "cInventory.h"





class cSurvivalInventory										//tolua_export
	: public cInventory
{														//tolua_export

	enum
	{
		SLOT_CRAFTING_RESULT  = 0,
		SLOT_CRAFTING_MIN     = 1,
		SLOT_CRAFTING_MAX     = 4,
		SLOT_ARMOR_MIN        = 5,
		SLOT_ARMOR_HELMET     = 5,
		SLOT_ARMOR_CHESTPLATE = 6,
		SLOT_ARMOR_LEGGINGS   = 7,
		SLOT_ARMOR_BOOTS      = 8,
		SLOT_ARMOR_MAX        = 8,
		SLOT_INVENTORY_MIN    = 9,
		SLOT_INVENTORY_MAX    = 35,
		SLOT_HOTBAR_MIN       = 36,
		SLOT_HOTBAR_MAX       = 44,
	} ;
	
	void ShiftClickedCraftingResult(short a_Slot);
	void ShiftClickedCraftingGrid  (short a_Slot);
	void ShiftClickedArmor         (short a_Slot);
	void ShiftClickedHotbar        (short a_Slot);
	void ShiftClickedInventory     (short a_Slot);
	
public:
	cSurvivalInventory(cPlayer* a_Owner);
	~cSurvivalInventory();

	virtual void Clicked(cPacket * a_ClickPacket) override;
	
	void ShiftClicked(cPacket_WindowClick * a_ClickPacket);
};	//tolua_export




