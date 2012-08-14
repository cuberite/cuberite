
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSurvivalInventory.h"
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cWindow.h"
#include "cItem.h"
#include "CraftingRecipes.h"
#include "cRoot.h"
#include "packets/cPacket_WindowClick.h"





cSurvivalInventory::cSurvivalInventory(cPlayer* a_Owner)
	: cInventory(a_Owner)
{
}





cSurvivalInventory::~cSurvivalInventory()
{
}





void cSurvivalInventory::Clicked( cPacket* a_ClickPacket )
{
	cPacket_WindowClick * Packet = reinterpret_cast<cPacket_WindowClick *>(a_ClickPacket);
	
	if (
		Packet->m_IsShiftPressed &&   // Shift pressed
		(GetWindow() != NULL) &&      // Window is valid
		(GetWindow()->GetDraggingItem()->IsEmpty())  // Not dragging anything
	)
	{
		ShiftClicked(Packet);
		return;
	}
	
	bool bDontCook = false;
	if (GetWindow())
	{
		// Override for craft result slot
		if (Packet->m_SlotNum == (short)c_CraftOffset)
		{
			LOGD("In craft slot: %i x %i !!", m_Slots[c_CraftOffset].m_ItemID, m_Slots[c_CraftOffset].m_ItemCount );
			cItem * DraggingItem = GetWindow()->GetDraggingItem();
			if (DraggingItem->IsEmpty())
			{
				*DraggingItem = m_Slots[c_CraftOffset];
				m_Slots[c_CraftOffset].Empty();
			}
			else if (DraggingItem->Equals(m_Slots[c_CraftOffset]))
			{
				if (DraggingItem->m_ItemCount + m_Slots[c_CraftOffset].m_ItemCount <= 64)
				{
					DraggingItem->m_ItemCount += m_Slots[c_CraftOffset].m_ItemCount;
					m_Slots[0].Empty();
				}
				else
				{
					bDontCook = true;
				}
			}
			else
			{
				bDontCook = true;
			}
			LOGD("Dragging Dish %i", DraggingItem->m_ItemCount );
		}
		else
		{
			GetWindow()->Clicked( Packet, *m_Owner );
		}
	}
	else
	{
		ASSERT(!"No inventory window! WTF?");
		LOG("No Inventory window! WTF");
	}

	if ((Packet->m_SlotNum >= (short)c_CraftOffset) && (Packet->m_SlotNum < (short)(c_CraftOffset + c_CraftSlots + 1)))
	{
		cCraftingGrid   Grid(m_Slots + c_CraftOffset + 1, 2, 2);
		cCraftingRecipe Recipe(Grid);
		
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(m_Owner, Grid, Recipe);
		
		if ((Packet->m_SlotNum == 0) && !bDontCook)
		{
			// Consume the items from the crafting grid:
			Recipe.ConsumeIngredients(Grid);
			
			// Propagate grid back to m_Slots:
			Grid.CopyToItems(m_Slots + c_CraftOffset + 1);
			
			// Get the recipe for the new grid contents:
			cRoot::Get()->GetCraftingRecipes()->GetRecipe(m_Owner, Grid, Recipe);
		}
		m_Slots[c_CraftOffset] = Recipe.GetResult();
		LOGD("%s cooked: %i x %i !!", m_Owner->GetName().c_str(), m_Slots[c_CraftOffset].m_ItemID, m_Slots[c_CraftOffset].m_ItemCount );
		SendWholeInventory( m_Owner->GetClientHandle() );
	}
	SendSlot(0);
}





void cSurvivalInventory::ShiftClicked(cPacket_WindowClick * a_ClickPacket)
{
	ASSERT((GetWindow() == NULL) || (GetWindow()->GetDraggingItem()->IsEmpty()));  // Cannot handle shift-click if dragging something
	
	short Slot = a_ClickPacket->m_SlotNum;
	if (Slot == SLOT_CRAFTING_RESULT)
	{
		ShiftClickedCraftingResult(Slot);
	}
	else if ((Slot >= SLOT_CRAFTING_MIN) && (Slot <= SLOT_CRAFTING_MAX))
	{
		ShiftClickedCraftingGrid(Slot);
	}
	else if ((Slot >= SLOT_ARMOR_MIN) && (Slot <= SLOT_ARMOR_MAX))
	{
		ShiftClickedArmor(Slot);
	}
	else if ((Slot >= SLOT_HOTBAR_MIN) && (Slot <= SLOT_HOTBAR_MAX))
	{
		ShiftClickedHotbar(Slot);
	}
	else
	{
		ShiftClickedInventory(Slot);
	}
	// Because the client tries to guess our actions, not always right, send the whole inventory:
	SendWholeInventoryToAll();
}





void cSurvivalInventory::ShiftClickedCraftingResult(short a_Slot)
{
	// Craft until either the recipe changes (due to ingredients) or there's not enough storage for the result
	cItem * CraftingResult = GetSlot(SLOT_CRAFTING_RESULT);
	if ((CraftingResult == NULL) || CraftingResult->IsEmpty())
	{
		return;
	}
	cItem ResultCopy = *CraftingResult;
	int HowManyItemsWillFit = HowManyCanFit(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	HowManyItemsWillFit += HowManyCanFit(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
	int HowManyPassesWillFit = HowManyItemsWillFit / CraftingResult->m_ItemCount;
	for (int i = 0; i < HowManyPassesWillFit; i++)
	{
		// First try moving into the hotbar:
		int NumMoved = MoveItem(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, CraftingResult->m_ItemCount, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
		
		// If something didn't fit, move into main inventory:
		if (NumMoved < CraftingResult->m_ItemCount)
		{
			MoveItem(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, CraftingResult->m_ItemCount - NumMoved, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
		}
		
		// "Use" the crafting recipe once:
		cCraftingGrid   Grid(m_Slots + SLOT_CRAFTING_MIN, 2, 2);
		cCraftingRecipe Recipe(Grid);
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(m_Owner, Grid, Recipe);
		Recipe.ConsumeIngredients(Grid);
		Grid.CopyToItems(m_Slots + c_CraftOffset + 1);
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(m_Owner, Grid, Recipe);
		m_Slots[SLOT_CRAFTING_RESULT] = Recipe.GetResult();
		
		// If the recipe changed, abort:
		if (!Recipe.GetResult().Equals(ResultCopy))
		{
			break;
		}
	}
}





void cSurvivalInventory::ShiftClickedCraftingGrid(short a_Slot)
{
	// Move the item from the crafting grid into the main inventory:
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	// First try the main inventory:
	Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	
	// If anything left, try the hotbar:
	if (Item->m_ItemCount > 0)
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
	}
	SendSlot(a_Slot);
}





void cSurvivalInventory::ShiftClickedArmor(short a_Slot)
{
	// Move the item from the armor slot into the main inventory:
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	SendSlot(a_Slot);
}





void cSurvivalInventory::ShiftClickedHotbar(short a_Slot)
{
	// Move the item from the hotbar into the main inventory:
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	SendSlot(a_Slot);
}





void cSurvivalInventory::ShiftClickedInventory(short a_Slot)
{
	// Move the item from the main inventory into armor slot if it is armor, or the hotbar otherwise:
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	if (ItemCategory::IsHelmet(Item->m_ItemID))
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_ARMOR_HELMET, SLOT_ARMOR_HELMET);
	}
	else if (ItemCategory::IsChestPlate(Item->m_ItemID))
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_ARMOR_CHESTPLATE, SLOT_ARMOR_CHESTPLATE);
	}
	else if (ItemCategory::IsLeggings(Item->m_ItemID))
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_ARMOR_LEGGINGS, SLOT_ARMOR_LEGGINGS);
	}
	else if (ItemCategory::IsBoots(Item->m_ItemID))
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_ARMOR_BOOTS, SLOT_ARMOR_BOOTS);
	}
	else
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
	}
	SendSlot(a_Slot);
}




