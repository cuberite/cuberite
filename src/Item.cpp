
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Item.h"
#include "json/json.h"
#include "Items/ItemHandler.h"

#include "FastRandom.h"





cItem cItem::CopyOne(void) const
{
	cItem res(*this);
	res.m_ItemCount = 1;
	return res;
}





cItem & cItem::AddCount(char a_AmountToAdd)
{
	m_ItemCount += a_AmountToAdd;
	if (m_ItemCount <= 0)
	{
		Empty();
	}
	return *this;
}





short cItem::GetMaxDamage(void) const
{
	switch (m_ItemType)
	{
		case E_ITEM_BOW:             return 384;
		case E_ITEM_DIAMOND_AXE:     return 1563;
		case E_ITEM_DIAMOND_HOE:     return 1563;
		case E_ITEM_DIAMOND_PICKAXE: return 1563;
		case E_ITEM_DIAMOND_SHOVEL:  return 1563;
		case E_ITEM_DIAMOND_SWORD:   return 1563;
		case E_ITEM_FLINT_AND_STEEL: return 65;
		case E_ITEM_GOLD_AXE:        return 32;
		case E_ITEM_GOLD_HOE:        return 32;
		case E_ITEM_GOLD_PICKAXE:    return 32;
		case E_ITEM_GOLD_SHOVEL:     return 32;
		case E_ITEM_GOLD_SWORD:      return 32;
		case E_ITEM_IRON_AXE:        return 251;
		case E_ITEM_IRON_HOE:        return 251;
		case E_ITEM_IRON_PICKAXE:    return 251;
		case E_ITEM_IRON_SHOVEL:     return 251;
		case E_ITEM_IRON_SWORD:      return 251;
		case E_ITEM_SHEARS:          return 251;
		case E_ITEM_STONE_AXE:       return 132;
		case E_ITEM_STONE_HOE:       return 132;
		case E_ITEM_STONE_PICKAXE:   return 132;
		case E_ITEM_STONE_SHOVEL:    return 132;
		case E_ITEM_STONE_SWORD:     return 132;
		case E_ITEM_WOODEN_AXE:      return 60;
		case E_ITEM_WOODEN_HOE:      return 60;
		case E_ITEM_WOODEN_PICKAXE:  return 60;
		case E_ITEM_WOODEN_SHOVEL:   return 60;
		case E_ITEM_WOODEN_SWORD:    return 60;
	}
	return 0;
}





bool cItem::DamageItem(short a_Amount)
{
	short MaxDamage = GetMaxDamage();
	if (MaxDamage == 0)
	{
		// Item doesn't have damage
		return false;
	}

	m_ItemDamage += a_Amount;
	return (m_ItemDamage >= MaxDamage);
}





bool cItem::IsFullStack(void) const
{
	return (m_ItemCount >= ItemHandler(m_ItemType)->GetMaxStackSize());
}





char cItem::GetMaxStackSize(void) const
{
	return ItemHandler(m_ItemType)->GetMaxStackSize();
}





/// Returns the cItemHandler responsible for this item type
cItemHandler * cItem::GetHandler(void) const
{
	return ItemHandler(m_ItemType);
}





void cItem::GetJson(Json::Value & a_OutValue) const
{
	a_OutValue["ID"] = m_ItemType;
	if (m_ItemType > 0)
	{
		a_OutValue["Count"] = m_ItemCount;
		a_OutValue["Health"] = m_ItemDamage;
		AString Enchantments(m_Enchantments.ToString());
		if (!Enchantments.empty())
		{
			a_OutValue["ench"] = Enchantments;
		}
		if (!IsCustomNameEmpty())
		{
			a_OutValue["Name"] = m_CustomName;
		}
		if (!IsLoreEmpty())
		{
			a_OutValue["Lore"] = m_Lore;
		}

		if ((m_ItemType == E_ITEM_FIREWORK_ROCKET) || (m_ItemType == E_ITEM_FIREWORK_STAR))
		{
			a_OutValue["Flicker"] = m_FireworkItem.m_HasFlicker;
			a_OutValue["Trail"] = m_FireworkItem.m_HasTrail;
			a_OutValue["Type"] = m_FireworkItem.m_Type;
			a_OutValue["FlightTimeInTicks"] = m_FireworkItem.m_FlightTimeInTicks;
			a_OutValue["Colours"] = m_FireworkItem.ColoursToString(m_FireworkItem);
			a_OutValue["FadeColours"] = m_FireworkItem.FadeColoursToString(m_FireworkItem);
		}

		a_OutValue["RepairCost"] = m_RepairCost;
	}
}





void cItem::FromJson(const Json::Value & a_Value)
{
	m_ItemType = (ENUM_ITEM_ID)a_Value.get("ID", -1).asInt();
	if (m_ItemType > 0)
	{
		m_ItemCount = (char)a_Value.get("Count", -1).asInt();
		m_ItemDamage = (short)a_Value.get("Health", -1).asInt();
		m_Enchantments.Clear();
		m_Enchantments.AddFromString(a_Value.get("ench", "").asString());
		m_CustomName = a_Value.get("Name", "").asString();
		m_Lore = a_Value.get("Lore", "").asString();

		if ((m_ItemType == E_ITEM_FIREWORK_ROCKET) || (m_ItemType == E_ITEM_FIREWORK_STAR))
		{
			m_FireworkItem.m_HasFlicker = a_Value.get("Flicker", false).asBool();
			m_FireworkItem.m_HasTrail = a_Value.get("Trail", false).asBool();
			m_FireworkItem.m_Type = (NIBBLETYPE)a_Value.get("Type", 0).asInt();
			m_FireworkItem.m_FlightTimeInTicks = (short)a_Value.get("FlightTimeInTicks", 0).asInt();
			m_FireworkItem.ColoursFromString(a_Value.get("Colours", "").asString(), m_FireworkItem);
			m_FireworkItem.FadeColoursFromString(a_Value.get("FadeColours", "").asString(), m_FireworkItem);
		}

		m_RepairCost = a_Value.get("RepairCost", 0).asInt();
	}
}





bool cItem::IsEnchantable(short item)
{
	if ((item >= 256) && (item <= 259))
	{
		return true;
	}
	if ((item >= 267) && (item <= 279))
	{
		return true;
	}
	if ((item >= 283) && (item <= 286))
	{
		return true;
	}
	if ((item >= 290) && (item <= 294))
	{
		return true;
	}
	if ((item >= 298) && (item <= 317))
	{
		return true;
	}
	if ((item == 346) || (item == 359) || (item == 261))
	{
		return true;
	}

	return false;
}





int cItem::GetEnchantability()
{
	int Enchantability = 0;

	switch (m_ItemType)
	{
		case E_ITEM_WOODEN_SWORD: Enchantability = 15; break;
		case E_ITEM_WOODEN_PICKAXE: Enchantability = 15; break;
		case E_ITEM_WOODEN_SHOVEL: Enchantability = 15; break;
		case E_ITEM_WOODEN_AXE: Enchantability = 15; break;
		case E_ITEM_WOODEN_HOE: Enchantability = 15; break;

		case E_ITEM_LEATHER_CAP: Enchantability = 15; break;
		case E_ITEM_LEATHER_TUNIC: Enchantability = 15; break;
		case E_ITEM_LEATHER_PANTS: Enchantability = 15; break;
		case E_ITEM_LEATHER_BOOTS: Enchantability = 15; break;

		case E_ITEM_STONE_SWORD: Enchantability = 5; break;
		case E_ITEM_STONE_PICKAXE: Enchantability = 5; break;
		case E_ITEM_STONE_SHOVEL: Enchantability = 5; break;
		case E_ITEM_STONE_AXE: Enchantability = 5; break;
		case E_ITEM_STONE_HOE: Enchantability = 5; break;

		case E_ITEM_IRON_HELMET: Enchantability = 9; break;
		case E_ITEM_IRON_CHESTPLATE: Enchantability = 9; break;
		case E_ITEM_IRON_LEGGINGS: Enchantability = 9; break;
		case E_ITEM_IRON_BOOTS: Enchantability = 9; break;

		case E_ITEM_IRON_SWORD: Enchantability = 14; break;
		case E_ITEM_IRON_PICKAXE: Enchantability = 14; break;
		case E_ITEM_IRON_SHOVEL: Enchantability = 14; break;
		case E_ITEM_IRON_AXE: Enchantability = 14; break;
		case E_ITEM_IRON_HOE: Enchantability = 14; break;

		case E_ITEM_CHAIN_HELMET: Enchantability = 12; break;
		case E_ITEM_CHAIN_CHESTPLATE: Enchantability = 12; break;
		case E_ITEM_CHAIN_LEGGINGS: Enchantability = 12; break;
		case E_ITEM_CHAIN_BOOTS: Enchantability = 12; break;

		case E_ITEM_DIAMOND_HELMET: Enchantability = 10; break;
		case E_ITEM_DIAMOND_CHESTPLATE: Enchantability = 10; break;
		case E_ITEM_DIAMOND_LEGGINGS: Enchantability = 10; break;
		case E_ITEM_DIAMOND_BOOTS: Enchantability = 10; break;

		case E_ITEM_DIAMOND_SWORD: Enchantability = 10; break;
		case E_ITEM_DIAMOND_PICKAXE: Enchantability = 10; break;
		case E_ITEM_DIAMOND_SHOVEL: Enchantability = 10; break;
		case E_ITEM_DIAMOND_AXE: Enchantability = 10; break;
		case E_ITEM_DIAMOND_HOE: Enchantability = 10; break;

		case E_ITEM_GOLD_HELMET: Enchantability = 25; break;
		case E_ITEM_GOLD_CHESTPLATE: Enchantability = 25; break;
		case E_ITEM_GOLD_LEGGINGS: Enchantability = 25; break;
		case E_ITEM_GOLD_BOOTS: Enchantability = 25; break;

		case E_ITEM_GOLD_SWORD: Enchantability = 22; break;
		case E_ITEM_GOLD_PICKAXE: Enchantability = 22; break;
		case E_ITEM_GOLD_SHOVEL: Enchantability = 22; break;
		case E_ITEM_GOLD_AXE: Enchantability = 22; break;
		case E_ITEM_GOLD_HOE: Enchantability = 22; break;

		case E_ITEM_FISHING_ROD: Enchantability = 1; break;
		case E_ITEM_BOW: Enchantability = 1; break;
		case E_ITEM_BOOK: Enchantability = 1; break;
	}

	return Enchantability;
}





bool cItem::EnchantByXPLevels(int a_NumXPLevels)
{
	if (!cItem::IsEnchantable(m_ItemType) && (m_ItemType != E_ITEM_BOOK))
	{
		return false;
	}

	int Enchantability = GetEnchantability();

	cFastRandom Random;
	int ModifiedEnchantmentLevel = a_NumXPLevels + (int)Random.NextFloat((float)Enchantability / 4) + (int)Random.NextFloat((float)Enchantability / 4) + 1;
	float RandomBonus = 1.0F + (Random.NextFloat(1) + Random.NextFloat(1) - 1.0F) * 0.15F;
	int FinalEnchantmentLevel = (int)(ModifiedEnchantmentLevel * RandomBonus + 0.5F);

	cWeightedEnchantments enchantments;
	cEnchantments::AddItemEnchantmentWeights(enchantments, m_ItemType, FinalEnchantmentLevel);

	if (m_ItemType == E_ITEM_BOOK)
	{
		m_ItemType = E_ITEM_ENCHANTED_BOOK;
	}

	cEnchantments Enchantment1 = cEnchantments::GetRandomEnchantmentFromVector(enchantments);
	m_Enchantments.AddFromString(Enchantment1.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(enchantments, Enchantment1);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(enchantments, Enchantment1);

	float NewEnchantmentLevel = (float)a_NumXPLevels;

	// Next Enchantment (Second)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float SecondEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (enchantments.empty() || (Random.NextFloat(100) > SecondEnchantmentChance))
	{
		return true;
	}

	cEnchantments Enchantment2 = cEnchantments::GetRandomEnchantmentFromVector(enchantments);
	m_Enchantments.AddFromString(Enchantment2.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(enchantments, Enchantment2);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(enchantments, Enchantment2);

	// Next Enchantment (Third)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float ThirdEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (enchantments.empty() || (Random.NextFloat(100) > ThirdEnchantmentChance))
	{
		return true;
	}

	cEnchantments Enchantment3 = cEnchantments::GetRandomEnchantmentFromVector(enchantments);
	m_Enchantments.AddFromString(Enchantment3.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(enchantments, Enchantment3);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(enchantments, Enchantment3);

	// Next Enchantment (Fourth)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float FourthEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (enchantments.empty() || (Random.NextFloat(100) > FourthEnchantmentChance))
	{
		return true;
	}
	cEnchantments Enchantment4 = cEnchantments::GetRandomEnchantmentFromVector(enchantments);
	m_Enchantments.AddFromString(Enchantment4.ToString());

	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cItems:

cItem * cItems::Get(int a_Idx)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to get an out-of-bounds item at index %d; there are currently " SIZE_T_FMT " items. Returning a nil.", a_Idx, size());
		return NULL;
	}
	return &at(a_Idx);
}





void cItems::Set(int a_Idx, const cItem & a_Item)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently " SIZE_T_FMT " items. Not setting.", a_Idx, size());
		return;
	}
	at(a_Idx) = a_Item;
}





void cItems::Delete(int a_Idx)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to delete an item at an out-of-bounds index %d; there are currently " SIZE_T_FMT " items. Ignoring.", a_Idx, size());
		return;
	}
	erase(begin() + a_Idx);
}





void cItems::Set(int a_Idx, short a_ItemType, char a_ItemCount, short a_ItemDamage)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently " SIZE_T_FMT " items. Not setting.", a_Idx, size());
		return;
	}
	at(a_Idx) = cItem(a_ItemType, a_ItemCount, a_ItemDamage);
}




