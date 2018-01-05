
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

	virtual bool IsDrinkable(short a_ItemDamage) override
	{
		UNUSED(a_ItemDamage);
		return true;
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		UNUSED(a_Item);
		a_Player->ClearEntityEffects();

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
			a_Player->GetInventory().AddItem(E_ITEM_BUCKET);
		}
		return true;
	}
};
