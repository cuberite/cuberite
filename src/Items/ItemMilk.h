
#pragma once





class cItemMilkHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemMilkHandler():
		Super(E_ITEM_MILK)
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
			a_Player->ReplaceOneEquippedItemTossRest(cItem(E_ITEM_BUCKET));
		}
		return true;
	}
};
