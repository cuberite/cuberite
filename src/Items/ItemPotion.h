
#pragma once

#include "../Entities/EntityEffects.h"

class cItemPotionHandler:
public cItemHandler
{
	typedef cItemHandler super;
	
	cEntityEffect::eType GetEntityEffectType(short a_ItemDamage)
	{
		// Potion effect bits are different from entity effect values
		// For reference: http://minecraft.gamepedia.com/Data_values#.22Potion_effect.22_bits
		switch (a_ItemDamage & 15)
		{
			case 1:  return cEntityEffect::efRegeneration;
			case 2:  return cEntityEffect::efSpeed;
			case 3:  return cEntityEffect::efFireResistance;
			case 4:  return cEntityEffect::efPoison;
			case 5:  return cEntityEffect::efInstantHealth;
			case 6:  return cEntityEffect::efNightVision;
			case 8:  return cEntityEffect::efWeakness;
			case 9:  return cEntityEffect::efStrength;
			case 10: return cEntityEffect::efSlowness;
			case 12: return cEntityEffect::efInstantDamage;
			case 13: return cEntityEffect::efWaterBreathing;
			case 14: return cEntityEffect::efInvisibility;
			
			// No effect potions
			case 0:
			case 7:
			case 11:
			case 15:
			{
				break;
			}
		}
		
		return cEntityEffect::efNoEffect;
	}
	
	short GetEntityEffectIntensity(short a_ItemDamage)
	{
		// Level II potion if fifth bit is set
		if (a_ItemDamage & 32) return 1;
		else                   return 0;
	}
	
	int GetEntityEffectDuration(short a_ItemDamage)
	{
		// Base duration in ticks
		int base = 0;
		double tier_multi = 1, ext_multi = 1, splash_multi = 1;
		
		switch (GetEntityEffectType(a_ItemDamage))
		{
			case cEntityEffect::efRegeneration:
			case cEntityEffect::efPoison:
			{
				base = 900;
				break;
			}
				
			case cEntityEffect::efSpeed:
			case cEntityEffect::efFireResistance:
			case cEntityEffect::efNightVision:
			case cEntityEffect::efStrength:
			case cEntityEffect::efWaterBreathing:
			case cEntityEffect::efInvisibility:
			{
				base = 3600;
				break;
			}
				
			case cEntityEffect::efWeakness:
			case cEntityEffect::efSlowness:
			{
				base = 1800;
				break;
			}
		}
		
		// If potion is level 2, half the duration. If not, stays the same
		tier_multi   = GetEntityEffectIntensity(a_ItemDamage) > 0 ? 0.5       : 1;
		
		// If potion is extended, multiply duration by 8/3. If not, stays the same
		// Extended potion if sixth bit is set
		ext_multi    = a_ItemDamage & 64                          ? (8.0/3.0) : 1;
		
		// If potion is splash potion, multiply duration by 3/4. If not, stays the same
		splash_multi = !IsDrinkable(a_ItemDamage)                 ? 0.75      : 1;
		
		// For reference: http://minecraft.gamepedia.com/Data_values#.22Tier.22_bit
		//                http://minecraft.gamepedia.com/Data_values#.22Extended_duration.22_bit
		//                http://minecraft.gamepedia.com/Data_values#.22Splash_potion.22_bit
		
		return base * tier_multi * ext_multi * splash_multi;
	}
	
	bool IsDrinkable(short a_ItemDamage)
	{
		// Drinkable potion if 13th bit is set
		// For reference: http://minecraft.gamepedia.com/Potions#Data_value_table
		return a_ItemDamage & 8192;
	}
	
public:
	cItemPotionHandler():
	super(E_ITEM_POTIONS)
	{
	}
	
	virtual bool IsDrinkable(const cItem * a_Item) override
	{
		return IsDrinkable(a_Item->m_ItemDamage);
	}
	
	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		// Called when potion is a splash potion
		return true;
	}
	
	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		// Called when potion is a drinkable potion
		short potion_damage = a_Item->m_ItemDamage;
		a_Player->AddEntityEffect(GetEntityEffectType(potion_damage),
								  cEntityEffect(GetEntityEffectDuration(potion_damage),
												GetEntityEffectIntensity(potion_damage),
												a_Player));
		a_Player->GetInventory().RemoveOneEquippedItem();
		a_Player->GetInventory().AddItem(E_ITEM_GLASS_BOTTLE);
		return true;
	}
};
