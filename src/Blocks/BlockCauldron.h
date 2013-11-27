
#pragma once

#include "BlockHandler.h"





class cBlockCauldronHandler :
	public cBlockHandler
{
public:
	cBlockCauldronHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_CAULDRON, 1, 0));
	}

	void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		char Meta = a_World->GetBlockMeta( a_BlockX, a_BlockY, a_BlockZ );
		switch( a_Player->GetEquippedItem().m_ItemType )
		{
			case E_ITEM_WATER_BUCKET:
			{
				a_World->SetBlockMeta( a_BlockX, a_BlockY, a_BlockZ, 3 );
				a_Player->GetInventory().RemoveOneEquippedItem();
				cItem NewItem(E_ITEM_BUCKET, 1);
				a_Player->GetInventory().AddItem(NewItem);
				break;
			}
			case E_ITEM_GLASS_BOTTLE:
			{
				if( Meta > 0 )
				{
					a_World->SetBlockMeta( a_BlockX, a_BlockY, a_BlockZ, --Meta);
					a_Player->GetInventory().RemoveOneEquippedItem();
					cItem NewItem(E_ITEM_POTIONS, 1, 0);
					a_Player->GetInventory().AddItem(NewItem);
				}
				break;
			}
		}
	}

	virtual bool IsUseable() override
	{
		return true;
	}
} ;




