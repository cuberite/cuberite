
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
	}
}





void cItem::FromJson(const Json::Value & a_Value)
{
	m_ItemType = (ENUM_ITEM_ID)a_Value.get("ID", -1 ).asInt();
	if (m_ItemType > 0)
	{
		m_ItemCount = (char)a_Value.get("Count", -1 ).asInt();
		m_ItemDamage = (short)a_Value.get("Health", -1 ).asInt();
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
	}
}





bool cItem::IsEnchantable(short item)
{
	if ((item >= 256) && (item <= 259))
		return true;
	if ((item >= 267) && (item <= 279))
		return true;
	if ((item >= 283) && (item <= 286))	
		return true;
	if ((item >= 290) && (item <= 294))
		return true;
	if ((item >= 298) && (item <= 317))
		return true;
	if ((item >= 290) && (item <= 294))
		return true;

	if ((item == 346) || (item == 359) || (item == 261))
		return true;

	return false;
}





int cItem::GetEnchantability()
{
	int Enchantability = 0;

	if (m_ItemType == E_ITEM_WOODEN_SWORD || m_ItemType == E_ITEM_WOODEN_PICKAXE || m_ItemType == E_ITEM_WOODEN_SHOVEL || m_ItemType == E_ITEM_WOODEN_AXE || m_ItemType == E_ITEM_WOODEN_HOE)
	{
		Enchantability = 15;
	}
	else if (m_ItemType == E_ITEM_LEATHER_CAP || m_ItemType == E_ITEM_LEATHER_TUNIC || m_ItemType == E_ITEM_LEATHER_PANTS || m_ItemType == E_ITEM_LEATHER_BOOTS)
	{
		Enchantability = 15;
	}
	else if (m_ItemType == E_ITEM_STONE_SWORD || m_ItemType == E_ITEM_STONE_PICKAXE || m_ItemType == E_ITEM_STONE_SHOVEL || m_ItemType == E_ITEM_STONE_AXE || m_ItemType == E_ITEM_STONE_HOE)
	{
		Enchantability = 5;
	}
	else if (m_ItemType == E_ITEM_IRON_HELMET || m_ItemType == E_ITEM_IRON_CHESTPLATE || m_ItemType == E_ITEM_IRON_LEGGINGS || m_ItemType == E_ITEM_IRON_BOOTS)
	{
		Enchantability = 9;
	}
	else if (m_ItemType == E_ITEM_IRON_SWORD || m_ItemType == E_ITEM_IRON_PICKAXE || m_ItemType == E_ITEM_IRON_SHOVEL || m_ItemType == E_ITEM_IRON_AXE || m_ItemType == E_ITEM_IRON_HOE)
	{
		Enchantability = 14;
	}
	else if (m_ItemType == E_ITEM_CHAIN_HELMET || m_ItemType == E_ITEM_CHAIN_CHESTPLATE || m_ItemType == E_ITEM_CHAIN_LEGGINGS || m_ItemType == E_ITEM_CHAIN_BOOTS)
	{
		Enchantability = 12;
	}
	else if (m_ItemType == E_ITEM_DIAMOND_HELMET || m_ItemType == E_ITEM_DIAMOND_CHESTPLATE || m_ItemType == E_ITEM_DIAMOND_LEGGINGS || m_ItemType == E_ITEM_DIAMOND_BOOTS)
	{
		Enchantability = 10;
	}
	else if (m_ItemType == E_ITEM_DIAMOND_SWORD || m_ItemType == E_ITEM_DIAMOND_PICKAXE || m_ItemType == E_ITEM_DIAMOND_SHOVEL || m_ItemType == E_ITEM_DIAMOND_AXE || m_ItemType == E_ITEM_DIAMOND_HOE)
	{
		Enchantability = 10;
	}
	else if (m_ItemType == E_ITEM_GOLD_HELMET || m_ItemType == E_ITEM_GOLD_CHESTPLATE || m_ItemType == E_ITEM_GOLD_LEGGINGS || m_ItemType == E_ITEM_GOLD_BOOTS)
	{
		Enchantability = 25;
	}
	else if (m_ItemType == E_ITEM_GOLD_SWORD || m_ItemType == E_ITEM_GOLD_PICKAXE || m_ItemType == E_ITEM_GOLD_SHOVEL || m_ItemType == E_ITEM_GOLD_AXE || m_ItemType == E_ITEM_GOLD_HOE)
	{
		Enchantability = 22;
	}
	else if (m_ItemType == E_ITEM_FISHING_ROD || m_ItemType == E_ITEM_BOW || m_ItemType == E_ITEM_BOOK)
	{
		Enchantability = 1;
	}

	return Enchantability;
}





bool cItem::EnchantByXPLevels(int a_NumXPLevels, cPlayer & a_Player, bool a_BlockConflictingEnchantments, bool a_DeleteLevelFromPlayer)
{
	if (!cItem::IsEnchantable(m_ItemType) && m_ItemType != E_ITEM_BOOK)
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

	cEnchantments Enchantment1 = cEnchantments::GetRandomEnchantmentFromVector(enchantments);
	m_Enchantments.AddFromString(Enchantment1.ToString());
	enchantments.erase(std::remove(enchantments.begin(), enchantments.end(), Enchantment1), enchantments.end());

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(enchantments, Enchantment1);

	float NewEnchantmentLevel = (float)a_NumXPLevels;

	// Next Enchantment (Second)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float SecondEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (Random.NextFloat(100) <= SecondEnchantmentChance)
	{
		if (enchantments.size() > 0)
		{
			cEnchantments Enchantment2 = cEnchantments::GetRandomEnchantmentFromVector(enchantments);
			m_Enchantments.AddFromString(Enchantment2.ToString());
			enchantments.erase(std::remove(enchantments.begin(), enchantments.end(), Enchantment2), enchantments.end());

			// Checking for conflicting enchantments
			cEnchantments::CheckEnchantmentConflictsFromVector(enchantments, Enchantment2);
		}
	}

	// Next Enchantment (Third)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float ThirdEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (Random.NextFloat(100) <= ThirdEnchantmentChance)
	{
		if (enchantments.size() > 0)
		{
			cEnchantments Enchantment3 = cEnchantments::GetRandomEnchantmentFromVector(enchantments);
			m_Enchantments.AddFromString(Enchantment3.ToString());
			enchantments.erase(std::remove(enchantments.begin(), enchantments.end(), Enchantment3), enchantments.end());

			// Checking for conflicting enchantments
			cEnchantments::CheckEnchantmentConflictsFromVector(enchantments, Enchantment3);
		}
	}

	// Next Enchantment (Fourth)
	NewEnchantmentLevel = NewEnchantmentLevel / 2;
	float FourthEnchantmentChance = (NewEnchantmentLevel + 1) / 50 * 100;
	if (Random.NextFloat(100) <= FourthEnchantmentChance)
	{
		if (enchantments.size() > 0)
		{
			cEnchantments Enchantment4 = cEnchantments::GetRandomEnchantmentFromVector(enchantments);
			m_Enchantments.AddFromString(Enchantment4.ToString());
			enchantments.erase(std::remove(enchantments.begin(), enchantments.end(), Enchantment4), enchantments.end());
		}
	}

	if (a_Player.DeltaExperience(-a_Player.XpForLevel(a_NumXPLevels)) >= 0 || a_Player.IsGameModeCreative())
	{
		return true;
	}

	return false;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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




