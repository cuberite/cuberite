
// ItemMinecart.h

// Declares the various minecart ItemHandlers





#pragma once

// Not needed, we're being included only from ItemHandler.cpp which already has this file: #include "ItemHandler.h"
#include "../Minecart.h"





class cItemMinecartHandler :
	public cItemHandler
{
	typedef cItemHandler super;
	
public:
	cItemMinecartHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}
	
	
	
	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, cItem * a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (a_Dir < 0)
		{
			return false;
		}
		
		// Check that there's rail in there:
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		switch (Block)
		{
			case E_BLOCK_MINECART_TRACKS:
			case E_BLOCK_POWERED_RAIL:
			case E_BLOCK_DETECTOR_RAIL:
			{
				// These are allowed
				break;
			}
			default:
			{
				LOGD("Used minecart on an unsuitable block %d (%s)", Block, ItemTypeToString(Block).c_str());
				return false;
			}
		}
		
		cMinecart::ePayload Payload = cMinecart::mpNone;
		switch (m_ItemType)
		{
			case E_ITEM_MINECART:         Payload = cMinecart::mpNone; break;
			case E_ITEM_CHEST_MINECART:   Payload = cMinecart::mpChest; break;
			case E_ITEM_FURNACE_MINECART: Payload = cMinecart::mpFurnace; break;
			default:
			{
				ASSERT(!"Unhandled minecart item");
				return false;
			}
		}  // switch (m_ItemType)
		cMinecart * Minecart = new cMinecart(Payload, (double)a_BlockX + 0.5, a_BlockY, (double)a_BlockZ + 0.5);
		a_World->AddEntity(Minecart);
		Minecart->Initialize(a_World);
		return true;
	}
	
} ;




