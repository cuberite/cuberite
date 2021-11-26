
#pragma once





class cItemMilkHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	constexpr cItemMilkHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}

	virtual bool IsDrinkable(short a_ItemDamage) const override
	{
		UNUSED(a_ItemDamage);
		return true;
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) const override
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
