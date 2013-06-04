
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Item.h"
#include <json/json.h>





cItem cItem::CopyOne(void) const
{
	cItem res(*this);
	res.m_ItemCount = 1;
	return res;
}





short cItem::GetMaxDamage(void) const
{
	switch (m_ItemType)
	{
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





bool cItem::IsStackableWith(const cItem & a_OtherStack) const
{
	if (a_OtherStack.m_ItemType != m_ItemType)
	{
		return false;
	}
	if (a_OtherStack.m_ItemDamage != m_ItemDamage)
	{
		return false;
	}
	if (a_OtherStack.m_Enchantments != m_Enchantments)
	{
		return false;
	}
	
	return true;
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





void cItems::Set(int a_Idx, ENUM_ITEM_ID a_ItemType, char a_ItemCount, short a_ItemDamage)
{
	if ((a_Idx < 0) || (a_Idx >= (int)size()))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently %d items. Not setting.", a_Idx, size());
		return;
	}
	at(a_Idx) = cItem(a_ItemType, a_ItemCount, a_ItemDamage);
}




