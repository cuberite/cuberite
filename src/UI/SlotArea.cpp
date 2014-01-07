// SlotArea.cpp

// Implements the cSlotArea class and its descendants

#include "Globals.h"
#include "SlotArea.h"
#include "../Entities/Player.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/DropSpenserEntity.h"
#include "../BlockEntities/EnderChestEntity.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "../Items/ItemHandler.h"
#include "Window.h"
#include "../CraftingRecipes.h"
#include "../Root.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotArea:

cSlotArea::cSlotArea(int a_NumSlots, cWindow & a_ParentWindow) :
	m_NumSlots(a_NumSlots),
	m_ParentWindow(a_ParentWindow)
{
}





void cSlotArea::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	/*
	LOGD("Slot area with %d slots clicked at slot number %d, clicked item %s, slot item %s", 
		GetNumSlots(), a_SlotNum, 
		ItemToFullString(a_ClickedItem).c_str(),
		ItemToFullString(*GetSlot(a_SlotNum, a_Player)).c_str()
	);
	*/
	
	ASSERT((a_SlotNum >= 0) && (a_SlotNum < GetNumSlots()));

	bool bAsync = false;
	if (GetSlot(a_SlotNum, a_Player) == NULL)
	{
		LOGWARNING("GetSlot(%d) returned NULL! Ignoring click", a_SlotNum);
		return;
	}
	
	switch (a_ClickAction)
	{
		case caShiftLeftClick:
		case caShiftRightClick:
		{
			ShiftClicked(a_Player, a_SlotNum, a_ClickedItem);
			return;
		}
		
		case caDblClick:
		{
			DblClicked(a_Player, a_SlotNum);
			return;
		}
		default:
		{
			break;
		}
	}
	
	cItem Slot(*GetSlot(a_SlotNum, a_Player));
	if (!Slot.IsSameType(a_ClickedItem))
	{
		LOGWARNING("*** Window lost sync at item %d in SlotArea with %d items ***", a_SlotNum, m_NumSlots);
		LOGWARNING("My item:    %s", ItemToFullString(Slot).c_str());
		LOGWARNING("Their item: %s", ItemToFullString(a_ClickedItem).c_str());
		bAsync = true;
	}
	cItem & DraggingItem = a_Player.GetDraggingItem();
	switch (a_ClickAction)
	{
		case caRightClick:
		{
			if (DraggingItem.m_ItemType <= 0) // Empty-handed?
			{
				DraggingItem.m_ItemCount = (char)(((float)Slot.m_ItemCount) / 2.f + 0.5f);
				Slot.m_ItemCount -= DraggingItem.m_ItemCount;
				DraggingItem.m_ItemType = Slot.m_ItemType;
				DraggingItem.m_ItemDamage = Slot.m_ItemDamage;

				if (Slot.m_ItemCount <= 0)
				{
					Slot.Empty();
				}
			}
			else if ((Slot.m_ItemType <= 0) || DraggingItem.IsEqual(Slot))
			{
				// Drop one item in slot
				cItemHandler * Handler = ItemHandler(Slot.m_ItemType);
				if ((DraggingItem.m_ItemCount > 0) && (Slot.m_ItemCount < Handler->GetMaxStackSize()))
				{
					Slot.m_ItemType = DraggingItem.m_ItemType;
					Slot.m_ItemCount++;
					Slot.m_ItemDamage = DraggingItem.m_ItemDamage;
					DraggingItem.m_ItemCount--;
				}
				if (DraggingItem.m_ItemCount <= 0)
				{
					DraggingItem.Empty();
				}
			}
			else if (!DraggingItem.IsEqual(Slot))
			{
				// Swap contents
				cItem tmp(DraggingItem);
				DraggingItem = Slot;
				Slot = tmp;
			}
			break;
		}
		
		case caLeftClick:
		{
			// Left-clicked
			if (!DraggingItem.IsEqual(Slot))
			{
				// Switch contents
				cItem tmp(DraggingItem);
				DraggingItem = Slot;
				Slot = tmp;
			}
			else
			{
				// Same type, add items:
				cItemHandler * Handler = ItemHandler(DraggingItem.m_ItemType);
				int FreeSlots = Handler->GetMaxStackSize() - Slot.m_ItemCount;
				if (FreeSlots < 0)
				{
					ASSERT(!"Bad item stack size - where did we get more items in a slot than allowed?");
					FreeSlots = 0;
				}
				int Filling = (FreeSlots > DraggingItem.m_ItemCount) ? DraggingItem.m_ItemCount : FreeSlots;
				Slot.m_ItemCount += (char)Filling;
				DraggingItem.m_ItemCount -= (char)Filling;
				if (DraggingItem.m_ItemCount <= 0)
				{
					DraggingItem.Empty();
				}
			}
			break;
		}
		default:
		{
			LOGWARNING("SlotArea: Unhandled click action: %d (%s)", a_ClickAction, ClickActionToString(a_ClickAction));
			m_ParentWindow.BroadcastWholeWindow();
			return;
		}
	}  // switch (a_ClickAction
	
	SetSlot(a_SlotNum, a_Player, Slot);
	if (bAsync)
	{
		m_ParentWindow.BroadcastWholeWindow();
	}
	
}





void cSlotArea::ShiftClicked(cPlayer & a_Player, int a_SlotNum, const cItem & a_ClickedItem)
{
	// Make a copy of the slot, distribute it among the other areas, then update the slot to contain the leftover:
	cItem Slot(*GetSlot(a_SlotNum, a_Player));
	m_ParentWindow.DistributeStack(Slot, a_Player, this, true);
	if (Slot.IsEmpty())
	{
		// Empty the slot completely, the cilent doesn't like left-over ItemType with zero count
		Slot.Empty();
	}
	SetSlot(a_SlotNum, a_Player, Slot);
	
	// Some clients try to guess our actions and not always right (armor slots in 1.2.5), so we fix them:
	m_ParentWindow.BroadcastWholeWindow();
}





void cSlotArea::DblClicked(cPlayer & a_Player, int a_SlotNum)
{
	cItem & Dragging = a_Player.GetDraggingItem();
	if (Dragging.IsEmpty())
	{
		// Move the item in the dblclicked slot into hand:
		Dragging = *GetSlot(a_SlotNum, a_Player);
		cItem EmptyItem;
		SetSlot(a_SlotNum, a_Player, EmptyItem);
	}
	if (Dragging.IsEmpty())
	{
		LOGD("%s DblClicked with an empty hand over empty slot, ignoring", a_Player.GetName().c_str());
		return;
	}
	
	// Add as many items from the surrounding area into hand as possible:
	// First skip full stacks, then if there's still space, process full stacks as well:
	if (!m_ParentWindow.CollectItemsToHand(Dragging, *this, a_Player, false))
	{
		m_ParentWindow.CollectItemsToHand(Dragging, *this, a_Player, true);
	}
	
	m_ParentWindow.BroadcastWholeWindow();  // We need to broadcast, in case the window was a chest opened by multiple players
}





void cSlotArea::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_Apply, bool a_KeepEmptySlots)
{
	for (int i = 0; i < m_NumSlots; i++)
	{
		const cItem * Slot = GetSlot(i, a_Player);
		if (!Slot->IsStackableWith(a_ItemStack) && (!Slot->IsEmpty() || a_KeepEmptySlots))
		{
			// Different items
			continue;
		}
		int NumFit = ItemHandler(Slot->m_ItemType)->GetMaxStackSize() - Slot->m_ItemCount;
		if (NumFit <= 0)
		{
			// Full stack already
			continue;
		}
		if (NumFit > a_ItemStack.m_ItemCount)
		{
			NumFit = a_ItemStack.m_ItemCount;
		}
		if (a_Apply)
		{
			cItem NewSlot(a_ItemStack);
			NewSlot.m_ItemCount = Slot->m_ItemCount + NumFit;
			SetSlot(i, a_Player, NewSlot);
		}
		a_ItemStack.m_ItemCount -= NumFit;
		if (a_ItemStack.IsEmpty())
		{
			return;
		}
	}  // for i - Slots
}





bool cSlotArea::CollectItemsToHand(cItem & a_Dragging, cPlayer & a_Player, bool a_CollectFullStacks)
{
	int NumSlots = GetNumSlots();
	for (int i = 0; i < NumSlots; i++)
	{
		const cItem & SlotItem = *GetSlot(i, a_Player);
		if (!SlotItem.IsStackableWith(a_Dragging))
		{
			continue;
		}
		int ToMove = a_Dragging.GetMaxStackSize() - a_Dragging.m_ItemCount;
		if (ToMove > SlotItem.m_ItemCount)
		{
			ToMove = SlotItem.m_ItemCount;
		}
		a_Dragging.m_ItemCount += ToMove;
		cItem NewSlot(SlotItem);
		NewSlot.m_ItemCount -= ToMove;
		SetSlot(i, a_Player, NewSlot);
		if (!NewSlot.IsEmpty())
		{
			// There are leftovers in the slot, so a_Dragging must be full
			return true;
		}
	}  // for i - Slots[]
	// a_Dragging may be full if there were exactly the number of items needed to fill it
	return a_Dragging.IsFullStack();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaChest:

cSlotAreaChest::cSlotAreaChest(cChestEntity * a_Chest, cWindow & a_ParentWindow) :
	cSlotArea(27, a_ParentWindow),
	m_Chest(a_Chest)
{
}





const cItem * cSlotAreaChest::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	// a_SlotNum ranges from 0 to 26, use that to index the chest entity's inventory directly:
	return &(m_Chest->GetSlot(a_SlotNum));
}





void cSlotAreaChest::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	m_Chest->SetSlot(a_SlotNum, a_Item);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaDoubleChest:

cSlotAreaDoubleChest::cSlotAreaDoubleChest(cChestEntity * a_TopChest, cChestEntity * a_BottomChest, cWindow & a_ParentWindow) :
	cSlotArea(54, a_ParentWindow),
	m_TopChest(a_TopChest),
	m_BottomChest(a_BottomChest)
{
}





const cItem * cSlotAreaDoubleChest::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	// a_SlotNum ranges from 0 to 53, use that to index the correct chest's inventory:
	if (a_SlotNum < 27)
	{
		return &(m_TopChest->GetSlot(a_SlotNum));
	}
	else
	{
		return &(m_BottomChest->GetSlot(a_SlotNum - 27));
	}
}





void cSlotAreaDoubleChest::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	if (a_SlotNum < 27)
	{
		m_TopChest->SetSlot(a_SlotNum, a_Item);
	}
	else
	{
		m_BottomChest->SetSlot(a_SlotNum - 27, a_Item);
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaCrafting:

cSlotAreaCrafting::cSlotAreaCrafting(int a_GridSize, cWindow & a_ParentWindow) :
	cSlotAreaTemporary(1 + a_GridSize * a_GridSize, a_ParentWindow),
	m_GridSize(a_GridSize)
{
	ASSERT((a_GridSize == 2) || (a_GridSize == 3));
}





void cSlotAreaCrafting::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	// Override for craft result slot
	if (a_SlotNum == 0)
	{
		if ((a_ClickAction == caShiftLeftClick) || (a_ClickAction == caShiftRightClick))
		{
			ShiftClickedResult(a_Player);
		}
		else
		{
			ClickedResult(a_Player);
		}
		return;
	}
	super::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
	UpdateRecipe(a_Player);
}





void cSlotAreaCrafting::DblClicked(cPlayer & a_Player, int a_SlotNum)
{
	if (a_SlotNum == 0)
	{
		// Dbl-clicking the crafting result slot shouldn't collect items to hand
		return;
	}
	super::DblClicked(a_Player, a_SlotNum);
}





void cSlotAreaCrafting::OnPlayerRemoved(cPlayer & a_Player)
{
	// Toss all items on the crafting grid:
	TossItems(a_Player, 1, m_NumSlots);
	
	// Remove the current recipe from the player -> recipe map:
	for (cRecipeMap::iterator itr = m_Recipes.begin(), end = m_Recipes.end(); itr != end; ++itr)
	{
		if (itr->first == a_Player.GetUniqueID())
		{
			// Remove the player from the recipe map:
			m_Recipes.erase(itr);
			return;
		}
	}  // for itr - m_Recipes[]
	// Player not found - that is acceptable
}





void cSlotAreaCrafting::ClickedResult(cPlayer & a_Player)
{
	cItem & DraggingItem = a_Player.GetDraggingItem();

	// Get the current recipe:
	cCraftingRecipe & Recipe = GetRecipeForPlayer(a_Player);

	cItem * PlayerSlots = GetPlayerSlots(a_Player) + 1;
	cCraftingGrid Grid(PlayerSlots, m_GridSize, m_GridSize);

	// If possible, craft:
	if (DraggingItem.IsEmpty())
	{
		DraggingItem = Recipe.GetResult();
		Recipe.ConsumeIngredients(Grid);
		Grid.CopyToItems(PlayerSlots);
	}
	else if (DraggingItem.IsEqual(Recipe.GetResult()))
	{
		cItemHandler * Handler = ItemHandler(Recipe.GetResult().m_ItemType);
		if (DraggingItem.m_ItemCount + Recipe.GetResult().m_ItemCount <= Handler->GetMaxStackSize())
		{
			DraggingItem.m_ItemCount += Recipe.GetResult().m_ItemCount;
			Recipe.ConsumeIngredients(Grid);
			Grid.CopyToItems(PlayerSlots);
		}
	}

	// Get the new recipe and update the result slot:
	UpdateRecipe(a_Player);
	
	// We're done. Send all changes to the client and bail out:
	m_ParentWindow.BroadcastWholeWindow();
}





void cSlotAreaCrafting::ShiftClickedResult(cPlayer & a_Player)
{
	cItem Result(*GetSlot(0, a_Player));
	if (Result.IsEmpty())
	{
		return;
	}
	cItem * PlayerSlots = GetPlayerSlots(a_Player) + 1;
	for (;;)
	{
		// Try distributing the result. If it fails, bail out:
		cItem ResultCopy(Result);
		m_ParentWindow.DistributeStack(ResultCopy, a_Player, this, false);
		if (!ResultCopy.IsEmpty())
		{
			// Couldn't distribute all of it. Bail out
			return;
		}
		
		// Distribute the result, this time for real:
		ResultCopy = Result;
		m_ParentWindow.DistributeStack(ResultCopy, a_Player, this, true);
		
		// Remove the ingredients from the crafting grid and update the recipe:
		cCraftingRecipe & Recipe = GetRecipeForPlayer(a_Player);
		cCraftingGrid Grid(PlayerSlots, m_GridSize, m_GridSize);
		Recipe.ConsumeIngredients(Grid);
		Grid.CopyToItems(PlayerSlots);
		UpdateRecipe(a_Player);
		if (!Recipe.GetResult().IsEqual(Result))
		{
			// The recipe has changed, bail out
			return;
		}
	}
}





void cSlotAreaCrafting::UpdateRecipe(cPlayer & a_Player)
{
	cCraftingGrid   Grid(GetPlayerSlots(a_Player) + 1, m_GridSize, m_GridSize);
	cCraftingRecipe & Recipe = GetRecipeForPlayer(a_Player);
	cRoot::Get()->GetCraftingRecipes()->GetRecipe(&a_Player, Grid, Recipe);
	SetSlot(0, a_Player, Recipe.GetResult());
	m_ParentWindow.SendSlot(a_Player, this, 0);
}





cCraftingRecipe & cSlotAreaCrafting::GetRecipeForPlayer(cPlayer & a_Player)
{
	for (cRecipeMap::iterator itr = m_Recipes.begin(), end = m_Recipes.end(); itr != end; ++itr)
	{
		if (itr->first == a_Player.GetUniqueID())
		{
			return itr->second;
		}
	}  // for itr - m_Recipes[]
	
	// Not found. Add a new one:
	cCraftingGrid   Grid(GetPlayerSlots(a_Player) + 1, m_GridSize, m_GridSize);
	cCraftingRecipe Recipe(Grid);
	cRoot::Get()->GetCraftingRecipes()->GetRecipe(&a_Player, Grid, Recipe);
	m_Recipes.push_back(std::make_pair(a_Player.GetUniqueID(), Recipe));
	return m_Recipes.back().second;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaEnderChest:

cSlotAreaEnderChest::cSlotAreaEnderChest(cEnderChestEntity * a_EnderChest, cWindow & a_ParentWindow) :
	cSlotArea(27, a_ParentWindow),
	m_EnderChest(a_EnderChest)
{
}





const cItem * cSlotAreaEnderChest::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	// a_SlotNum ranges from 0 to 26, use that to index the chest entity's inventory directly:
	return &(m_EnderChest->GetSlot(a_SlotNum));
}





void cSlotAreaEnderChest::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	m_EnderChest->SetSlot(a_SlotNum, a_Item);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaFurnace:

cSlotAreaFurnace::cSlotAreaFurnace(cFurnaceEntity * a_Furnace, cWindow & a_ParentWindow) :
	cSlotArea(3, a_ParentWindow),
	m_Furnace(a_Furnace)
{
	m_Furnace->GetContents().AddListener(*this);
}





cSlotAreaFurnace::~cSlotAreaFurnace()
{
	m_Furnace->GetContents().RemoveListener(*this);
}





void cSlotAreaFurnace::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	super::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
	
	if (m_Furnace == NULL)
	{
		LOGERROR("cSlotAreaFurnace::Clicked(): m_Furnace == NULL");
		ASSERT(!"cSlotAreaFurnace::Clicked(): m_Furnace == NULL");
		return;
	}
}





const cItem * cSlotAreaFurnace::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	// a_SlotNum ranges from 0 to 2, query the items from the underlying furnace:
	return &(m_Furnace->GetSlot(a_SlotNum));
}





void cSlotAreaFurnace::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	m_Furnace->SetSlot(a_SlotNum, a_Item);
}





void cSlotAreaFurnace::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	// Something has changed in the window, broadcast the entire window to all clients
	ASSERT(a_ItemGrid == &(m_Furnace->GetContents()));
	
	m_ParentWindow.BroadcastWholeWindow();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaInventoryBase:

cSlotAreaInventoryBase::cSlotAreaInventoryBase(int a_NumSlots, int a_SlotOffset, cWindow & a_ParentWindow) :
	cSlotArea(a_NumSlots, a_ParentWindow),
	m_SlotOffset(a_SlotOffset)
{
}





void cSlotAreaInventoryBase::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	if (a_Player.IsGameModeCreative() && (m_ParentWindow.GetWindowType() == cWindow::wtInventory))
	{
		// Creative inventory must treat a_ClickedItem as a DraggedItem instead, replacing the inventory slot with it
		SetSlot(a_SlotNum, a_Player, a_ClickedItem);
		return;
	}
	
	// Survival inventory and all other windows' inventory has the same handling as normal slot areas
	super::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
	return;
}





const cItem * cSlotAreaInventoryBase::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	// a_SlotNum ranges from 0 to 35, map that to the player's inventory slots according to the internal offset
	return &a_Player.GetInventory().GetSlot(a_SlotNum + m_SlotOffset);
}





void cSlotAreaInventoryBase::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	a_Player.GetInventory().SetSlot(a_SlotNum + m_SlotOffset, a_Item);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaArmor:

void cSlotAreaArmor::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots)
{
	if (ItemCategory::IsHelmet(a_ItemStack.m_ItemType) && GetSlot(0, a_Player)->IsEmpty())
	{
		if (a_ShouldApply)
		{
			SetSlot(0, a_Player, a_ItemStack.CopyOne());
		}
		a_ItemStack.m_ItemCount -= 1;
	}
	else if (ItemCategory::IsChestPlate(a_ItemStack.m_ItemType) && GetSlot(1, a_Player)->IsEmpty())
	{
		if (a_ShouldApply)
		{
			SetSlot(1, a_Player, a_ItemStack.CopyOne());
		}
		a_ItemStack.m_ItemCount -= 1;
	}
	else if (ItemCategory::IsLeggings(a_ItemStack.m_ItemType) && GetSlot(2, a_Player)->IsEmpty())
	{
		if (a_ShouldApply)
		{
			SetSlot(2, a_Player, a_ItemStack.CopyOne());
		}
		a_ItemStack.m_ItemCount -= 1;
	}
	else if (ItemCategory::IsBoots(a_ItemStack.m_ItemType) && GetSlot(3, a_Player)->IsEmpty())
	{
		if (a_ShouldApply)
		{
			SetSlot(3, a_Player, a_ItemStack.CopyOne());
		}
		a_ItemStack.m_ItemCount -= 1;
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaItemGrid:

cSlotAreaItemGrid::cSlotAreaItemGrid(cItemGrid & a_ItemGrid, cWindow & a_ParentWindow) :
	super(a_ItemGrid.GetNumSlots(), a_ParentWindow),
	m_ItemGrid(a_ItemGrid)
{
	m_ItemGrid.AddListener(*this);
}





cSlotAreaItemGrid::~cSlotAreaItemGrid()
{
	m_ItemGrid.RemoveListener(*this);
}





const cItem * cSlotAreaItemGrid::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	return &m_ItemGrid.GetSlot(a_SlotNum);
}





void cSlotAreaItemGrid::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	m_ItemGrid.SetSlot(a_SlotNum, a_Item);
}





void cSlotAreaItemGrid::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	ASSERT(a_ItemGrid == &m_ItemGrid);
	m_ParentWindow.BroadcastSlot(this, a_SlotNum);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSlotAreaTemporary:

cSlotAreaTemporary::cSlotAreaTemporary(int a_NumSlots, cWindow & a_ParentWindow) :
	cSlotArea(a_NumSlots, a_ParentWindow)
{
}





const cItem * cSlotAreaTemporary::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	cItemMap::const_iterator itr = m_Items.find(a_Player.GetUniqueID());
	if (itr == m_Items.end())
	{
		LOGERROR("cSlotAreaTemporary: player \"%s\" not found for slot %d!", a_Player.GetName().c_str(), a_SlotNum);
		ASSERT(!"cSlotAreaTemporary: player not found!");
		
		// Player not found, this should not happen, ever! Return NULL, but things may break by this.
		return NULL;
	}
	
	if (a_SlotNum >= (int)(itr->second.size()))
	{
		LOGERROR("cSlotAreaTemporary: asking for more slots than actually stored!");
		ASSERT(!"cSlotAreaTemporary: asking for more slots than actually stored!");
		return NULL;
	}
	
	return &(itr->second[a_SlotNum]);
}





void cSlotAreaTemporary::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	cItemMap::iterator itr = m_Items.find(a_Player.GetUniqueID());
	if (itr == m_Items.end())
	{
		// Player not found
		LOGWARNING("cSlotAreaTemporary: player not found!");
		return;
	}
	
	if (a_SlotNum >= (int)(itr->second.size()))
	{
		LOGERROR("cSlotAreaTemporary: asking for more slots than actually stored!");
		return;
	}
	
	itr->second[a_SlotNum] = a_Item;
}





void cSlotAreaTemporary::OnPlayerAdded(cPlayer & a_Player)
{
	ASSERT(m_Items.find(a_Player.GetUniqueID()) == m_Items.end());  // The player shouldn't be in the itemmap, otherwise we probably have a leak
	m_Items[a_Player.GetUniqueID()].resize(m_NumSlots);  // Make the vector the specified size of empty items
}





void cSlotAreaTemporary::OnPlayerRemoved(cPlayer & a_Player)
{
	cItemMap::iterator itr = m_Items.find(a_Player.GetUniqueID());
	ASSERT(itr != m_Items.end());  // The player should be in the list, otherwise a call to OnPlayerAdded() was mismatched
	m_Items.erase(itr);
}





void cSlotAreaTemporary::TossItems(cPlayer & a_Player, int a_Begin, int a_End)
{
	cItemMap::iterator itr = m_Items.find(a_Player.GetUniqueID());
	if (itr == m_Items.end())
	{
		LOGWARNING("Player tossing items (%s) not found in the item map", a_Player.GetName().c_str());
		return;
	}
	
	cItems Drops;
	for (int i = a_Begin; i < a_End; i++)
	{
		cItem & Item = itr->second[i];
		if (!Item.IsEmpty())
		{
			Drops.push_back(Item);
		}
		Item.Empty();
	}  // for i - itr->second[]
	
	double vX = 0, vY = 0, vZ = 0;
	EulerToVector(-a_Player.GetRotation(), a_Player.GetPitch(), vZ, vX, vY);
	vY = -vY * 2 + 1.f;
	a_Player.GetWorld()->SpawnItemPickups(Drops, a_Player.GetPosX(), a_Player.GetPosY() + 1.6f, a_Player.GetPosZ(), vX * 3, vY * 3, vZ * 3, true); // 'true' because player created
}





cItem * cSlotAreaTemporary::GetPlayerSlots(cPlayer & a_Player)
{
	cItemMap::iterator itr = m_Items.find(a_Player.GetUniqueID());
	if (itr == m_Items.end())
	{
		return NULL;
	}
	return &(itr->second[0]);
}




