
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cCraftingWindow.h"
#include "cItem.h"
#include "cRecipeChecker.h"
#include "CraftingRecipes.h"
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cInventory.h"
#include "cPickup.h"
#include "cRoot.h"

#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_InventorySlot.h"





cCraftingWindow::cCraftingWindow( cWindowOwner* a_Owner, bool a_bInventoryVisible )
	 : cWindow( a_Owner, a_bInventoryVisible )
{
	SetWindowID( 1 );
	SetWindowType( cWindow::Workbench ); // Workbench

	cItem* Slots = new cItem[10];
	SetSlots( Slots, 10 );
}





void cCraftingWindow::Clicked( cPacket_WindowClick* a_ClickPacket, cPlayer & a_Player )
{
	bool bDontCook = false;
	// Override for craft result slot
	if( a_ClickPacket->m_SlotNum == 0 )
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
		cItem CookedItem;
		if ((a_ClickPacket->m_SlotNum == 0) && !bDontCook)
		{
			// Consume the ingredients from the crafting grid:
			CookedItem = cRoot::Get()->GetCraftingRecipes()->Craft(GetSlots() + 1, 3, 3);
			LOGD("New recipes crafted: %i x %i", CookedItem.m_ItemID, CookedItem.m_ItemCount);
			// Upgrade the crafting result from the new crafting grid contents:
			CookedItem = cRoot::Get()->GetCraftingRecipes()->Offer(GetSlots() + 1, 3, 3);
			if (CookedItem.IsEmpty())
			{
				// Fallback to the old recipes:
				CookedItem = cRoot::Get()->GetRecipeChecker()->CookIngredients( GetSlots()+1, 3, 3, true );
				LOGD("Old recipes crafted: %i x %i", CookedItem.m_ItemID, CookedItem.m_ItemCount );
			}
		}
		else
		{
			CookedItem = cRoot::Get()->GetCraftingRecipes()->Offer(GetSlots() + 1, 3, 3);
			LOGD("New recipes offer: %i x %i", CookedItem.m_ItemID, CookedItem.m_ItemCount );
			if (CookedItem.IsEmpty())
			{
				// Fallback to the old recipes
				CookedItem = cRoot::Get()->GetRecipeChecker()->CookIngredients( GetSlots()+1, 3, 3 );
				LOGD("Old recipes offer: %i x %i", CookedItem.m_ItemID, CookedItem.m_ItemCount );
			}
		}
		*GetSlot(0) = CookedItem;
		LOG("You cooked: %i x %i !!", GetSlot(0)->m_ItemID, GetSlot(0)->m_ItemCount );
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





void cCraftingWindow::Close( cPlayer & a_Player )
{
	// Start from slot 1, don't drop what's in the result slot
	for( int i = 1; i < GetNumSlots(); i++ )
	{
		cItem* Item = GetSlot( i );
		if( Item->m_ItemID > 0 && Item->m_ItemCount > 0 )
		{
			float vX = 0, vY = 0, vZ = 0;
			EulerToVector( -a_Player.GetRotation(), a_Player.GetPitch(), vZ, vX, vY );
			vY = -vY*2 + 1.f;
			cPickup* Pickup = new cPickup( (int)(a_Player.GetPosX()*32), (int)(a_Player.GetPosY()*32) + (int)(1.6f*32), (int)(a_Player.GetPosZ()*32), *Item, vX*2, vY*2, vZ*2 );
			Pickup->Initialize( a_Player.GetWorld() );
		}
		Item->Empty();
	}
	cWindow::Close( a_Player );
}




