
#pragma once

#include "../Entities/EntityEffect.h"


class cItemPotionHandler:
	public cItemHandler
{
	typedef cItemHandler super;

public:

	cItemPotionHandler():
		super(E_ITEM_POTION)
	{
	}


	// cItemHandler overrides:
	virtual bool IsDrinkable(short a_ItemDamage) override
	{
		// Drinkable potion if 13th lowest bit is set
		// Ref.: https://minecraft.gamepedia.com/Potions#Data_value_table
		return cEntityEffect::IsPotionDrinkable(a_ItemDamage);
	}


	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface,
		const cItem & a_Item, eHand a_Hand
	) override
	{
		short PotionDamage = a_Item.m_ItemDamage;

		// Do not throw non-splash potions:
		if (cEntityEffect::IsPotionDrinkable(PotionDamage))
		{
			return false;
		}

		Vector3d Pos = a_Player->GetThrowStartPos();
		Vector3d Speed = a_Player->GetLookVector() * 7;

		if (a_World->CreateProjectile(Pos.x, Pos.y, Pos.z, cProjectileEntity::pkSplashPotion, a_Player, &a_Player->GetEquippedItem(), &Speed) == cEntity::INVALID_ID)
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem(a_Hand);
		}

		return true;
	}


	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item, eHand a_Hand) override
	{
		short PotionDamage = a_Item->m_ItemDamage;

		// Do not drink undrinkable potions:
		if (!cEntityEffect::IsPotionDrinkable(a_Item->m_ItemDamage))
		{
			return false;
		}

		a_Player->AddEntityEffect(
			cEntityEffect::GetPotionEffectType(PotionDamage),
			cEntityEffect::GetPotionEffectDuration(PotionDamage),
			cEntityEffect::GetPotionEffectIntensity(PotionDamage)
		);

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem(a_Hand);
			a_Player->GetInventory().AddItem(E_ITEM_GLASS_BOTTLE);
		}
		return true;
	}
};




