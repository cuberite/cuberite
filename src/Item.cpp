
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
		case E_ITEM_DIAMOND_AXE:     return 1561;
		case E_ITEM_DIAMOND_HOE:     return 1561;
		case E_ITEM_DIAMOND_PICKAXE: return 1561;
		case E_ITEM_DIAMOND_SHOVEL:  return 1561;
		case E_ITEM_DIAMOND_SWORD:   return 1561;
		case E_ITEM_FLINT_AND_STEEL: return 64;
		case E_ITEM_GOLD_AXE:        return 32;
		case E_ITEM_GOLD_HOE:        return 32;
		case E_ITEM_GOLD_PICKAXE:    return 32;
		case E_ITEM_GOLD_SHOVEL:     return 32;
		case E_ITEM_GOLD_SWORD:      return 32;
		case E_ITEM_IRON_AXE:        return 250;
		case E_ITEM_IRON_HOE:        return 250;
		case E_ITEM_IRON_PICKAXE:    return 250;
		case E_ITEM_IRON_SHOVEL:     return 250;
		case E_ITEM_IRON_SWORD:      return 250;
		case E_ITEM_SHEARS:          return 250;
		case E_ITEM_STONE_AXE:       return 131;
		case E_ITEM_STONE_HOE:       return 131;
		case E_ITEM_STONE_PICKAXE:   return 131;
		case E_ITEM_STONE_SHOVEL:    return 131;
		case E_ITEM_STONE_SWORD:     return 131;
		case E_ITEM_WOODEN_AXE:      return 59;
		case E_ITEM_WOODEN_HOE:      return 59;
		case E_ITEM_WOODEN_PICKAXE:  return 59;
		case E_ITEM_WOODEN_SHOVEL:   return 59;
		case E_ITEM_WOODEN_SWORD:    return 59;
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
	return (m_ItemDamage > MaxDamage);
}





bool cItem::IsFullStack(void) const
{
	return (m_ItemCount >= ItemHandler(m_ItemType)->GetMaxStackSize());
}





char cItem::GetMaxStackSize(void) const
{
	return ItemHandler(m_ItemType)->GetMaxStackSize();
}





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

		if (m_ItemColor.IsValid())
		{
			a_OutValue["Color_Red"] = m_ItemColor.GetRed();
			a_OutValue["Color_Green"] = m_ItemColor.GetGreen();
			a_OutValue["Color_Blue"] = m_ItemColor.GetBlue();
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
	m_ItemType = static_cast<ENUM_ITEM_ID>(a_Value.get("ID", -1).asInt());
	if (m_ItemType > 0)
	{
		m_ItemCount = static_cast<char>(a_Value.get("Count", -1).asInt());
		m_ItemDamage = static_cast<short>(a_Value.get("Health", -1).asInt());
		m_Enchantments.Clear();
		m_Enchantments.AddFromString(a_Value.get("ench", "").asString());
		m_CustomName = a_Value.get("Name", "").asString();
		m_Lore = a_Value.get("Lore", "").asString();

		int red = a_Value.get("Color_Red", -1).asInt();
		int green = a_Value.get("Color_Green", -1).asInt();
		int blue = a_Value.get("Color_Blue", -1).asInt();
		if ((red > -1) && (red < static_cast<int>(cColor::COLOR_LIMIT)) && (green > -1) && (green < static_cast<int>(cColor::COLOR_LIMIT)) && (blue > -1) && (blue < static_cast<int>(cColor::COLOR_LIMIT)))
		{
			m_ItemColor.SetColor(static_cast<unsigned char>(red), static_cast<unsigned char>(green), static_cast<unsigned char>(blue));
		}
		else if ((red != -1) || (blue != -1) || (green != -1))
		{
			LOGWARNING("Item with invalid red, green, and blue values read in from json file.");
		}

		if ((m_ItemType == E_ITEM_FIREWORK_ROCKET) || (m_ItemType == E_ITEM_FIREWORK_STAR))
		{
			m_FireworkItem.m_HasFlicker = a_Value.get("Flicker", false).asBool();
			m_FireworkItem.m_HasTrail = a_Value.get("Trail", false).asBool();
			m_FireworkItem.m_Type = static_cast<NIBBLETYPE>(a_Value.get("Type", 0).asInt());
			m_FireworkItem.m_FlightTimeInTicks = static_cast<short>(a_Value.get("FlightTimeInTicks", 0).asInt());
			m_FireworkItem.ColoursFromString(a_Value.get("Colours", "").asString(), m_FireworkItem);
			m_FireworkItem.FadeColoursFromString(a_Value.get("FadeColours", "").asString(), m_FireworkItem);
		}

		m_RepairCost = a_Value.get("RepairCost", 0).asInt();
	}
}





bool cItem::IsEnchantable(short a_ItemType, bool a_WithBook)
{
	if (
		ItemCategory::IsAxe(a_ItemType) ||
		ItemCategory::IsSword(a_ItemType) ||
		ItemCategory::IsShovel(a_ItemType) ||
		ItemCategory::IsPickaxe(a_ItemType) ||
		(a_WithBook && ItemCategory::IsHoe(a_ItemType)) ||
		ItemCategory::IsArmor(a_ItemType)
	)
	{
		return true;
	}

	switch (a_ItemType)
	{
		case E_ITEM_BOOK:
		case E_ITEM_BOW:
		case E_ITEM_FISHING_ROD:
		{
			return true;
		}

		case E_ITEM_CARROT_ON_STICK:
		case E_ITEM_SHEARS:
		case E_ITEM_FLINT_AND_STEEL:
		{
			return a_WithBook;
		}
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
	if (!cItem::IsEnchantable(m_ItemType))
	{
		return false;
	}

	int Enchantability = GetEnchantability();
	if (Enchantability == 0)
	{
		return false;
	}

	cFastRandom Random;
	int ModifiedEnchantmentLevel = a_NumXPLevels + static_cast<int>(Random.NextFloat(static_cast<float>(Enchantability / 4))) + static_cast<int>(Random.NextFloat(static_cast<float>(Enchantability / 4))) + 1;
	float RandomBonus = 1.0F + (Random.NextFloat(1) + Random.NextFloat(1) - 1.0F) * 0.15F;
	int FinalEnchantmentLevel = static_cast<int>(ModifiedEnchantmentLevel * RandomBonus + 0.5F);

	cWeightedEnchantments Enchantments;
	cEnchantments::AddItemEnchantmentWeights(Enchantments, m_ItemType, FinalEnchantmentLevel);

	if (m_ItemType == E_ITEM_BOOK)
	{
		m_ItemType = E_ITEM_ENCHANTED_BOOK;
	}

	cEnchantments Enchantment1 = cEnchantments::GetRandomEnchantmentFromVector(Enchantments);
	m_Enchantments.AddFromString(Enchantment1.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(Enchantments, Enchantment1);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(Enchantments, Enchantment1);

	float NewEnchantmentLevel = static_cast<float>(a_NumXPLevels);

	// Next Enchantment (Second)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float SecondEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (Enchantments.empty() || (Random.NextFloat(100) > SecondEnchantmentChance))
	{
		return true;
	}

	cEnchantments Enchantment2 = cEnchantments::GetRandomEnchantmentFromVector(Enchantments);
	m_Enchantments.AddFromString(Enchantment2.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(Enchantments, Enchantment2);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(Enchantments, Enchantment2);

	// Next Enchantment (Third)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float ThirdEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (Enchantments.empty() || (Random.NextFloat(100) > ThirdEnchantmentChance))
	{
		return true;
	}

	cEnchantments Enchantment3 = cEnchantments::GetRandomEnchantmentFromVector(Enchantments);
	m_Enchantments.AddFromString(Enchantment3.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(Enchantments, Enchantment3);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(Enchantments, Enchantment3);

	// Next Enchantment (Fourth)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float FourthEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (Enchantments.empty() || (Random.NextFloat(100) > FourthEnchantmentChance))
	{
		return true;
	}
	cEnchantments Enchantment4 = cEnchantments::GetRandomEnchantmentFromVector(Enchantments);
	m_Enchantments.AddFromString(Enchantment4.ToString());

	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cItems:

cItem * cItems::Get(int a_Idx)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to get an out-of-bounds item at index %d; there are currently " SIZE_T_FMT " items. Returning a nil.", a_Idx, size());
		return nullptr;
	}
	return &at(static_cast<size_t>(a_Idx));
}





void cItems::Set(int a_Idx, const cItem & a_Item)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently " SIZE_T_FMT " items. Not setting.", a_Idx, size());
		return;
	}
	at(static_cast<size_t>(a_Idx)) = a_Item;
}





void cItems::Delete(int a_Idx)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to delete an item at an out-of-bounds index %d; there are currently " SIZE_T_FMT " items. Ignoring.", a_Idx, size());
		return;
	}
	erase(begin() + a_Idx);
}





void cItems::Set(int a_Idx, short a_ItemType, char a_ItemCount, short a_ItemDamage)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently " SIZE_T_FMT " items. Not setting.", a_Idx, size());
		return;
	}
	at(static_cast<size_t>(a_Idx)) = cItem(a_ItemType, a_ItemCount, a_ItemDamage);
}




