
// SlotArea.cpp

// Implements the cSlotArea class and its descendants

#include "Globals.h"
#include "SlotArea.h"
#include "../Entities/Player.h"
#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/BrewingstandEntity.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/DropSpenserEntity.h"
#include "../BlockEntities/EnderChestEntity.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "../Entities/Minecart.h"
#include "../Items/ItemHandler.h"
#include "AnvilWindow.h"
#include "../CraftingRecipes.h"
#include "../Root.h"
#include "../FastRandom.h"
#include "../BlockArea.h"
#include "../EffectID.h"
#include "../ClientHandle.h"
#include "../Mobs/Horse.h"





////////////////////////////////////////////////////////////////////////////////
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
	if (GetSlot(a_SlotNum, a_Player) == nullptr)
	{
		LOGWARNING("GetSlot(%d) returned nullptr! Ignoring click", a_SlotNum);
		return;
	}

	if (a_Player.IsGameModeSpectator())
	{
		// Block the action of the player and make sure, the inventory doesn't get out of sync
		a_Player.GetClientHandle()->SendInventorySlot(-1, -1, cItem());  // Reset the dragged item
		SetSlot(a_SlotNum, a_Player, *GetSlot(a_SlotNum, a_Player));  // Update the current slot
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
		case caMiddleClick:
		{
			MiddleClicked(a_Player, a_SlotNum);
			return;
		}
		case caDropKey:
		case caCtrlDropKey:
		{
			DropClicked(a_Player, a_SlotNum, (a_ClickAction == caCtrlDropKey));
			return;
		}
		case caNumber1:
		case caNumber2:
		case caNumber3:
		case caNumber4:
		case caNumber5:
		case caNumber6:
		case caNumber7:
		case caNumber8:
		case caNumber9:
		{
			NumberClicked(a_Player, a_SlotNum, a_ClickAction);
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
			if (DraggingItem.m_ItemType <= 0)  // Empty-handed?
			{
				DraggingItem = Slot.CopyOne();  // Obtain copy of slot to preserve lore, enchantments, etc.

				DraggingItem.m_ItemCount = static_cast<char>(static_cast<float>(Slot.m_ItemCount) / 2.f + 0.5f);
				Slot.m_ItemCount -= DraggingItem.m_ItemCount;

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
					char OldSlotCount = Slot.m_ItemCount;

					Slot = DraggingItem.CopyOne();  // See above
					OldSlotCount++;
					Slot.m_ItemCount = OldSlotCount;

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
				char Filling = static_cast<char>((FreeSlots > DraggingItem.m_ItemCount) ? DraggingItem.m_ItemCount : FreeSlots);

				Slot.m_ItemCount += Filling;
				DraggingItem.m_ItemCount -= Filling;
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
	m_ParentWindow.DistributeStack(Slot, a_SlotNum, a_Player, this, true);
	if (Slot.IsEmpty())
	{
		// Empty the slot completely, the client doesn't like left-over ItemType with zero count
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





void cSlotArea::MiddleClicked(cPlayer & a_Player, int a_SlotNum)
{
	cItem Slot(*GetSlot(a_SlotNum, a_Player));
	cItem & DraggingItem = a_Player.GetDraggingItem();

	if (!a_Player.IsGameModeCreative() || Slot.IsEmpty() || !DraggingItem.IsEmpty())
	{
		return;
	}

	DraggingItem = Slot;
	DraggingItem.m_ItemCount = DraggingItem.GetMaxStackSize();
}





void cSlotArea::DropClicked(cPlayer & a_Player, int a_SlotNum, bool a_DropStack)
{
	cItem Slot(*GetSlot(a_SlotNum, a_Player));
	if (Slot.IsEmpty())
	{
		return;
	}

	cItem ItemToDrop = Slot.CopyOne();
	if (a_DropStack)
	{
		ItemToDrop.m_ItemCount = Slot.m_ItemCount;
	}

	Slot.m_ItemCount -= ItemToDrop.m_ItemCount;
	if (Slot.m_ItemCount <= 0)
	{
		Slot.Empty();
	}
	SetSlot(a_SlotNum, a_Player, Slot);

	a_Player.TossPickup(ItemToDrop);
}





void cSlotArea::NumberClicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction)
{
	if ((a_ClickAction < caNumber1) || (a_ClickAction > caNumber9))
	{
		return;
	}

	int HotbarSlot = static_cast<int>(a_ClickAction - caNumber1);
	cItem ItemInHotbar(a_Player.GetInventory().GetHotbarSlot(HotbarSlot));
	cItem ItemInSlot(*GetSlot(a_SlotNum, a_Player));

	// The items are equal. Do nothing.
	if (ItemInHotbar.IsEqual(ItemInSlot))
	{
		return;
	}

	a_Player.GetInventory().SetHotbarSlot(HotbarSlot, ItemInSlot);
	SetSlot(a_SlotNum, a_Player, ItemInHotbar);
}





void cSlotArea::OnPlayerAdded(cPlayer & a_Player)
{
	UNUSED(a_Player);
}





void cSlotArea::OnPlayerRemoved(cPlayer & a_Player)
{
	UNUSED(a_Player);
}





void cSlotArea::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill)
{
	for (int i = 0; i < m_NumSlots; i++)
	{
		int SlotNum = (a_BackFill) ? (m_NumSlots - 1 - i) : i;

		const cItem * Slot = GetSlot(SlotNum, a_Player);
		if (!Slot->IsEqual(a_ItemStack) && (!Slot->IsEmpty() || a_KeepEmptySlots))
		{
			// Different items
			continue;
		}
		char NumFit = ItemHandler(Slot->m_ItemType)->GetMaxStackSize() - Slot->m_ItemCount;
		if (NumFit <= 0)
		{
			// Full stack already
			continue;
		}
		NumFit = std::min(NumFit, a_ItemStack.m_ItemCount);

		if (a_ShouldApply)
		{
			cItem NewSlot(a_ItemStack);
			NewSlot.m_ItemCount = Slot->m_ItemCount + NumFit;
			SetSlot(SlotNum, a_Player, NewSlot);
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
		if (!SlotItem.IsEqual(a_Dragging))
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





////////////////////////////////////////////////////////////////////////////////
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





////////////////////////////////////////////////////////////////////////////////
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





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaCrafting:

cSlotAreaCrafting::cSlotAreaCrafting(int a_GridSize, cWindow & a_ParentWindow) :
	cSlotAreaTemporary(1 + a_GridSize * a_GridSize, a_ParentWindow),
	m_GridSize(a_GridSize)
{
	ASSERT((a_GridSize == 2) || (a_GridSize == 3));
}





void cSlotAreaCrafting::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	if (a_ClickAction == caMiddleClick)
	{
		MiddleClicked(a_Player, a_SlotNum);
		return;
	}

	// Override for craft result slot
	if (a_SlotNum == 0)
	{
		if ((a_ClickAction == caShiftLeftClick) || (a_ClickAction == caShiftRightClick))
		{
			ShiftClickedResult(a_Player);
		}
		else if ((a_ClickAction == caDropKey) || (a_ClickAction == caCtrlDropKey))
		{
			DropClickedResult(a_Player);
		}
		else
		{
			ClickedResult(a_Player);
		}
		return;
	}

	Super::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
	UpdateRecipe(a_Player);
}





void cSlotAreaCrafting::DblClicked(cPlayer & a_Player, int a_SlotNum)
{
	if (a_SlotNum == 0)
	{
		// Dbl-clicking the crafting result slot shouldn't collect items to hand
		return;
	}
	Super::DblClicked(a_Player, a_SlotNum);
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





void cSlotAreaCrafting::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	// Update the recipe after setting the slot, if the slot is not the result slot:
	Super::SetSlot(a_SlotNum, a_Player, a_Item);
	if (a_SlotNum != 0)
	{
		UpdateRecipe(a_Player);
	}
}





void cSlotAreaCrafting::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill)
{
	UNUSED(a_ItemStack);
	UNUSED(a_Player);
	UNUSED(a_ShouldApply);
	UNUSED(a_KeepEmptySlots);
	UNUSED(a_BackFill);
}





void cSlotAreaCrafting::ClickedResult(cPlayer & a_Player)
{
	cItem & DraggingItem = a_Player.GetDraggingItem();

	// Get the current recipe:
	cCraftingRecipe & Recipe = GetRecipeForPlayer(a_Player);
	const cItem & Result = Recipe.GetResult();

	cItem * PlayerSlots = GetPlayerSlots(a_Player) + 1;
	cCraftingGrid Grid(PlayerSlots, m_GridSize, m_GridSize);

	// If possible, craft:
	if (DraggingItem.IsEmpty())
	{
		DraggingItem = Result;
		Recipe.ConsumeIngredients(Grid);
		Grid.CopyToItems(PlayerSlots);

		HandleCraftItem(Result, a_Player);
	}
	else if (DraggingItem.IsEqual(Result))
	{
		cItemHandler * Handler = ItemHandler(Result.m_ItemType);
		if (DraggingItem.m_ItemCount + Result.m_ItemCount <= Handler->GetMaxStackSize())
		{
			DraggingItem.m_ItemCount += Result.m_ItemCount;
			Recipe.ConsumeIngredients(Grid);
			Grid.CopyToItems(PlayerSlots);

			HandleCraftItem(Result, a_Player);
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
	a_Player.AddKnownItem(Result);
	cItem * PlayerSlots = GetPlayerSlots(a_Player) + 1;
	for (;;)
	{
		// Try distributing the result. If it fails, bail out:
		cItem ResultCopy(Result);
		m_ParentWindow.DistributeStack(ResultCopy, 0, a_Player, this, false);
		if (!ResultCopy.IsEmpty())
		{
			// Couldn't distribute all of it. Bail out
			return;
		}

		// Distribute the result, this time for real:
		ResultCopy = Result;
		m_ParentWindow.DistributeStack(ResultCopy, 0, a_Player, this, true);

		// Remove the ingredients from the crafting grid and update the recipe:
		cCraftingRecipe & Recipe = GetRecipeForPlayer(a_Player);
		cCraftingGrid Grid(PlayerSlots, m_GridSize, m_GridSize);
		Recipe.ConsumeIngredients(Grid);
		Grid.CopyToItems(PlayerSlots);
		UpdateRecipe(a_Player);

		// Broadcast the window, we sometimes move items to different locations than Vanilla, causing needless desyncs:
		m_ParentWindow.BroadcastWholeWindow();

		// Added achievements processing
		HandleCraftItem(Result, a_Player);

		// If the recipe has changed, bail out:
		if (!Recipe.GetResult().IsEqual(Result))
		{
			return;
		}
	}
}





void cSlotAreaCrafting::DropClickedResult(cPlayer & a_Player)
{
	// Get the current recipe:
	cCraftingRecipe & Recipe = GetRecipeForPlayer(a_Player);
	const cItem & Result = Recipe.GetResult();

	cItem * PlayerSlots = GetPlayerSlots(a_Player) + 1;
	cCraftingGrid Grid(PlayerSlots, m_GridSize, m_GridSize);

	a_Player.TossPickup(Result);
	Recipe.ConsumeIngredients(Grid);
	Grid.CopyToItems(PlayerSlots);

	HandleCraftItem(Result, a_Player);
	UpdateRecipe(a_Player);
}





void cSlotAreaCrafting::UpdateRecipe(cPlayer & a_Player)
{
	cCraftingGrid   Grid(GetPlayerSlots(a_Player) + 1, m_GridSize, m_GridSize);
	cCraftingRecipe & Recipe = GetRecipeForPlayer(a_Player);
	cRoot::Get()->GetCraftingRecipes()->GetRecipe(a_Player, Grid, Recipe);
	SetSlot(0, a_Player, Recipe.GetResult());
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
	cRoot::Get()->GetCraftingRecipes()->GetRecipe(a_Player, Grid, Recipe);
	m_Recipes.push_back(std::make_pair(a_Player.GetUniqueID(), Recipe));
	return m_Recipes.back().second;
}





void cSlotAreaCrafting::HandleCraftItem(const cItem & a_Result, cPlayer & a_Player)
{
	switch (a_Result.m_ItemType)
	{
		case E_BLOCK_WORKBENCH:         a_Player.AwardAchievement(Statistic::AchBuildWorkBench);      break;
		case E_BLOCK_FURNACE:           a_Player.AwardAchievement(Statistic::AchBuildFurnace);        break;
		case E_BLOCK_CAKE:              a_Player.AwardAchievement(Statistic::AchBakeCake);            break;
		case E_BLOCK_ENCHANTMENT_TABLE: a_Player.AwardAchievement(Statistic::AchEnchantments);        break;
		case E_BLOCK_BOOKCASE:          a_Player.AwardAchievement(Statistic::AchBookcase);            break;
		case E_ITEM_WOODEN_PICKAXE:     a_Player.AwardAchievement(Statistic::AchBuildPickaxe);        break;
		case E_ITEM_WOODEN_SWORD:       a_Player.AwardAchievement(Statistic::AchBuildSword);          break;
		case E_ITEM_STONE_PICKAXE:      a_Player.AwardAchievement(Statistic::AchBuildBetterPickaxe);  break;
		case E_ITEM_WOODEN_HOE:         a_Player.AwardAchievement(Statistic::AchBuildHoe);            break;
		case E_ITEM_BREAD:              a_Player.AwardAchievement(Statistic::AchMakeBread);           break;
		default: break;
	}
}





void cSlotAreaCrafting::LoadRecipe(cPlayer & a_Player, UInt32 a_RecipeId)
{
	if (a_RecipeId == 0)
	{
		return;
	}
	auto Recipe = cRoot::Get()->GetCraftingRecipes()->GetRecipeById(a_RecipeId);

	int NumItems = 0;
	ClearCraftingGrid(a_Player);

	for (auto itrS = Recipe->m_Ingredients.begin(); itrS != Recipe->m_Ingredients.end(); ++itrS)
	{
		cItem * FoundItem = a_Player.GetInventory().FindItem(itrS->m_Item);
		if (FoundItem == nullptr)
		{
			ClearCraftingGrid(a_Player);
			break;
		}
		cItem Item = FoundItem->CopyOne();
		++NumItems;
		int pos = 1 + itrS->x + m_GridSize * itrS->y;
		// Assuming there are ether shaped or unshaped recipes, no mixed ones
		if ((itrS->x == -1) && (itrS->y == -1))
		{
			pos = NumItems;
		}
		// Handle x wildcard
		else if (itrS->x == -1)
		{
			for (int i = 0; i < m_GridSize; i++)
			{
				pos = 1 + i + m_GridSize * itrS->y;
				auto itemCheck = GetSlot(pos, a_Player);
				if (itemCheck->IsEmpty())
				{
					break;
				}
			}
		}
		SetSlot(pos, a_Player, Item);
		a_Player.GetInventory().RemoveItem(Item);
	}
}





void cSlotAreaCrafting::ClearCraftingGrid(cPlayer & a_Player)
{
	for (int pos = 1; pos <= m_GridSize * m_GridSize; pos++)
	{
		auto Item = GetSlot(pos, a_Player);
		if (Item->m_ItemCount > 0)
		{
			a_Player.GetInventory().AddItem(*Item);
			SetSlot(pos, a_Player, cItem());
		}
	}
}



////////////////////////////////////////////////////////////////////////////////
// cSlotAreaAnvil:

cSlotAreaAnvil::cSlotAreaAnvil(cWindow & a_ParentWindow) :
	cSlotAreaTemporary(3, a_ParentWindow),
	m_MaximumCost(0),
	m_StackSizeToBeUsedInRepair(0)
{
}





void cSlotAreaAnvil::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	ASSERT((a_SlotNum >= 0) && (a_SlotNum < GetNumSlots()));
	if (a_SlotNum != 2)
	{
		Super::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
		UpdateResult(a_Player);
		return;
	}

	bool bAsync = false;
	if (GetSlot(a_SlotNum, a_Player) == nullptr)
	{
		LOGWARNING("GetSlot(%d) returned nullptr! Ignoring click", a_SlotNum);
		return;
	}

	switch (a_ClickAction)
	{
		case caDblClick:
		{
			return;
		}
		case caShiftLeftClick:
		case caShiftRightClick:
		{
			ShiftClicked(a_Player, a_SlotNum, a_ClickedItem);
			return;
		}
		case caMiddleClick:
		{
			MiddleClicked(a_Player, a_SlotNum);
			return;
		}
		case caDropKey:
		case caCtrlDropKey:
		{
			if (CanTakeResultItem(a_Player))
			{
				DropClicked(a_Player, a_SlotNum, true);
				OnTakeResult(a_Player);
			}
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

	if (Slot.IsEmpty())
	{
		return;
	}
	if (!DraggingItem.IsEmpty())
	{
		if (!(DraggingItem.IsEqual(Slot) && ((DraggingItem.m_ItemCount + Slot.m_ItemCount) <= cItemHandler::GetItemHandler(Slot)->GetMaxStackSize())))
		{
			return;
		}
	}

	if (!CanTakeResultItem(a_Player))
	{
		return;
	}

	cItem NewItem = cItem(Slot);
	NewItem.m_ItemCount += DraggingItem.m_ItemCount;

	Slot.Empty();
	DraggingItem.Empty();
	SetSlot(a_SlotNum, a_Player, Slot);

	DraggingItem = NewItem;
	OnTakeResult(a_Player);

	if (bAsync)
	{
		m_ParentWindow.BroadcastWholeWindow();
	}
}





void cSlotAreaAnvil::ShiftClicked(cPlayer & a_Player, int a_SlotNum, const cItem & a_ClickedItem)
{
	if (a_SlotNum != 2)
	{
		Super::ShiftClicked(a_Player, a_SlotNum, a_ClickedItem);
		UpdateResult(a_Player);
		return;
	}

	// Make a copy of the slot, distribute it among the other areas, then update the slot to contain the leftover:
	cItem Slot(*GetSlot(a_SlotNum, a_Player));

	if (Slot.IsEmpty() || !CanTakeResultItem(a_Player))
	{
		return;
	}

	m_ParentWindow.DistributeStack(Slot, a_SlotNum, a_Player, this, true);
	if (Slot.IsEmpty())
	{
		Slot.Empty();
		OnTakeResult(a_Player);
	}
	SetSlot(a_SlotNum, a_Player, Slot);

	// Some clients try to guess our actions and not always right (armor slots in 1.2.5), so we fix them:
	m_ParentWindow.BroadcastWholeWindow();
}





void cSlotAreaAnvil::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill)
{
	for (int i = 0; i < 2; i++)
	{
		int SlotNum = (a_BackFill) ? (2 - 1 - i) : i;

		const cItem * Slot = GetSlot(SlotNum, a_Player);
		if (!Slot->IsEqual(a_ItemStack) && (!Slot->IsEmpty() || a_KeepEmptySlots))
		{
			// Different items
			continue;
		}
		char NumFit = ItemHandler(Slot->m_ItemType)->GetMaxStackSize() - Slot->m_ItemCount;
		if (NumFit <= 0)
		{
			// Full stack already
			continue;
		}
		NumFit = std::min(NumFit, a_ItemStack.m_ItemCount);

		if (a_ShouldApply)
		{
			cItem NewSlot(a_ItemStack);
			NewSlot.m_ItemCount = Slot->m_ItemCount + NumFit;
			SetSlot(SlotNum, a_Player, NewSlot);
		}
		a_ItemStack.m_ItemCount -= NumFit;
		if (a_ItemStack.IsEmpty())
		{
			UpdateResult(a_Player);
			return;
		}
	}  // for i - Slots
	UpdateResult(a_Player);
}





void cSlotAreaAnvil::OnTakeResult(cPlayer & a_Player)
{
	if (!a_Player.IsGameModeCreative())
	{
		a_Player.DeltaExperience(-cPlayer::XpForLevel(m_MaximumCost));
	}
	SetSlot(0, a_Player, cItem());

	if (m_StackSizeToBeUsedInRepair > 0)
	{
		const cItem * Item = GetSlot(1, a_Player);
		if (!Item->IsEmpty() && (Item->m_ItemCount > m_StackSizeToBeUsedInRepair))
		{
			cItem NewSecondItem(*Item);
			NewSecondItem.m_ItemCount -= m_StackSizeToBeUsedInRepair;
			m_StackSizeToBeUsedInRepair = 0;
			SetSlot(1, a_Player, NewSecondItem);
		}
		else
		{
			SetSlot(1, a_Player, cItem());
		}
	}
	else
	{
		SetSlot(1, a_Player, cItem());
	}
	m_ParentWindow.SetProperty(0, static_cast<short>(m_MaximumCost));

	m_MaximumCost = 0;
	static_cast<cAnvilWindow &>(m_ParentWindow).SetRepairedItemName("", nullptr);

	const Vector3i BlockPos = static_cast<cAnvilWindow &>(m_ParentWindow).GetBlockPos();

	BLOCKTYPE Block;
	NIBBLETYPE BlockMeta;
	a_Player.GetWorld()->GetBlockTypeMeta(BlockPos, Block, BlockMeta);

	if (!a_Player.IsGameModeCreative() && (Block == E_BLOCK_ANVIL) && GetRandomProvider().RandBool(0.12))
	{
		NIBBLETYPE Orientation = BlockMeta & 0x3;
		NIBBLETYPE AnvilDamage = BlockMeta >> 2;
		++AnvilDamage;

		if (AnvilDamage > 2)
		{
			// Anvil will break
			a_Player.GetWorld()->SetBlock(BlockPos, E_BLOCK_AIR, 0);
			a_Player.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_ANVIL_BREAK, BlockPos, 0);
			a_Player.CloseWindow(false);
		}
		else
		{
			a_Player.GetWorld()->SetBlockMeta(BlockPos, static_cast<NIBBLETYPE>(Orientation | (AnvilDamage << 2)));
			a_Player.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_ANVIL_USE, BlockPos, 0);
		}
	}
	else
	{
		a_Player.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_ANVIL_USE, BlockPos, 0);
	}
}





bool cSlotAreaAnvil::CanTakeResultItem(cPlayer & a_Player)
{
	return (
			(
				a_Player.IsGameModeCreative() ||              // Is the player in gamemode?
				(a_Player.GetXpLevel() >= m_MaximumCost)      // or the player have enough exp?
			) &&
			(!GetSlot(2, a_Player)->IsEmpty()) &&             // Is a item in the result slot?
			(m_MaximumCost > 0)                               // When no maximum cost is set, the item isn't set from the UpdateResult() method and can't be a valid enchanting result.
	);
}





void cSlotAreaAnvil::OnPlayerRemoved(cPlayer & a_Player)
{
	TossItems(a_Player, 0, 2);
	Super::OnPlayerRemoved(a_Player);
}





void cSlotAreaAnvil::UpdateResult(cPlayer & a_Player)
{
	const cItem Target(*GetSlot(0, a_Player));
	const cItem Sacrifice(*GetSlot(1, a_Player));

	// Output initialised as copy of target
	cItem Output(Target);

	if (Target.IsEmpty())
	{
		Output.Empty();
		SetSlot(2, a_Player, Output);
		m_ParentWindow.SetProperty(0, 0);
		m_MaximumCost = 0;
		return;
	}

	m_MaximumCost = 0;
	m_StackSizeToBeUsedInRepair = 0;
	int RepairCost = Target.m_RepairCost;
	int NeedExp = 0;
	if (!Sacrifice.IsEmpty())
	{
		bool IsEnchantBook = (Sacrifice.m_ItemType == E_ITEM_ENCHANTED_BOOK);

		RepairCost += Sacrifice.m_RepairCost;
		// Can we repair with sacrifce material?
		if (Target.IsDamageable() && cItemHandler::GetItemHandler(Target)->CanRepairWithRawMaterial(Sacrifice.m_ItemType))
		{
			// Tool and armor repair with special item (iron / gold / diamond / ...)
			int DamageDiff = std::min(static_cast<int>(Target.m_ItemDamage), static_cast<int>(Target.GetMaxDamage()) / 4);
			if (DamageDiff <= 0)
			{
				// No enchantment
				Output.Empty();
				SetSlot(2, a_Player, Output);
				m_ParentWindow.SetProperty(0, 0);
				m_MaximumCost = 0;
				return;
			}

			int NumItemsConsumed = 0;
			// Repair until out of materials, or fully repaired
			while ((DamageDiff > 0) && (NumItemsConsumed < Sacrifice.m_ItemCount))
			{
				Output.m_ItemDamage -= DamageDiff;
				NeedExp += std::max(1, DamageDiff / 100) + static_cast<int>(Target.m_Enchantments.Count());
				DamageDiff = std::min(static_cast<int>(Output.m_ItemDamage), static_cast<int>(Target.GetMaxDamage()) / 4);

				++NumItemsConsumed;
			}
			m_StackSizeToBeUsedInRepair = static_cast<char>(NumItemsConsumed);
		}
		else  // Combining tools / armour
		{
			// No result if we can't combine the items
			if (!IsEnchantBook && (!Target.IsSameType(Sacrifice) || !Target.IsDamageable()))
			{
				// No enchantment
				Output.Empty();
				SetSlot(2, a_Player, Output);
				m_ParentWindow.SetProperty(0, 0);
				m_MaximumCost = 0;
				return;
			}

			// Can we repair with sacrifice tool / armour?
			if (Target.IsDamageable() && !IsEnchantBook && (Target.m_ItemDamage!=0))
			{
				// Durability = MaxDamage - m_ItemDamage = how far from broken
				const short TargetDurability = Target.GetMaxDamage() - Target.m_ItemDamage;
				const short SacrificeDurability = Sacrifice.GetMaxDamage() - Sacrifice.m_ItemDamage;
				// How much durability to repair by:
				const short RepairDurability = SacrificeDurability + Target.GetMaxDamage() * 12 / 100;

				// Don't give item a negative damage:
				short NewItemDamage = std::max<short>(Target.GetMaxDamage() - (TargetDurability + RepairDurability), 0);

				if (NewItemDamage < Target.m_ItemDamage)
				{
					Output.m_ItemDamage = NewItemDamage;
					NeedExp += std::max(1, RepairDurability / 100);
				}
			}

			// Add the enchantments from the sacrifice to the target
			int EnchantmentCost = Output.AddEnchantmentsFromItem(Sacrifice);
			NeedExp += EnchantmentCost;
		}
	}

	int NameChangeExp = 0;
	const AString & RepairedItemName = static_cast<cAnvilWindow*>(&m_ParentWindow)->GetRepairedItemName();
	if (RepairedItemName.empty())
	{
		// Remove custom name
		if (!Target.m_CustomName.empty())
		{
			NameChangeExp = (Target.IsDamageable()) ? 7 : (Target.m_ItemCount * 5);
			NeedExp += NameChangeExp;
			Output.m_CustomName = "";
		}
	}
	else if (RepairedItemName != Target.m_CustomName)
	{
		// Change custom name
		NameChangeExp = (Target.IsDamageable()) ? 7 : (Target.m_ItemCount * 5);
		NeedExp += NameChangeExp;

		if (!Target.m_CustomName.empty())
		{
			RepairCost += NameChangeExp / 2;
		}

		Output.m_CustomName = RepairedItemName;
	}

	m_MaximumCost = RepairCost + NeedExp;

	if (NeedExp < 0)
	{
		Output.Empty();
	}

	if ((NameChangeExp == NeedExp) && (NameChangeExp > 0) && (m_MaximumCost >= 40))
	{
		m_MaximumCost = 39;
	}
	if ((m_MaximumCost >= 40) && !a_Player.IsGameModeCreative())
	{
		Output.Empty();
	}

	if (!Output.IsEmpty())
	{
		RepairCost = std::max(Target.m_RepairCost, Sacrifice.m_RepairCost);
		if (!Output.m_CustomName.empty())
		{
			RepairCost -= 9;
		}
		RepairCost = std::max(RepairCost, 0);
		RepairCost += 2;
		Output.m_RepairCost = RepairCost;
	}

	// If after everything, output will be the same then no point enchanting
	if (Target.IsEqual(Output))
	{
		Output.Empty();
		m_MaximumCost = 0;
	}

	SetSlot(2, a_Player, Output);
	m_ParentWindow.SetProperty(0, static_cast<Int16>(m_MaximumCost));
}





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaBeacon:

cSlotAreaBeacon::cSlotAreaBeacon(cBeaconEntity * a_Beacon, cWindow & a_ParentWindow) :
	cSlotArea(1, a_ParentWindow),
	m_Beacon(a_Beacon)
{
	m_Beacon->GetContents().AddListener(*this);
}





cSlotAreaBeacon::~cSlotAreaBeacon()
{
	m_Beacon->GetContents().RemoveListener(*this);
}





bool cSlotAreaBeacon::IsPlaceableItem(short a_ItemType)
{
	switch (a_ItemType)
	{
		case E_ITEM_EMERALD:
		case E_ITEM_DIAMOND:
		case E_ITEM_GOLD:
		case E_ITEM_IRON:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





void cSlotAreaBeacon::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	ASSERT((a_SlotNum >= 0) && (a_SlotNum < GetNumSlots()));

	bool bAsync = false;
	if (GetSlot(a_SlotNum, a_Player) == nullptr)
	{
		LOGWARNING("GetSlot(%d) returned nullptr! Ignoring click", a_SlotNum);
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
		case caMiddleClick:
		{
			MiddleClicked(a_Player, a_SlotNum);
			return;
		}
		case caDropKey:
		case caCtrlDropKey:
		{
			DropClicked(a_Player, a_SlotNum, false);
			return;
		}
		case caNumber1:
		case caNumber2:
		case caNumber3:
		case caNumber4:
		case caNumber5:
		case caNumber6:
		case caNumber7:
		case caNumber8:
		case caNumber9:
		{
			NumberClicked(a_Player, a_SlotNum, a_ClickAction);
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

	if (DraggingItem.IsEmpty())
	{
		DraggingItem = Slot;
		Slot.Empty();
	}
	else if (Slot.IsEmpty())
	{
		if (!IsPlaceableItem(DraggingItem.m_ItemType))
		{
			return;
		}

		Slot = DraggingItem.CopyOne();
		DraggingItem.m_ItemCount -= 1;
		if (DraggingItem.m_ItemCount <= 0)
		{
			DraggingItem.Empty();
		}
	}
	else if (DraggingItem.m_ItemCount == 1)
	{
		if (!IsPlaceableItem(DraggingItem.m_ItemCount))
		{
			return;
		}

		// Switch contents
		cItem tmp(DraggingItem);
		DraggingItem = Slot;
		Slot = tmp;
	}

	SetSlot(a_SlotNum, a_Player, Slot);
	if (bAsync)
	{
		m_ParentWindow.BroadcastWholeWindow();
	}
}





void cSlotAreaBeacon::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill)
{
	const cItem * Slot = GetSlot(0, a_Player);
	if (!Slot->IsEmpty() || !IsPlaceableItem(a_ItemStack.m_ItemType) || (a_ItemStack.m_ItemCount != 1))
	{
		return;
	}

	if (a_ShouldApply)
	{
		SetSlot(0, a_Player, a_ItemStack.CopyOne());
	}
	a_ItemStack.Empty();
}





const cItem * cSlotAreaBeacon::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	UNUSED(a_Player);
	return &(m_Beacon->GetSlot(a_SlotNum));
}





void cSlotAreaBeacon::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	UNUSED(a_Player);
	m_Beacon->SetSlot(a_SlotNum, a_Item);
}





void cSlotAreaBeacon::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	UNUSED(a_SlotNum);
	// Something has changed in the window, broadcast the entire window to all clients
	ASSERT(a_ItemGrid == &(m_Beacon->GetContents()));

	m_ParentWindow.BroadcastWholeWindow();
}





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaEnchanting:

cSlotAreaEnchanting::cSlotAreaEnchanting(cWindow & a_ParentWindow, Vector3i a_BlockPos) :
	cSlotAreaTemporary(2, a_ParentWindow),
	m_BlockPos(a_BlockPos)
{
}





void cSlotAreaEnchanting::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	ASSERT((a_SlotNum >= 0) && (a_SlotNum < GetNumSlots()));

	bool bAsync = false;
	if (GetSlot(a_SlotNum, a_Player) == nullptr)
	{
		LOGWARNING("GetSlot(%d) returned nullptr! Ignoring click", a_SlotNum);
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
		case caMiddleClick:
		{
			MiddleClicked(a_Player, a_SlotNum);
			return;
		}
		case caDropKey:
		case caCtrlDropKey:
		{
			DropClicked(a_Player, a_SlotNum, false);
			return;
		}
		case caNumber1:
		case caNumber2:
		case caNumber3:
		case caNumber4:
		case caNumber5:
		case caNumber6:
		case caNumber7:
		case caNumber8:
		case caNumber9:
		{
			NumberClicked(a_Player, a_SlotNum, a_ClickAction);
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
	if (a_SlotNum == 1)
	{
		// Lapis slot can have a full stack handle it normally, also check for empty hand
		if ((DraggingItem.IsEmpty()) || ((DraggingItem.m_ItemType == E_ITEM_DYE) && (DraggingItem.m_ItemDamage == E_META_DYE_BLUE)))
		{
			return cSlotArea::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
		}

		if (bAsync)
		{
			m_ParentWindow.BroadcastWholeWindow();
		}
		return;
	}
	// Slot 0 is where the item to enhance goes.
	if (DraggingItem.IsEmpty())
	{
		// DraggingItem is empty -> Switch draggingitem and slot
		if (!Slot.IsEmpty())
		{
			std::swap(DraggingItem, Slot);
		}
	}
	else if (Slot.IsEmpty())
	{
		// DraggingItem isn't empty and slot is empty -> Set one dragging item in the slot
		Slot = DraggingItem.CopyOne();
		DraggingItem.m_ItemCount -= 1;

		if (DraggingItem.m_ItemCount <= 0)
		{
			DraggingItem.Empty();
		}
	}
	else if ((DraggingItem.m_ItemCount == 1) && !DraggingItem.IsEqual(Slot))
	{
		// DraggingItem and slot aren't empty -> Switch items
		std::swap(DraggingItem, Slot);
	}

	SetSlot(a_SlotNum, a_Player, Slot);
	if (bAsync)
	{
		m_ParentWindow.BroadcastWholeWindow();
	}
}





void cSlotAreaEnchanting::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_Apply, bool a_KeepEmptySlots, bool a_BackFill)
{
	if ((a_ItemStack.m_ItemType == E_ITEM_DYE) && (a_ItemStack.m_ItemDamage == E_META_DYE_BLUE))
	{
		// It's lapis, put it in the lapis spot.
		const cItem * Slot = GetSlot(1, a_Player);
		char NumFit = ItemHandler(Slot->m_ItemType)->GetMaxStackSize() - Slot->m_ItemCount;
		if (NumFit <= 0)
		{
			// Full stack already
			return;
		}
		NumFit = std::min(NumFit, a_ItemStack.m_ItemCount);

		if (a_Apply)
		{
			cItem NewSlot(a_ItemStack);
			NewSlot.m_ItemCount = Slot->m_ItemCount + NumFit;
			SetSlot(1, a_Player, NewSlot);
		}
		a_ItemStack.m_ItemCount -= NumFit;
		// Return so we don't put overflow into the enchantment slot
		return;
	}
	const cItem * Slot = GetSlot(0, a_Player);
	if (!Slot->IsEmpty())
	{
		return;
	}

	if (a_Apply)
	{
		SetSlot(0, a_Player, a_ItemStack.CopyOne());
	}
	a_ItemStack.m_ItemCount -= 1;
	if (a_ItemStack.m_ItemCount <= 0)
	{
		a_ItemStack.Empty();
	}
}





void cSlotAreaEnchanting::OnPlayerAdded(cPlayer & a_Player)
{
	Super::OnPlayerAdded(a_Player);
	UpdateResult(a_Player);
}





void cSlotAreaEnchanting::OnPlayerRemoved(cPlayer & a_Player)
{
	// Toss the item in the enchanting slot, as well as lapis
	TossItems(a_Player, 0, m_NumSlots);

	Super::OnPlayerRemoved(a_Player);
}





void cSlotAreaEnchanting::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	Super::SetSlot(a_SlotNum, a_Player, a_Item);
	UpdateResult(a_Player);
}





void cSlotAreaEnchanting::UpdateResult(cPlayer & a_Player)
{
	cItem Item = *GetSlot(0, a_Player);

	if (!cItem::IsEnchantable(Item.m_ItemType) || !Item.m_Enchantments.IsEmpty())
	{
		return;
	}

	// Pseudocode found at: https://minecraft.gamepedia.com/Enchanting_mechanics
	const auto Bookshelves = std::min(static_cast<int>(GetBookshelvesCount(*a_Player.GetWorld())), 15);

	// A PRNG initialised using the player's enchantment seed.
	auto Random = a_Player.GetEnchantmentRandomProvider();

	// Calculate the levels for the offered enchantment options:
	const auto Base = (Random.RandInt(1, 8) + (Bookshelves / 2) + Random.RandInt(0, Bookshelves));
	const std::array<short, 3> OptionLevels
	{
		static_cast<short>(std::max(Base / 3, 1)),
		static_cast<short>((Base * 2) / 3 + 1),
		static_cast<short>(std::max(Base, Bookshelves * 2))
	};

	// Properties set according to: https://wiki.vg/Protocol#Window_Property
	// Fake a "seed" for the client to draw Standard Galactic Alphabet glyphs:
	m_ParentWindow.SetProperty(3, Random.RandInt<short>());

	// Calculate an enchanting possibility for each option (top, middle and bottom) and send details to window:
	for (size_t i = 0; i != OptionLevels.size(); i++)
	{
		// A copy of the item.
		cItem EnchantedItem = Item.CopyOne();

		// Enchant based on the number of levels:
		EnchantedItem.EnchantByXPLevels(OptionLevels[i], Random);

		LOGD("Generated enchanted item %d with enchantments: %s", i, EnchantedItem.m_Enchantments.ToString());

		// Send the level requirement for the enchantment option:
		m_ParentWindow.SetProperty(i, OptionLevels[i]);

		// Get the first enchantment ID, which must exist:
		ASSERT(EnchantedItem.m_Enchantments.begin() != EnchantedItem.m_Enchantments.end());
		const auto EnchantmentID = static_cast<short>(EnchantedItem.m_Enchantments.begin()->first);

		// Send the enchantment ID of the first enchantment on our item:
		m_ParentWindow.SetProperty(4 + i, EnchantmentID);

		const auto EnchantmentLevel = static_cast<short>(EnchantedItem.m_Enchantments.GetLevel(EnchantmentID));
		ASSERT(EnchantmentLevel > 0);

		// Send the level for the first enchantment on our item:
		m_ParentWindow.SetProperty(7 + i, EnchantmentLevel);

		// Store the item we've enchanted as an option to be retrieved later:
		m_EnchantedItemOptions[i] = std::move(EnchantedItem);
	}
}





unsigned cSlotAreaEnchanting::GetBookshelvesCount(cWorld & a_World)
{
	cBlockArea Area;
	Area.Read(a_World, m_BlockPos - Vector3i(2, 0, 2), m_BlockPos + Vector3i(2, 1, 2));

	static const struct
	{
		int m_BookX, m_BookY, m_BookZ;  // Coords to check for bookcases
		int m_AirX, m_AirY, m_AirZ;  // Coords to check for air; if not air, the bookcase won't be counted
	} CheckCoords[] =
	{
		{ 0, 0, 0, 1, 0, 1 },  // Bookcase at {0, 0, 0}, air at {1, 0, 1}
		{ 0, 0, 1, 1, 0, 1 },  // Bookcase at {0, 0, 1}, air at {1, 0, 1}
		{ 0, 0, 2, 1, 0, 2 },  // Bookcase at {0, 0, 2}, air at {1, 0, 2}
		{ 0, 0, 3, 1, 0, 3 },  // Bookcase at {0, 0, 3}, air at {1, 0, 3}
		{ 0, 0, 4, 1, 0, 3 },  // Bookcase at {0, 0, 4}, air at {1, 0, 3}
		{ 1, 0, 4, 1, 0, 3 },  // Bookcase at {1, 0, 4}, air at {1, 0, 3}
		{ 2, 0, 4, 2, 0, 3 },  // Bookcase at {2, 0, 4}, air at {2, 0, 3}
		{ 3, 0, 4, 3, 0, 3 },  // Bookcase at {3, 0, 4}, air at {3, 0, 3}
		{ 4, 0, 4, 3, 0, 3 },  // Bookcase at {4, 0, 4}, air at {3, 0, 3}
		{ 4, 0, 3, 3, 0, 3 },  // Bookcase at {4, 0, 3}, air at {3, 0, 3}
		{ 4, 0, 2, 3, 0, 2 },  // Bookcase at {4, 0, 2}, air at {3, 0, 2}
		{ 4, 0, 1, 3, 0, 1 },  // Bookcase at {4, 0, 1}, air at {3, 0, 1}
		{ 4, 0, 0, 3, 0, 1 },  // Bookcase at {4, 0, 0}, air at {3, 0, 1}
		{ 3, 0, 0, 3, 0, 1 },  // Bookcase at {3, 0, 0}, air at {3, 0, 1}
		{ 2, 0, 0, 2, 0, 1 },  // Bookcase at {2, 0, 0}, air at {2, 0, 1}
		{ 1, 0, 0, 1, 0, 1 },  // Bookcase at {1, 0, 0}, air at {1, 0, 1}

		{ 0, 1, 0, 1, 1, 1 },  // Bookcase at {0, 1, 0}, air at {1, 1, 1}
		{ 0, 1, 1, 1, 1, 1 },  // Bookcase at {0, 1, 1}, air at {1, 1, 1}
		{ 0, 1, 2, 1, 1, 2 },  // Bookcase at {0, 1, 2}, air at {1, 1, 2}
		{ 0, 1, 3, 1, 1, 3 },  // Bookcase at {0, 1, 3}, air at {1, 1, 3}
		{ 0, 1, 4, 1, 1, 3 },  // Bookcase at {0, 1, 4}, air at {1, 1, 3}
		{ 1, 1, 4, 1, 1, 3 },  // Bookcase at {1, 1, 4}, air at {1, 1, 3}
		{ 2, 1, 4, 2, 1, 3 },  // Bookcase at {2, 1, 4}, air at {2, 1, 3}
		{ 3, 1, 4, 3, 1, 3 },  // Bookcase at {3, 1, 4}, air at {3, 1, 3}
		{ 4, 1, 4, 3, 1, 3 },  // Bookcase at {4, 1, 4}, air at {3, 1, 3}
		{ 4, 1, 3, 3, 1, 3 },  // Bookcase at {4, 1, 3}, air at {3, 1, 3}
		{ 4, 1, 2, 3, 1, 2 },  // Bookcase at {4, 1, 2}, air at {3, 1, 2}
		{ 4, 1, 1, 3, 1, 1 },  // Bookcase at {4, 1, 1}, air at {3, 1, 1}
		{ 4, 1, 0, 3, 1, 1 },  // Bookcase at {4, 1, 0}, air at {3, 1, 1}
		{ 3, 1, 0, 3, 1, 1 },  // Bookcase at {3, 1, 0}, air at {3, 1, 1}
		{ 2, 1, 0, 2, 1, 1 },  // Bookcase at {2, 1, 0}, air at {2, 1, 1}
		{ 1, 1, 0, 1, 1, 1 },  // Bookcase at {1, 1, 0}, air at {1, 1, 1}
	};

	unsigned Bookshelves = 0;

	for (size_t i = 0; i < ARRAYCOUNT(CheckCoords); i++)
	{
		if (
			(Area.GetRelBlockType(CheckCoords[i].m_AirX, CheckCoords[i].m_AirY, CheckCoords[i].m_AirZ) == E_BLOCK_AIR) &&  // There's air in the checkspot
			(Area.GetRelBlockType(CheckCoords[i].m_BookX, CheckCoords[i].m_BookY, CheckCoords[i].m_BookZ) == E_BLOCK_BOOKCASE)  // There's bookcase in the wanted place
		)
		{
			Bookshelves++;
		}
	}  // for i - CheckCoords

	return Bookshelves;
}





cItem cSlotAreaEnchanting::SelectEnchantedOption(size_t a_EnchantOption)
{
	ASSERT(a_EnchantOption < m_EnchantedItemOptions.size());
	return std::move(m_EnchantedItemOptions[a_EnchantOption]);
}





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaEnderChest:

cSlotAreaEnderChest::cSlotAreaEnderChest(cEnderChestEntity * a_EnderChest, cWindow & a_ParentWindow) :
	cSlotArea(27, a_ParentWindow),
	m_EnderChest(a_EnderChest)
{
}





const cItem * cSlotAreaEnderChest::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	return &(a_Player.GetEnderChestContents().GetSlot(a_SlotNum));
}





void cSlotAreaEnderChest::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	a_Player.GetEnderChestContents().SetSlot(a_SlotNum, a_Item);
}





////////////////////////////////////////////////////////////////////////////////
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
	if (m_Furnace == nullptr)
	{
		LOGERROR("cSlotAreaFurnace::Clicked(): m_Furnace == nullptr");
		ASSERT(!"cSlotAreaFurnace::Clicked(): m_Furnace == nullptr");
		return;
	}

	if (a_SlotNum == 2)
	{
		bool bAsync = false;
		if (GetSlot(a_SlotNum, a_Player) == nullptr)
		{
			LOGWARNING("GetSlot(%d) returned nullptr! Ignoring click", a_SlotNum);
			return;
		}

		cItem Slot(*GetSlot(a_SlotNum, a_Player));
		if (!Slot.IsSameType(a_ClickedItem))
		{
			LOGWARNING("*** Window lost sync at item %d in SlotArea with %d items ***", a_SlotNum, m_NumSlots);
			LOGWARNING("My item:    %s", ItemToFullString(Slot).c_str());
			LOGWARNING("Their item: %s", ItemToFullString(a_ClickedItem).c_str());
			bAsync = true;
		}

		switch (a_ClickAction)
		{
			case caShiftLeftClick:
			case caShiftRightClick:
			{
				HandleSmeltItem(Slot, a_Player);
				ShiftClicked(a_Player, a_SlotNum, Slot);
				return;
			}
			case caMiddleClick:
			{
				MiddleClicked(a_Player, a_SlotNum);
				return;
			}
			case caDropKey:
			case caCtrlDropKey:
			{
				DropClicked(a_Player, a_SlotNum, (a_SlotNum == caCtrlDropKey));
				Slot.m_ItemCount = Slot.m_ItemCount - GetSlot(a_SlotNum, a_Player)->m_ItemCount;
				HandleSmeltItem(Slot, a_Player);
				return;
			}
			default:
			{
				break;
			}
		}

		cItem & DraggingItem = a_Player.GetDraggingItem();
		if (!DraggingItem.IsEmpty())
		{
			if (a_ClickAction == caDblClick)
			{
				return;
			}
			if (!DraggingItem.IsEqual(Slot))
			{
				return;
			}
			if ((DraggingItem.m_ItemCount + Slot.m_ItemCount) > Slot.GetMaxStackSize())
			{
				return;
			}

			DraggingItem.m_ItemCount += Slot.m_ItemCount;
			HandleSmeltItem(Slot, a_Player);
			Slot.Empty();
		}
		else
		{
			switch (a_ClickAction)
			{
				case caDblClick:
				{
					DblClicked(a_Player, a_SlotNum);
					return;
				}
				case caLeftClick:
				{
					DraggingItem = Slot;
					HandleSmeltItem(Slot, a_Player);
					Slot.Empty();
					break;
				}
				case caRightClick:
				{
					DraggingItem = Slot.CopyOne();
					DraggingItem.m_ItemCount = static_cast<char>(static_cast<float>(Slot.m_ItemCount) / 2.f + 0.5f);
					Slot.m_ItemCount -= DraggingItem.m_ItemCount;

					if (Slot.m_ItemCount <= 0)
					{
						Slot.Empty();
					}
					HandleSmeltItem(DraggingItem, a_Player);
					break;
				}
				default:
				{
					ASSERT(!"Unhandled click type!");
				}
			}
		}

		SetSlot(a_SlotNum, a_Player, Slot);
		if (bAsync)
		{
			m_ParentWindow.BroadcastWholeWindow();
		}
		return;
	}

	Super::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
}





void cSlotAreaFurnace::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill)
{
	int SlotNum;
	cFurnaceRecipe * FurnaceRecipes = cRoot::Get()->GetFurnaceRecipe();

	if (FurnaceRecipes->GetRecipeFrom(a_ItemStack) != nullptr)
	{
		SlotNum = 0;
	}
	else if (FurnaceRecipes->IsFuel(a_ItemStack))
	{
		SlotNum = 1;
	}
	else
	{
		return;
	}

	const cItem * Slot = GetSlot(SlotNum, a_Player);
	if (!Slot->IsEqual(a_ItemStack) && (!Slot->IsEmpty() || a_KeepEmptySlots))
	{
		// Different items
		return;
	}

	char NumFit = ItemHandler(Slot->m_ItemType)->GetMaxStackSize() - Slot->m_ItemCount;
	if (NumFit <= 0)
	{
		// Full stack already
		return;
	}
	NumFit = std::min(NumFit, a_ItemStack.m_ItemCount);

	if (a_ShouldApply)
	{
		cItem NewSlot(a_ItemStack);
		NewSlot.m_ItemCount = Slot->m_ItemCount + NumFit;
		SetSlot(SlotNum, a_Player, NewSlot);
	}
	a_ItemStack.m_ItemCount -= NumFit;
	if (a_ItemStack.IsEmpty())
	{
		return;
	}
}





const cItem * cSlotAreaFurnace::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	UNUSED(a_Player);
	// a_SlotNum ranges from 0 to 2, query the items from the underlying furnace:
	return &(m_Furnace->GetSlot(a_SlotNum));
}





void cSlotAreaFurnace::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	m_Furnace->SetSlot(a_SlotNum, a_Item);
}





void cSlotAreaFurnace::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	UNUSED(a_SlotNum);
	// Something has changed in the window, broadcast the entire window to all clients
	ASSERT(a_ItemGrid == &(m_Furnace->GetContents()));

	m_ParentWindow.BroadcastWholeWindow();
}





void cSlotAreaFurnace::HandleSmeltItem(const cItem & a_Result, cPlayer & a_Player)
{
	int Reward = m_Furnace->GetAndResetReward();
	if (Reward > 0)
	{
		a_Player.GetWorld()->SpawnSplitExperienceOrbs(a_Player.GetPosX(), a_Player.GetPosY(), a_Player.GetPosZ(), Reward);
	}

	/** TODO 2014-05-12 xdot: Figure out when to call this method. */
	switch (a_Result.m_ItemType)
	{
		case E_ITEM_IRON:        a_Player.AwardAchievement(Statistic::AchAcquireIron); break;
		case E_ITEM_COOKED_FISH: a_Player.AwardAchievement(Statistic::AchCookFish);    break;
		default: break;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaBrewingstand:
cSlotAreaBrewingstand::cSlotAreaBrewingstand(cBrewingstandEntity * a_Brewingstand, cWindow & a_ParentWindow) :
	cSlotArea(5, a_ParentWindow),
	m_Brewingstand(a_Brewingstand)
{
	m_Brewingstand->GetContents().AddListener(*this);
}





cSlotAreaBrewingstand::~cSlotAreaBrewingstand()
{
	m_Brewingstand->GetContents().RemoveListener(*this);
}





void cSlotAreaBrewingstand::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	if (m_Brewingstand == nullptr)
	{
		LOGERROR("cSlotAreaBrewingstand::Clicked(): m_Brewingstand == nullptr");
		ASSERT(!"cSlotAreaBrewingstand::Clicked(): m_Brewingstand == nullptr");
		return;
	}

	if (GetSlot(a_SlotNum, a_Player) == nullptr)
	{
		LOGWARNING("GetSlot(%d) returned nullptr! Ignoring click", a_SlotNum);
		return;
	}

	cItem Slot(*GetSlot(a_SlotNum, a_Player));
	cItem & DraggingItem = a_Player.GetDraggingItem();
	cBrewingRecipes * BR = cRoot::Get()->GetBrewingRecipes();

	if ((a_SlotNum >= 0) && (a_SlotNum <= 2))
	{
		// Bottle slots
		switch (a_ClickAction)
		{
			case caLeftClick:
			case caRightClick:
			{
				if (BR->IsBottle(Slot))
				{
					HandleBrewedItem(a_Player, Slot);
				}
				if (!DraggingItem.IsEmpty() && !BR->IsBottle(DraggingItem))
				{
					// Deny placing a invalid item into the bottle slot
					return;
				}
				break;
			}
			case caShiftLeftClick:
			case caShiftRightClick:
			{
				if (BR->IsBottle(Slot))
				{
					HandleBrewedItem(a_Player, Slot);
				}
				Super::ShiftClicked(a_Player, a_SlotNum, Slot);
				break;
			}
			default:
			{
				if (!DraggingItem.IsEmpty() && !BR->IsBottle(DraggingItem))
				{
					// Deny placing a invalid item into the bottle slot
					return;
				}
				break;
			}
		}
	}

	if ((a_SlotNum == 3) && !DraggingItem.IsEmpty())
	{
		// Ingredient slot
		switch (a_ClickAction)
		{
			case caShiftLeftClick:
			case caShiftRightClick:
			{
				Super::ShiftClicked(a_Player, a_SlotNum, Slot);
				break;
			}
			default:
			{
				if (!BR->IsIngredient(DraggingItem))
				{
					// Deny placing a invalid item into the ingredient slot
					return;
				}
				break;
			}
		}
	}

	if ((a_SlotNum == 4) && !DraggingItem.IsEmpty())
	{
		// Fuel slot
		switch (a_ClickAction)
		{
			case caShiftLeftClick:
			case caShiftRightClick:
			{
				Super::ShiftClicked(a_Player, a_SlotNum, Slot);
				break;
			}
			default:
			{
				if (!BR->IsFuel(DraggingItem))
				{
					// Deny placing a invalid item into the fuel slot
					return;
				}
				break;
			}
		}
	}

	Super::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
}





void cSlotAreaBrewingstand::HandleBrewedItem(cPlayer & a_Player, const cItem & a_ClickedItem)
{
	// Award an achievement if the item is not a water bottle (is a real brewed potion)
	if (a_ClickedItem.m_ItemDamage > 0)
	{
		a_Player.AwardAchievement(Statistic::AchPotion);
	}
}





void cSlotAreaBrewingstand::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill)
{
	int SlotNum = -1;
	cBrewingRecipes * BR = cRoot::Get()->GetBrewingRecipes();
	if (BR->IsBottle(a_ItemStack))
	{
		for (int i = 0;i < 3;i++)
		{
			if (GetSlot(i, a_Player)->IsEmpty())
			{
				SlotNum = i;
				break;
			}
		}

		if (SlotNum == -1)
		{
			// All slots are full
			return;
		}
	}
	else if (BR->IsIngredient(a_ItemStack))
	{
		SlotNum = 3;
	}
	else
	{
		return;
	}

	const cItem * Slot = GetSlot(SlotNum, a_Player);
	if (!Slot->IsEqual(a_ItemStack) && (!Slot->IsEmpty() || a_KeepEmptySlots))
	{
		// Different items
		return;
	}

	char NumFit = ItemHandler(Slot->m_ItemType)->GetMaxStackSize() - Slot->m_ItemCount;
	if (NumFit <= 0)
	{
		// Full stack already
		return;
	}
	NumFit = std::min(NumFit, a_ItemStack.m_ItemCount);

	if (a_ShouldApply)
	{
		cItem NewSlot(a_ItemStack);
		NewSlot.m_ItemCount = Slot->m_ItemCount + NumFit;
		SetSlot(SlotNum, a_Player, NewSlot);
	}
	a_ItemStack.m_ItemCount -= NumFit;
	if (a_ItemStack.IsEmpty())
	{
		return;
	}
}





const cItem * cSlotAreaBrewingstand::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	UNUSED(a_Player);
	// a_SlotNum ranges from 0 to 3, query the items from the underlying brewing stand:
	return &(m_Brewingstand->GetSlot(a_SlotNum));
}





void cSlotAreaBrewingstand::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	UNUSED(a_Player);
	m_Brewingstand->SetSlot(a_SlotNum, a_Item);
}





void cSlotAreaBrewingstand::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	UNUSED(a_SlotNum);
	// Something has changed in the window, broadcast the entire window to all clients
	ASSERT(a_ItemGrid == &(m_Brewingstand->GetContents()));

	m_ParentWindow.BroadcastWholeWindow();
}





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaMinecartWithChest:

cSlotAreaMinecartWithChest::cSlotAreaMinecartWithChest(cMinecartWithChest * a_Chest, cWindow & a_ParentWindow) :
	cSlotArea(27, a_ParentWindow),
	m_Chest(a_Chest)
{
}





const cItem * cSlotAreaMinecartWithChest::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	// a_SlotNum ranges from 0 to 26, use that to index the minecart chest entity's inventory directly:
	UNUSED(a_Player);
	return &(m_Chest->GetSlot(a_SlotNum));
}





void cSlotAreaMinecartWithChest::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	UNUSED(a_Player);
	m_Chest->SetSlot(a_SlotNum, a_Item);
}





////////////////////////////////////////////////////////////////////////////////
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
		if ((a_ClickAction == caDropKey) || (a_ClickAction == caCtrlDropKey))
		{
			DropClicked(a_Player, a_SlotNum, (a_ClickAction == caCtrlDropKey));
			return;
		}

		// Creative inventory must treat a_ClickedItem as a DraggedItem instead, replacing the inventory slot with it
		SetSlot(a_SlotNum, a_Player, a_ClickedItem);
		return;
	}

	// Survival inventory and all other windows' inventory has the same handling as normal slot areas
	Super::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
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





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaArmor:

void cSlotAreaArmor::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill)
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





void cSlotAreaArmor::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	ASSERT((a_SlotNum >= 0) && (a_SlotNum < GetNumSlots()));

	// When the player is in creative mode, the client sends the new item as a_ClickedItem, not the current item in the slot.
	if (a_Player.IsGameModeCreative() && (m_ParentWindow.GetWindowType() == cWindow::wtInventory))
	{
		if ((a_ClickAction == caDropKey) || (a_ClickAction == caCtrlDropKey))
		{
			DropClicked(a_Player, a_SlotNum, (a_ClickAction == caCtrlDropKey));
			return;
		}

		SetSlot(a_SlotNum, a_Player, a_ClickedItem);
		return;
	}

	bool bAsync = false;
	if (GetSlot(a_SlotNum, a_Player) == nullptr)
	{
		LOGWARNING("GetSlot(%d) returned nullptr! Ignoring click", a_SlotNum);
		return;
	}

	switch (a_ClickAction)
	{
		case caDblClick:
		{
			// Armors haven't a dbl click
			return;
		}
		case caShiftLeftClick:
		case caShiftRightClick:
		{
			ShiftClicked(a_Player, a_SlotNum, a_ClickedItem);
			return;
		}
		case caMiddleClick:
		{
			MiddleClicked(a_Player, a_SlotNum);
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
	if ((a_ClickAction != caRightClick) && (a_ClickAction != caLeftClick))
	{
		LOGWARNING("SlotArea: Unhandled click action: %d (%s)", a_ClickAction, ClickActionToString(a_ClickAction));
		m_ParentWindow.BroadcastWholeWindow();
		return;
	}

	if (DraggingItem.IsEmpty() || CanPlaceArmorInSlot(a_SlotNum, DraggingItem))
	{
		// Swap contents
		cItem tmp(DraggingItem);
		DraggingItem = Slot;
		Slot = tmp;
	}

	SetSlot(a_SlotNum, a_Player, Slot);
	if (bAsync)
	{
		m_ParentWindow.BroadcastWholeWindow();
	}
}





bool cSlotAreaArmor::CanPlaceArmorInSlot(int a_SlotNum, const cItem & a_Item)
{
	switch (a_SlotNum)
	{
		case 0:  return (ItemCategory::IsHelmet(a_Item.m_ItemType) || (a_Item.m_ItemType == E_BLOCK_PUMPKIN));
		case 1:  return ItemCategory::IsChestPlate(a_Item.m_ItemType);
		case 2:  return ItemCategory::IsLeggings(a_Item.m_ItemType);
		case 3:  return ItemCategory::IsBoots(a_Item.m_ItemType);
		default: return false;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaItemGrid:

cSlotAreaItemGrid::cSlotAreaItemGrid(cItemGrid & a_ItemGrid, cWindow & a_ParentWindow) :
	Super(a_ItemGrid.GetNumSlots(), a_ParentWindow),
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





////////////////////////////////////////////////////////////////////////////////
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

		// Player not found, this should not happen, ever! Return nullptr, but things may break by this.
		return nullptr;
	}

	if (a_SlotNum >= static_cast<int>(itr->second.size()))
	{
		LOGERROR("cSlotAreaTemporary: asking for more slots than actually stored!");
		ASSERT(!"cSlotAreaTemporary: asking for more slots than actually stored!");
		return nullptr;
	}

	return &(itr->second[static_cast<size_t>(a_SlotNum)]);
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

	if (a_SlotNum >= static_cast<int>(itr->second.size()))
	{
		LOGERROR("cSlotAreaTemporary: asking for more slots than actually stored!");
		return;
	}

	itr->second[static_cast<size_t>(a_SlotNum)] = a_Item;

	m_ParentWindow.SendSlot(a_Player, this, a_SlotNum);
}





void cSlotAreaTemporary::OnPlayerAdded(cPlayer & a_Player)
{
	ASSERT(m_Items.find(a_Player.GetUniqueID()) == m_Items.end());  // The player shouldn't be in the itemmap, otherwise we probably have a leak
	m_Items[a_Player.GetUniqueID()].resize(static_cast<size_t>(m_NumSlots));  // Make the vector the specified size of empty items
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
		cItem & Item = itr->second[static_cast<size_t>(i)];
		if (!Item.IsEmpty())
		{
			Drops.push_back(Item);
		}
		Item.Empty();
	}  // for i - itr->second[]

	a_Player.TossItems(Drops);
}





cItem * cSlotAreaTemporary::GetPlayerSlots(cPlayer & a_Player)
{
	cItemMap::iterator itr = m_Items.find(a_Player.GetUniqueID());
	if (itr == m_Items.end())
	{
		return nullptr;
	}
	return itr->second.data();
}





////////////////////////////////////////////////////////////////////////////////
// cSlotAreaHorse:

cSlotAreaHorse::cSlotAreaHorse(cHorse & a_Horse, cWindow & a_ParentWindow) :
	cSlotArea(2, a_ParentWindow),
	m_Horse(a_Horse)
{
}





void cSlotAreaHorse::Clicked(cPlayer & a_Player, int a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	cItem & DraggingItem = a_Player.GetDraggingItem();

	switch (a_ClickAction)
	{
		case caLeftClick:
		case caRightClick:
		case caDblClick:
		{
			// Check for invalid item types
			if (DraggingItem.IsEmpty())
			{
				break;
			}

			switch (a_SlotNum)
			{
				case SaddleSlot:
				{
					if (DraggingItem.m_ItemType != E_ITEM_SADDLE)
					{
						return;
					}
				}
				case ArmorSlot:
				{
					if (!ItemCategory::IsHorseArmor(DraggingItem.m_ItemType))
					{
						return;
					}
				}
				default: break;
			}
		}
		default: break;
	}

	cSlotArea::Clicked(a_Player, a_SlotNum, a_ClickAction, a_ClickedItem);
}





const cItem * cSlotAreaHorse::GetSlot(int a_SlotNum, cPlayer & a_Player) const
{
	static const cItem InvalidItem;
	switch (a_SlotNum)
	{
		case SaddleSlot: return &m_Horse.GetHorseSaddle();
		case ArmorSlot:  return &m_Horse.GetHorseArmorItem();
		default:
		{
			LOGWARN("cSlotAreaHorse::GetSlot: Invalid slot number %d", a_SlotNum);
			return &InvalidItem;
		}
	}
}





void cSlotAreaHorse::SetSlot(int a_SlotNum, cPlayer & a_Player, const cItem & a_Item)
{
	switch (a_SlotNum)
	{
		case SaddleSlot: m_Horse.SetHorseSaddle(a_Item); break;
		case ArmorSlot:  m_Horse.SetHorseArmor(a_Item);  break;
		default:
		{
			LOGWARN("cSlotAreaHorse::SetSlot: Invalid slot number %d", a_SlotNum);
		}
	}
}





void cSlotAreaHorse::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, bool a_ShouldApply, bool a_KeepEmptySlots, bool a_BackFill)
{
	if (ItemCategory::IsHorseArmor(a_ItemStack.m_ItemType) && m_Horse.GetHorseArmorItem().IsEmpty())
	{
		if (a_ShouldApply)
		{
			m_Horse.SetHorseArmor(a_ItemStack.CopyOne());
		}
		--a_ItemStack.m_ItemCount;
	}
	else if ((a_ItemStack.m_ItemType == E_ITEM_SADDLE) && !m_Horse.IsSaddled())
	{
		if (a_ShouldApply)
		{
			m_Horse.SetHorseSaddle(a_ItemStack.CopyOne());
		}
		--a_ItemStack.m_ItemCount;
	}
}
