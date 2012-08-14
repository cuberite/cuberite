
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

#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_InventorySlot.h"





cCraftingWindow::cCraftingWindow( cWindowOwner* a_Owner, bool a_bInventoryVisible )
	 : cWindow(a_Owner, a_bInventoryVisible, cWindow::Workbench, 1)
{
	cItem * Slots = new cItem[10];
	SetSlots( Slots, 10 );
}





void cCraftingWindow::Clicked( cPacket_WindowClick* a_ClickPacket, cPlayer & a_Player )
{
	bool bDontCook = false;
	
	cItem * DraggingItem = GetDraggingItem(&a_Player);
	if (
		a_ClickPacket->m_IsShiftPressed && 
		((DraggingItem == NULL) || DraggingItem->IsEmpty())
	)
	{
		ShiftClicked(a_ClickPacket, a_Player);
		return;
	}
	
	// Override for craft result slot
	if (a_ClickPacket->m_SlotNum == 0)
	{
		LOG("In craft slot: %i x %i !!", GetSlot(0)->m_ItemID, GetSlot(0)->m_ItemCount );
		cItem* DraggingItem = GetDraggingItem( &a_Player );
		if( DraggingItem->m_ItemID <= 0 )
		{
			*DraggingItem = *GetSlot(0);
			GetSlot(0)->Empty();
		}
		else if( DraggingItem->Equals( *GetSlot(0) ) )
		{
			if( DraggingItem->m_ItemCount + GetSlot(0)->m_ItemCount <= 64 )
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
		LOG("Dragging Dish %i", DraggingItem->m_ItemCount );
	}
	else
	{
		cWindow::Clicked( a_ClickPacket, a_Player );
	}

	if ((a_ClickPacket->m_SlotNum >= 0) && (a_ClickPacket->m_SlotNum < 10))
	{
		cCraftingGrid   Grid(GetSlots() + 1, 3, 3);
		cCraftingRecipe Recipe(Grid);
		
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(&a_Player, Grid, Recipe);

		if ((a_ClickPacket->m_SlotNum == 0) && !bDontCook)
		{
			// Consume the items from the crafting grid:
			Recipe.ConsumeIngredients(Grid);
			
			// Propagate grid back to m_Slots:
			Grid.CopyToItems(GetSlots() + 1);
			
			// Get the recipe for the new grid contents:
			cRoot::Get()->GetCraftingRecipes()->GetRecipe(&a_Player, Grid, Recipe);
		}
		*GetSlot(0) = Recipe.GetResult();
		LOGD("%s cooked: %i x %i !!", a_Player.GetName().c_str(), GetSlot(0)->m_ItemID, GetSlot(0)->m_ItemCount );
	}
	SendWholeWindow( a_Player.GetClientHandle() );
	a_Player.GetInventory().SendWholeInventory( a_Player.GetClientHandle() );

	// Separate packet for result =/ Don't know why
	cPacket_InventorySlot Packet;
	Packet.m_WindowID = (char)GetWindowID();
	Packet.m_SlotNum = 0;
	Packet.m_ItemID = (short)GetSlot(0)->m_ItemID;
	Packet.m_ItemCount = GetSlot(0)->m_ItemCount;
	Packet.m_ItemUses = (char)GetSlot(0)->m_ItemHealth;
	a_Player.GetClientHandle()->Send( Packet );
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





void cCraftingWindow::ShiftClicked(cPacket_WindowClick * a_ClickPacket, cPlayer & a_Player)
{
	short Slot = a_ClickPacket->m_SlotNum;
	if (Slot == SLOT_CRAFTING_RESULT)
	{
		ShiftClickedCraftingResult(Slot, a_Player);
	}
	else if ((Slot >= SLOT_CRAFTING_MIN) && (Slot <= SLOT_CRAFTING_MAX))
	{
		ShiftClickedCraftingGrid(Slot, a_Player);
	}
	else
	{
		// No need to handle inventory shift-click, it is handled by the underlying cSurvivalInventory, surprise surprise ;)
	}
	SendWholeWindow(a_Player.GetClientHandle());
}





void cCraftingWindow::ShiftClickedCraftingResult(short a_Slot, cPlayer & a_Player)
{
	// TODO
}





void cCraftingWindow::ShiftClickedCraftingGrid(short a_Slot, cPlayer & a_Player)
{
	enum
	{
		SLOT_INVENTORY_MIN = 9,
		SLOT_INVENTORY_MAX = 35,
		SLOT_HOTBAR_MIN    = 36,
		SLOT_HOTBAR_MAX    = 44
	} ;
	
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




