
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Item.h"
#include "json/json.h"
#include "Items/ItemHandler.h"





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





void cItem::GetApplicableEnchantmentsForType(short a_ItemType, cEnchantmentsArray & a_Enchantments)
{
	LOGWARN("Get Enchantments");

	if (ItemCategory::IsSword(a_ItemType))
	{
		a_Enchantments.push_back(cEnchantments("Sharpness=4"));
		a_Enchantments.push_back(cEnchantments("Smite=5"));
		a_Enchantments.push_back(cEnchantments("BaneOfArthropods=5"));
		a_Enchantments.push_back(cEnchantments("Knockback=2"));
		a_Enchantments.push_back(cEnchantments("FireAspect=2"));
		a_Enchantments.push_back(cEnchantments("Looting=3"));
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
	}

	else if (ItemCategory::IsPickaxe(a_ItemType))
	{
		a_Enchantments.push_back(cEnchantments("Efficiency=4"));
		a_Enchantments.push_back(cEnchantments("SilkTouch=1"));
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
		a_Enchantments.push_back(cEnchantments("Fortune=3"));
	}

	else if (ItemCategory::IsAxe(a_ItemType))
	{
		a_Enchantments.push_back(cEnchantments("Efficiency=4"));
		a_Enchantments.push_back(cEnchantments("SilkTouch=1"));
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
		a_Enchantments.push_back(cEnchantments("Fortune=3"));
	}

	else if (ItemCategory::IsShovel(a_ItemType))
	{
		a_Enchantments.push_back(cEnchantments("Efficiency=4"));
		a_Enchantments.push_back(cEnchantments("SilkTouch=1"));
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
		a_Enchantments.push_back(cEnchantments("Fortune=3"));
	}

	else if (ItemCategory::IsHoe(a_ItemType))
	{
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
	}

	else if (ItemCategory::IsArmor(a_ItemType))
	{
		a_Enchantments.push_back(cEnchantments("Protection=4"));
		a_Enchantments.push_back(cEnchantments("FireProtection=4"));
		a_Enchantments.push_back(cEnchantments("BlastProtection=4"));
		a_Enchantments.push_back(cEnchantments("ProjectileProtection=4"));
		a_Enchantments.push_back(cEnchantments("Thorns=3"));
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));

		if (ItemCategory::IsHelmet(a_ItemType))
		{
			a_Enchantments.push_back(cEnchantments("Respiration=3"));
			a_Enchantments.push_back(cEnchantments("AquaAffinity=1"));
		}

		else if (ItemCategory::IsBoots(a_ItemType))
		{
			a_Enchantments.push_back(cEnchantments("FeatherFalling=4"));
		}
	}

	//Bow
	else if (a_ItemType == 261)
	{
		a_Enchantments.push_back(cEnchantments("Power=4"));
		a_Enchantments.push_back(cEnchantments("Punch=2"));
		a_Enchantments.push_back(cEnchantments("Flame=1"));
		a_Enchantments.push_back(cEnchantments("Infinity=1"));
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
	}

	//Fishing Rod
	else if (a_ItemType == 346)
	{
		a_Enchantments.push_back(cEnchantments("LuckOfTheSea=3"));
		a_Enchantments.push_back(cEnchantments("Lure=3"));
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
	}

	//Shears
	else if (a_ItemType == 359)
	{
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
	}

	//Flint and Steel 
	else if (a_ItemType == 259)
	{
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
	}

	//Carrot on a Stick
	else if (a_ItemType == 398)
	{
		a_Enchantments.push_back(cEnchantments("Unbreaking=3"));
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cItems:

cItem * cItems::Get(int a_Idx)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to get an out-of-bounds item at index %d; there are currently %d items. Returning a nil.", a_Idx, size());
		return NULL;
	}
	return &at(a_Idx);
}





void cItems::Set(int a_Idx, const cItem & a_Item)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently %d items. Not setting.", a_Idx, size());
		return;
	}
	at(a_Idx) = a_Item;
}





void cItems::Delete(int a_Idx)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to delete an item at an out-of-bounds index %d; there are currently %d items. Ignoring.", a_Idx, size());
		return;
	}
	erase(begin() + a_Idx);
}





void cItems::Set(int a_Idx, short a_ItemType, char a_ItemCount, short a_ItemDamage)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently %d items. Not setting.", a_Idx, size());
		return;
	}
	at(a_Idx) = cItem(a_ItemType, a_ItemCount, a_ItemDamage);
}




