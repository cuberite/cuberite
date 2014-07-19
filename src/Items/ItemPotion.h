
#pragma once

#include "../Entities/EntityEffect.h"
#include "../Entities/SplashPotionEntity.h"

class cItemPotionHandler:
	public cItemHandler
{
	typedef cItemHandler super;
	
public:

	cItemPotionHandler():
		super(E_ITEM_POTION)
	{
	}
	

	/** Returns the potion color (used by the client for visuals), based on the potion's damage value */
	static int GetPotionColor(short a_ItemDamage)
	{
		// Lowest six bits
		return (a_ItemDamage & 0x3f);
	}
	
	
	/** Translates the potion's damage value into the entity effect that the potion gives */
	static cEntityEffect::eType GetEntityEffectType(short a_ItemDamage)
	{
		// Lowest four bits
		// Potion effect bits are different from entity effect values
		// For reference: http://minecraft.gamepedia.com/Data_values#.22Potion_effect.22_bits
		switch (a_ItemDamage & 0x0f)
		{
			case 0x01: return cEntityEffect::effRegeneration;
			case 0x02: return cEntityEffect::effSpeed;
			case 0x03: return cEntityEffect::effFireResistance;
			case 0x04: return cEntityEffect::effPoison;
			case 0x05: return cEntityEffect::effInstantHealth;
			case 0x06: return cEntityEffect::effNightVision;
			case 0x08: return cEntityEffect::effWeakness;
			case 0x09: return cEntityEffect::effStrength;
			case 0x0a: return cEntityEffect::effSlowness;
			case 0x0c: return cEntityEffect::effInstantDamage;
			case 0x0d: return cEntityEffect::effWaterBreathing;
			case 0x0e: return cEntityEffect::effInvisibility;
			
			// No effect potions
			case 0x00:
			case 0x07:
			case 0x0b:  // Will be potion of leaping in 1.8
			case 0x0f:
			{
				break;
			}
		}
		return cEntityEffect::effNoEffect;
	}


	/** Retrieves the intensity level from the potion's damage value.
	Returns 0 for level I potions, 1 for level II potions. */
	static short GetEntityEffectIntensity(short a_ItemDamage)
	{
		// Level II potion if the fifth lowest bit is set
		return ((a_ItemDamage & 0x20) != 0) ? 1 : 0;
	}
	
	
	/** Returns the effect duration, in ticks, based on the potion's damage value */
	static int GetEntityEffectDuration(short a_ItemDamage)
	{
		// Base duration in ticks
		int base = 0;
		double TierCoeff = 1, ExtCoeff = 1, SplashCoeff = 1;
		
		switch (GetEntityEffectType(a_ItemDamage))
		{
			case cEntityEffect::effRegeneration:
			case cEntityEffect::effPoison:
			{
				base = 900;
				break;
			}
				
			case cEntityEffect::effSpeed:
			case cEntityEffect::effFireResistance:
			case cEntityEffect::effNightVision:
			case cEntityEffect::effStrength:
			case cEntityEffect::effWaterBreathing:
			case cEntityEffect::effInvisibility:
			{
				base = 3600;
				break;
			}
				
			case cEntityEffect::effWeakness:
			case cEntityEffect::effSlowness:
			{
				base = 1800;
				break;
			}
		}
		
		// If potion is level II, half the duration. If not, stays the same
		TierCoeff = (GetEntityEffectIntensity(a_ItemDamage) > 0) ? 0.5 : 1;
		
		// If potion is extended, multiply duration by 8/3. If not, stays the same
		// Extended potion if sixth lowest bit is set
		ExtCoeff = (a_ItemDamage & 0x40) ? (8.0 / 3.0) : 1;
		
		// If potion is splash potion, multiply duration by 3/4. If not, stays the same
		SplashCoeff = IsPotionDrinkable(a_ItemDamage) ? 1 : 0.75;
		
		// Ref.:
		//   http://minecraft.gamepedia.com/Data_values#.22Tier.22_bit
		//   http://minecraft.gamepedia.com/Data_values#.22Extended_duration.22_bit
		//   http://minecraft.gamepedia.com/Data_values#.22Splash_potion.22_bit
		
		return (int)(base * TierCoeff * ExtCoeff * SplashCoeff);
	}


	/** Returns true if the potion with the given damage is drinkable */
	static bool IsPotionDrinkable(short a_ItemDamage)
	{
		// Drinkable potion if 13th lowest bit is set
		// Ref.: http://minecraft.gamepedia.com/Potions#Data_value_table
		return ((a_ItemDamage & 0x2000) != 0);
	}
	

	// cItemHandler overrides:
	virtual bool IsDrinkable(short a_ItemDamage) override
	{
		// Drinkable potion if 13th lowest bit is set
		// Ref.: http://minecraft.gamepedia.com/Potions#Data_value_table
		return IsPotionDrinkable(a_ItemDamage);
	}

	
	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		short PotionDamage = a_Item.m_ItemDamage;
		
		// Do not throw non-splash potions:
		if (IsPotionDrinkable(PotionDamage))
		{
			return false;
		}
		
		Vector3d Pos = a_Player->GetThrowStartPos();
		Vector3d Speed = a_Player->GetLookVector() * 7;
		
		cSplashPotionEntity * Projectile = new cSplashPotionEntity(
			a_Player, Pos.x, Pos.y, Pos.z, Speed,
			GetEntityEffectType(PotionDamage), cEntityEffect(GetEntityEffectDuration(PotionDamage),
			GetEntityEffectIntensity(PotionDamage)), GetPotionColor(PotionDamage)
		);
		if (Projectile == NULL)
		{
			return false;
		}
		if (!Projectile->Initialize(*a_World))
		{
			delete Projectile;
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
		if (!IsDrinkable(a_Item->m_ItemDamage))
		{
			return false;
		}
		
		a_Player->AddEntityEffect(GetEntityEffectType(PotionDamage), GetEntityEffectDuration(PotionDamage), GetEntityEffectIntensity(PotionDamage));
		a_Player->GetInventory().RemoveOneEquippedItem();
		a_Player->GetInventory().AddItem(E_ITEM_GLASS_BOTTLE);
		return true;
	}
};




