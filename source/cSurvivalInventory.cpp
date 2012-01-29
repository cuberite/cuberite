
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSurvivalInventory.h"
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cWindow.h"
#include "cItem.h"
#include "cRecipeChecker.h"
#include "cRoot.h"
#include "packets/cPacket_WindowClick.h"





cSurvivalInventory::~cSurvivalInventory()
{
	
}

cSurvivalInventory::cSurvivalInventory(cPlayer* a_Owner)
	: cInventory(a_Owner)
{
	
}

void cSurvivalInventory::Clicked( cPacket* a_ClickPacket )
{
	cPacket_WindowClick *Packet = reinterpret_cast<cPacket_WindowClick *>(a_ClickPacket);
	bool bDontCook = false;
	if( GetWindow() )
	{
		// Override for craft result slot
		if( Packet->m_SlotNum == (short)c_CraftOffset )
		{
			LOG("In craft slot: %i x %i !!", m_Slots[c_CraftOffset].m_ItemID, m_Slots[c_CraftOffset].m_ItemCount );
			cItem* DraggingItem = GetWindow()->GetDraggingItem();
			if( DraggingItem->IsEmpty() )
			{
				*DraggingItem = m_Slots[c_CraftOffset];
				m_Slots[c_CraftOffset].Empty();
			}
			else if( DraggingItem->Equals( m_Slots[c_CraftOffset] ) )
			{
				if( DraggingItem->m_ItemCount + m_Slots[c_CraftOffset].m_ItemCount <= 64 )
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
			LOG("Dragging Dish %i", DraggingItem->m_ItemCount );
		}
		else
		{
			GetWindow()->Clicked( Packet, *m_Owner );
		}
	}
	else
	{
		LOG("No Inventory window! WTF");
	}

	if( Packet->m_SlotNum >= (short)c_CraftOffset && Packet->m_SlotNum < (short)(c_CraftOffset+c_CraftSlots+1) )
	{
		cItem CookedItem;
		if( Packet->m_SlotNum == 0 && !bDontCook )
		{
			CookedItem = cRoot::Get()->GetRecipeChecker()->CookIngredients( m_Slots+c_CraftOffset+1, 2, 2, true );
		}
		else
		{
			CookedItem = cRoot::Get()->GetRecipeChecker()->CookIngredients( m_Slots+c_CraftOffset+1, 2, 2 );
		}
		m_Slots[c_CraftOffset] = CookedItem;
		LOG("You cooked: %i x %i !!", m_Slots[c_CraftOffset].m_ItemID, m_Slots[c_CraftOffset].m_ItemCount );
		SendWholeInventory( m_Owner->GetClientHandle() );
	}
	SendSlot( 0 );
}
