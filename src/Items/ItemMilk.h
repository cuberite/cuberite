
#pragma once

class cItemMilkHandler:
	public cItemHandler
{
	typedef cItemHandler super;
public:
	cItemMilkHandler():
		super(E_ITEM_MILK)
	{
	}
	
	virtual bool IsDrinkable(void) override
	{
		return true;
	}
	
	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		a_Player->ClearEntityEffects();
		a_Player->GetInventory().RemoveOneEquippedItem();
		a_Player->GetInventory().AddItem(E_ITEM_BUCKET);
		return true;
	}
};
