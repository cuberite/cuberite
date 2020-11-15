
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Item.h"
#include "BlockType.h"
#include "ItemGrid.h"
#include "json/json.h"
#include "Items/ItemHandler.h"

#include "FastRandom.h"





cItem::cItem():
	m_ItemType(E_ITEM_EMPTY),
	m_ItemCount(0),
	m_ItemDamage(0),
	m_CustomName(""),
	m_RepairCost(0),
	m_FireworkItem(),
	m_ItemColor()
{
}





cItem::cItem(
	short a_ItemType,
	char a_ItemCount,
	short a_ItemDamage,
	const AString & a_Enchantments,
	const AString & a_CustomName,
	const AStringVector & a_LoreTable
):
	m_ItemType    (a_ItemType),
	m_ItemCount   (a_ItemCount),
	m_ItemDamage  (a_ItemDamage),
	m_Enchantments(a_Enchantments),
	m_CustomName  (a_CustomName),
	m_LoreTable   (a_LoreTable),
	m_RepairCost  (0),
	m_FireworkItem(),
	m_ItemColor()
{
	if (!IsValidItem(m_ItemType))
	{
		if ((m_ItemType != E_BLOCK_AIR) && (m_ItemType != E_ITEM_EMPTY))
		{
			LOGWARNING("%s: creating an invalid item type (%d), resetting to empty.", __FUNCTION__, a_ItemType);
		}
		Empty();
	}
}





void cItem::Empty()
{
	m_ItemType = E_ITEM_EMPTY;
	m_ItemCount = 0;
	m_ItemDamage = 0;
	m_Enchantments.Clear();
	m_CustomName = "";
	m_LoreTable.clear();
	m_RepairCost = 0;
	m_FireworkItem.EmptyData();
	m_ItemColor.Clear();
}





void cItem::Clear()
{
	m_ItemType = E_ITEM_EMPTY;
	m_ItemCount = 0;
	m_ItemDamage = 0;
	m_RepairCost = 0;
	m_ItemColor.Clear();
}





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
		case E_ITEM_CHAIN_BOOTS:     return 196;
		case E_ITEM_CHAIN_CHESTPLATE:return 241;
		case E_ITEM_CHAIN_HELMET:    return 166;
		case E_ITEM_CHAIN_LEGGINGS:  return 226;
		case E_ITEM_DIAMOND_AXE:     return 1561;
		case E_ITEM_DIAMOND_BOOTS:   return 430;
		case E_ITEM_DIAMOND_CHESTPLATE: return 529;
		case E_ITEM_DIAMOND_HELMET:  return 364;
		case E_ITEM_DIAMOND_HOE:     return 1561;
		case E_ITEM_DIAMOND_LEGGINGS:return 496;
		case E_ITEM_DIAMOND_PICKAXE: return 1561;
		case E_ITEM_DIAMOND_SHOVEL:  return 1561;
		case E_ITEM_DIAMOND_SWORD:   return 1561;
		case E_ITEM_FLINT_AND_STEEL: return 64;
		case E_ITEM_FISHING_ROD:     return 65;
		case E_ITEM_GOLD_AXE:        return 32;
		case E_ITEM_GOLD_BOOTS:      return 92;
		case E_ITEM_GOLD_CHESTPLATE: return 113;
		case E_ITEM_GOLD_HELMET:     return 78;
		case E_ITEM_GOLD_HOE:        return 32;
		case E_ITEM_GOLD_LEGGINGS:   return 106;
		case E_ITEM_GOLD_PICKAXE:    return 32;
		case E_ITEM_GOLD_SHOVEL:     return 32;
		case E_ITEM_GOLD_SWORD:      return 32;
		case E_ITEM_IRON_AXE:        return 250;
		case E_ITEM_IRON_BOOTS:      return 196;
		case E_ITEM_IRON_CHESTPLATE: return 241;
		case E_ITEM_IRON_HELMET:     return 166;
		case E_ITEM_IRON_HOE:        return 250;
		case E_ITEM_IRON_LEGGINGS:   return 226;
		case E_ITEM_IRON_PICKAXE:    return 250;
		case E_ITEM_IRON_SHOVEL:     return 250;
		case E_ITEM_IRON_SWORD:      return 250;
		case E_ITEM_LEATHER_BOOTS:   return 66;
		case E_ITEM_LEATHER_CAP:     return 55;
		case E_ITEM_LEATHER_PANTS:   return 76;
		case E_ITEM_LEATHER_TUNIC:   return 81;
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

		default: return 0;
	}
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
			auto & LoreArray = (a_OutValue["Lore"] = Json::Value(Json::arrayValue));

			for (const auto & Line : m_LoreTable)
			{
				LoreArray.append(Line);
			}
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
	m_ItemType = static_cast<ENUM_ITEM_TYPE>(a_Value.get("ID", -1).asInt());
	if (m_ItemType > 0)
	{
		m_ItemCount = static_cast<char>(a_Value.get("Count", -1).asInt());
		m_ItemDamage = static_cast<short>(a_Value.get("Health", -1).asInt());
		m_Enchantments.Clear();
		m_Enchantments.AddFromString(a_Value.get("ench", "").asString());
		m_CustomName = a_Value.get("Name", "").asString();
		auto Lore = a_Value.get("Lore", Json::arrayValue);
		for (auto & Line : Lore)
		{
			m_LoreTable.push_back(Line.asString());
		}

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





bool cItem::IsEnchantable(short a_ItemType, bool a_FromBook)
{
	if (
		ItemCategory::IsAxe(a_ItemType) ||
		ItemCategory::IsSword(a_ItemType) ||
		ItemCategory::IsShovel(a_ItemType) ||
		ItemCategory::IsPickaxe(a_ItemType) ||
		(a_FromBook && ItemCategory::IsHoe(a_ItemType)) ||
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
			return a_FromBook;
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





bool cItem::EnchantByXPLevels(int a_NumXPLevels, MTRand & a_Random)
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

	int ModifiedEnchantmentLevel = a_NumXPLevels + a_Random.RandInt(Enchantability / 4) + a_Random.RandInt(Enchantability / 4) + 1;
	float RandomBonus = 1.0F + (a_Random.RandReal() + a_Random.RandReal() - 1.0F) * 0.15F;
	int FinalEnchantmentLevel = static_cast<int>(ModifiedEnchantmentLevel * RandomBonus + 0.5F);

	cWeightedEnchantments Enchantments;
	cEnchantments::AddItemEnchantmentWeights(Enchantments, m_ItemType, FinalEnchantmentLevel);

	if (m_ItemType == E_ITEM_BOOK)
	{
		m_ItemType = E_ITEM_ENCHANTED_BOOK;
	}

	cEnchantments Enchantment1 = cEnchantments::GetRandomEnchantmentFromVector(Enchantments, a_Random);
	m_Enchantments.AddFromString(Enchantment1.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(Enchantments, Enchantment1);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(Enchantments, Enchantment1);

	// Next Enchantment (Second)
	float NewEnchantmentLevel = a_NumXPLevels / 2.0f;
	float SecondEnchantmentChance = (NewEnchantmentLevel + 1) / 50.0f;
	if (Enchantments.empty() || !a_Random.RandBool(SecondEnchantmentChance))
	{
		return true;
	}

	cEnchantments Enchantment2 = cEnchantments::GetRandomEnchantmentFromVector(Enchantments, a_Random);
	m_Enchantments.AddFromString(Enchantment2.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(Enchantments, Enchantment2);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(Enchantments, Enchantment2);

	// Next Enchantment (Third)
	NewEnchantmentLevel = NewEnchantmentLevel / 2.0f;
	float ThirdEnchantmentChance = (NewEnchantmentLevel + 1) / 50.0f;
	if (Enchantments.empty() || !a_Random.RandBool(ThirdEnchantmentChance))
	{
		return true;
	}

	cEnchantments Enchantment3 = cEnchantments::GetRandomEnchantmentFromVector(Enchantments, a_Random);
	m_Enchantments.AddFromString(Enchantment3.ToString());
	cEnchantments::RemoveEnchantmentWeightFromVector(Enchantments, Enchantment3);

	// Checking for conflicting enchantments
	cEnchantments::CheckEnchantmentConflictsFromVector(Enchantments, Enchantment3);

	// Next Enchantment (Fourth)
	NewEnchantmentLevel = NewEnchantmentLevel / 2.0f;
	float FourthEnchantmentChance = (NewEnchantmentLevel + 1) / 50.0f;
	if (Enchantments.empty() || !a_Random.RandBool(FourthEnchantmentChance))
	{
		return true;
	}
	cEnchantments Enchantment4 = cEnchantments::GetRandomEnchantmentFromVector(Enchantments, a_Random);
	m_Enchantments.AddFromString(Enchantment4.ToString());

	return true;
}





int cItem::AddEnchantment(int a_EnchantmentID, unsigned int a_Level, bool a_FromBook)
{
	unsigned int OurLevel = m_Enchantments.GetLevel(a_EnchantmentID);
	int Multiplier = cEnchantments::GetXPCostMultiplier(a_EnchantmentID, a_FromBook);
	unsigned int NewLevel = 0;
	if (OurLevel > a_Level)
	{
		// They don't add anything to us
		NewLevel = OurLevel;
	}
	else if (OurLevel == a_Level)
	{
		// Bump it by 1
		NewLevel = OurLevel + 1;
	}
	else
	{
		// Take the sacrifice's level
		NewLevel = a_Level;
	}
	unsigned int LevelCap = cEnchantments::GetLevelCap(a_EnchantmentID);
	if (NewLevel > LevelCap)
	{
		NewLevel = LevelCap;
	}

	m_Enchantments.SetLevel(a_EnchantmentID, NewLevel);
	return static_cast<int>(NewLevel) * Multiplier;
}





bool cItem::CanHaveEnchantment(int a_EnchantmentID)
{
	if (m_ItemType == E_ITEM_ENCHANTED_BOOK)
	{
		// Enchanted books can take anything
		return true;
	}

	// The organization here is based on the summary at:
	// https://minecraft.gamepedia.com/Enchanting
	// as of July 2017 (Minecraft 1.12).

	// Hand tool enchantments
	static const std::set<int> SwordEnchantments =
	{
		cEnchantments::enchBaneOfArthropods,
		cEnchantments::enchFireAspect,
		cEnchantments::enchKnockback,
		cEnchantments::enchLooting,
		cEnchantments::enchSharpness,
		cEnchantments::enchSmite,
		cEnchantments::enchUnbreaking
	};
	static const std::set<int> AxeEnchantments =
	{
		cEnchantments::enchBaneOfArthropods,
		cEnchantments::enchEfficiency,
		cEnchantments::enchFortune,
		cEnchantments::enchSharpness,
		cEnchantments::enchSilkTouch,
		cEnchantments::enchSmite,
		cEnchantments::enchUnbreaking
	};
	static const std::set<int> ToolEnchantments =
	{
		cEnchantments::enchEfficiency,
		cEnchantments::enchFortune,
		cEnchantments::enchSilkTouch,
		cEnchantments::enchUnbreaking
	};
	static const std::set<int> ShearEnchantments =
	{
		cEnchantments::enchEfficiency,
		cEnchantments::enchUnbreaking
	};
	static const std::set<int> BowEnchantments =
	{
		cEnchantments::enchFlame,
		cEnchantments::enchInfinity,
		cEnchantments::enchPower,
		cEnchantments::enchPunch
	};
	static const std::set<int> FishingEnchantments =
	{
		cEnchantments::enchLuckOfTheSea,
		cEnchantments::enchLure
	};
	static const std::set<int> MiscEnchantments =
	{
		cEnchantments::enchUnbreaking
	};

	if (ItemCategory::IsSword(m_ItemType))
	{
		return SwordEnchantments.count(a_EnchantmentID) > 0;
	}
	if (ItemCategory::IsAxe(m_ItemType))
	{
		return AxeEnchantments.count(a_EnchantmentID) > 0;
	}
	if (ItemCategory::IsPickaxe(m_ItemType) || ItemCategory::IsShovel(m_ItemType))
	{
		return ToolEnchantments.count(a_EnchantmentID) > 0;
	}
	if (m_ItemType == E_ITEM_SHEARS)
	{
		return ShearEnchantments.count(a_EnchantmentID) > 0;
	}
	if (m_ItemType == E_ITEM_BOW)
	{
		return BowEnchantments.count(a_EnchantmentID) > 0;
	}
	if (m_ItemType == E_ITEM_FISHING_ROD)
	{
		return FishingEnchantments.count(a_EnchantmentID) > 0;
	}
	if (ItemCategory::IsHoe(m_ItemType) || (m_ItemType == E_ITEM_FLINT_AND_STEEL) || (m_ItemType == E_ITEM_CARROT_ON_STICK) || (m_ItemType == E_ITEM_SHIELD))
	{
		return MiscEnchantments.count(a_EnchantmentID) > 0;
	}

	// Armor enchantments
	static const std::set<int> ArmorEnchantments =
	{
		cEnchantments::enchBlastProtection,
		cEnchantments::enchFireProtection,
		cEnchantments::enchProjectileProtection,
		cEnchantments::enchProtection,
		cEnchantments::enchThorns,
		cEnchantments::enchUnbreaking
	};
	static const std::set<int> HatOnlyEnchantments =
	{
		cEnchantments::enchAquaAffinity,
		cEnchantments::enchRespiration
	};
	static const std::set<int> BootOnlyEnchantments =
	{
		cEnchantments::enchDepthStrider,
		cEnchantments::enchFeatherFalling
	};

	if (ItemCategory::IsBoots(m_ItemType))
	{
		return (BootOnlyEnchantments.count(a_EnchantmentID) > 0) || (ArmorEnchantments.count(a_EnchantmentID) > 0);
	}
	if (ItemCategory::IsHelmet(m_ItemType))
	{
		return (HatOnlyEnchantments.count(a_EnchantmentID) > 0) || (ArmorEnchantments.count(a_EnchantmentID) > 0);
	}
	if (ItemCategory::IsArmor(m_ItemType))
	{
		return ArmorEnchantments.count(a_EnchantmentID) > 0;
	}
	return false;
}





int cItem::AddEnchantmentsFromItem(const cItem & a_Other)
{
	bool FromBook = (a_Other.m_ItemType == E_ITEM_ENCHANTED_BOOK);

	// Consider each enchantment seperately
	int EnchantingCost = 0;
	for (auto & Enchantment : a_Other.m_Enchantments)
	{
		if (CanHaveEnchantment(Enchantment.first))
		{
			if (!m_Enchantments.CanAddEnchantment(Enchantment.first))
			{
				// Cost of incompatible enchantments
				EnchantingCost += 1;
			}
			else
			{
				EnchantingCost += AddEnchantment(Enchantment.first, Enchantment.second, FromBook);
			}
		}
	}
	return EnchantingCost;
}





////////////////////////////////////////////////////////////////////////////////
// cItems:

cItems::cItems(cItem && a_InitialItem)
{
	push_back(std::move(a_InitialItem));
}





cItem * cItems::Get(int a_Idx)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to get an out-of-bounds item at index %d; there are currently %zu items. Returning a nil.", a_Idx, size());
		return nullptr;
	}
	return &at(static_cast<size_t>(a_Idx));
}





void cItems::Set(int a_Idx, const cItem & a_Item)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently %zu items. Not setting.", a_Idx, size());
		return;
	}
	at(static_cast<size_t>(a_Idx)) = a_Item;
}





void cItems::Delete(int a_Idx)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to delete an item at an out-of-bounds index %d; there are currently %zu items. Ignoring.", a_Idx, size());
		return;
	}
	erase(begin() + a_Idx);
}





void cItems::Set(int a_Idx, short a_ItemType, char a_ItemCount, short a_ItemDamage)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently %zu items. Not setting.", a_Idx, size());
		return;
	}
	at(static_cast<size_t>(a_Idx)) = cItem(a_ItemType, a_ItemCount, a_ItemDamage);
}





bool cItems::Contains(const cItem & a_Item)
{
	for (const auto & itr : *this)
	{
		if (a_Item.IsEqual(itr))
		{
			return true;
		}
	}
	return false;
}





bool cItems::ContainsType(const cItem & a_Item)
{
	for (const auto & itr : *this)
	{
		if (a_Item.IsSameType(itr))
		{
			return true;
		}
	}
	return false;
}





void cItems::AddItemGrid(const cItemGrid & a_ItemGrid)
{
	for (int i = 0; i < a_ItemGrid.GetNumSlots(); ++i)
	{
		const auto & Slot = a_ItemGrid.GetSlot(i);
		if (!Slot.IsEmpty())
		{
			Add(Slot);
		}
	}
}
