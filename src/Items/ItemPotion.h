
#pragma once

#include "../Entities/EntityEffect.h"


class cItemPotionHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemPotionHandler():
		Super(E_ITEM_POTION)
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
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		short PotionDamage = a_HeldItem.m_ItemDamage;

		// Do not throw non-splash potions:
		if (cEntityEffect::IsPotionDrinkable(PotionDamage))
		{
			return false;
		}

		Vector3d Pos = a_Player->GetThrowStartPos();
		Vector3d Speed = a_Player->GetLookVector() * 14;

		// Play sound
		a_World->BroadcastSoundEffect("entity.arrow.shoot", a_Player->GetPosition() - Vector3d(0, a_Player->GetHeight(), 0), 0.5f, 0.4f / GetRandomProvider().RandReal(0.8f, 1.2f));

		if (a_World->CreateProjectile(Pos.x, Pos.y, Pos.z, cProjectileEntity::pkSplashPotion, a_Player, &a_Player->GetEquippedItem(), &Speed) == cEntity::INVALID_ID)
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}





	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
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
			a_Player->ReplaceOneEquippedItemTossRest(cItem(E_ITEM_GLASS_BOTTLE));
		}
		return true;
	}
};




