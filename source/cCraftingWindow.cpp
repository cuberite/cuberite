
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cCraftingWindow.h"
#include "cItem.h"
#include "CraftingRecipes.h"
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cInventory.h"
#include "cPickup.h"
#include "cRoot.h"
#include "cWorld.h"
#include "items/Item.h"





/* These numbers are valid for the underlying cInventory object, because that's where we're sending the 
MoveItem() and HowManyCanFit() function calls
*/
enum
{
	SLOT_INVENTORY_MIN = 9,
	SLOT_INVENTORY_MAX = 35,
	SLOT_HOTBAR_MIN    = 36,
	SLOT_HOTBAR_MAX    = 44
} ;





cCraftingWindow::cCraftingWindow( cWindowOwner* a_Owner, bool a_bInventoryVisible )
	 : cWindow(a_Owner, a_bInventoryVisible, cWindow::Workbench, 1)
{
	cItem * Slots = new cItem[10];
	SetSlots( Slots, 10 );
}





void cCraftingWindow::Clicked(
	cPlayer & a_Player, 
	int a_WindowID, short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, 
	const cItem & a_HeldItem
)
{
	bool bDontCook = false;
	
	cItem * DraggingItem = GetDraggingItem(&a_Player);
	if (
		a_IsShiftPressed && 
		((DraggingItem == NULL) || DraggingItem->IsEmpty())
	)
	{
		ShiftClicked(a_Player, a_SlotNum);
		return;
	}
	
	// Override for craft result slot
	if (a_SlotNum == 0)
	{
		LOGD("Clicked in craft result slot, item there: %d:%d (%d times) !!", GetSlot(0)->m_ItemID, GetSlot(0)->m_ItemHealth, GetSlot(0)->m_ItemCount);
		cItem * DraggingItem = GetDraggingItem(&a_Player);
		if (DraggingItem->IsEmpty())
		{
			*DraggingItem = *GetSlot(0);
			GetSlot(0)->Empty();
		}
		else if (DraggingItem->IsEqual(*GetSlot(0)))
		{
			cItemHandler * Handler = ItemHandler(GetSlot(0)->m_ItemID);
			if (DraggingItem->m_ItemCount + GetSlot(0)->m_ItemCount <= Handler->GetMaxStackSize())
			{
				DraggingItem->m_ItemCount += GetSlot(0)->m_ItemCount;
				GetSlot(0)->Empty();
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
	}
	else
	{
		cWindow::Clicked(a_Player, GetWindowID(), a_SlotNum, a_IsRightClick, a_IsShiftPressed, a_HeldItem);
	}

	if ((a_SlotNum >= 0) && (a_SlotNum < 10))
	{
		cCraftingGrid   Grid(GetSlots() + 1, 3, 3);
		cCraftingRecipe Recipe(Grid);
		
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(&a_Player, Grid, Recipe);

		if ((a_SlotNum == 0) && !bDontCook)
		{
			// Consume the items from the crafting grid:
			Recipe.ConsumeIngredients(Grid);
			
			// Propagate grid back to m_Slots:
			Grid.CopyToItems(GetSlots() + 1);
			
			// Get the recipe for the new grid contents:
			cRoot::Get()->GetCraftingRecipes()->GetRecipe(&a_Player, Grid, Recipe);
		}
		*GetSlot(0) = Recipe.GetResult();
		LOGD("%s cooked: %d:%d (%d times) !!", a_Player.GetName().c_str(), GetSlot(0)->m_ItemID, GetSlot(0)->m_ItemHealth, GetSlot(0)->m_ItemCount);
	}
	SendWholeWindow( a_Player.GetClientHandle() );
	a_Player.GetInventory().SendWholeInventory( a_Player.GetClientHandle() );

	// Separate packet for result =/ Don't know why
	a_Player.GetClientHandle()->SendInventorySlot((char)GetWindowID(), 0, *GetSlot(0));
}





void cCraftingWindow::Close(cPlayer & a_Player)
{
	// Start from slot 1, don't drop what's in the result slot
	cItems Drops;
	for( int i = 1; i < GetNumSlots(); i++ )
	{
		cItem * Item = GetSlot(i);
		if (!Item->IsEmpty())
		{
			Drops.push_back(*Item);
		}
		Item->Empty();
	}
	float vX = 0, vY = 0, vZ = 0;
	EulerToVector( -a_Player.GetRotation(), a_Player.GetPitch(), vZ, vX, vY);
	vY = -vY*2 + 1.f;
	a_Player.GetWorld()->SpawnItemPickups(Drops, a_Player.GetPosX(), a_Player.GetPosY() + 1.6f, a_Player.GetPosZ(), vX * 2, vY * 2, vZ * 2);
	cWindow::Close(a_Player);
}





void cCraftingWindow::ShiftClicked(cPlayer & a_Player, short a_SlotNum)
{
	if (a_SlotNum == SLOT_CRAFTING_RESULT)
	{
		ShiftClickedCraftingResult(a_Player, a_SlotNum);
	}
	else if ((a_SlotNum >= SLOT_CRAFTING_MIN) && (a_SlotNum <= SLOT_CRAFTING_MAX))
	{
		ShiftClickedCraftingGrid(a_Player, a_SlotNum);
	}
	else
	{
		// No need to handle inventory shift-click, it is handled by the underlying cSurvivalInventory, surprise surprise ;)
	}
	SendWholeWindow(a_Player.GetClientHandle());
}





void cCraftingWindow::ShiftClickedCraftingResult(cPlayer & a_Player, short a_Slot)
{
	// Craft until either the recipe changes (due to ingredients) or there's not enough storage for the result
	cInventory & Inventory = a_Player.GetInventory();
	cItem * CraftingResult = GetSlot(SLOT_CRAFTING_RESULT);
	if ((CraftingResult == NULL) || CraftingResult->IsEmpty())
	{
		return;
	}
	cItem ResultCopy = *CraftingResult;
	int HowManyItemsWillFit = Inventory.HowManyCanFit(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	HowManyItemsWillFit += Inventory.HowManyCanFit(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
	int HowManyPassesWillFit = HowManyItemsWillFit / CraftingResult->m_ItemCount;
	for (int i = 0; i < HowManyPassesWillFit; i++)
	{
		// First try moving into the hotbar:
		int NumMoved = Inventory.MoveItem(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, CraftingResult->m_ItemCount, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
		
		// If something didn't fit, move into main inventory:
		if (NumMoved < CraftingResult->m_ItemCount)
		{
			NumMoved += Inventory.MoveItem(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, CraftingResult->m_ItemCount - NumMoved, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
			ASSERT(NumMoved == CraftingResult->m_ItemCount);  // We checked earlier that we can fit this many items
		}
		
		// "Use" the crafting recipe once:
		cCraftingGrid   Grid(GetSlots() + 1, 3, 3);
		cCraftingRecipe Recipe(Grid);
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(&a_Player, Grid, Recipe);
		Recipe.ConsumeIngredients(Grid);
		Grid.CopyToItems(GetSlots() + 1);
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(&a_Player, Grid, Recipe);
		GetSlots()[SLOT_CRAFTING_RESULT] = Recipe.GetResult();
		
		// If the recipe changed, abort:
		if (!Recipe.GetResult().IsEqual(ResultCopy))
		{
			break;
		}
	}
}





void cCraftingWindow::ShiftClickedCraftingGrid(cPlayer & a_Player, short a_Slot)
{
	cInventory & Inventory = a_Player.GetInventory();
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	
	// First try the main inventory:
	Item->m_ItemCount -= Inventory.MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	
	// If anything left, try the hotbar:
	if (Item->m_ItemCount > 0)
	{
		Item->m_ItemCount -= Inventory.MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
	}
	if (Item->m_ItemCount == 0)
	{
		Item->Empty();
	}
}




