
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Item.h"
#include "BlockType.h"
#include "ItemGrid.h"
#include "json/json.h"
#include "Items/ItemHandler.h"

#include "FastRandom.h"





cItem::cItem():
	m_ItemType(Item::Air),
	m_ItemCount(0),
	m_ItemDamage(0),
	m_CustomName(),
	m_RepairCost(0),
	m_FireworkItem(),
	m_ItemColor()
{
}





cItem::cItem(
	enum Item a_ItemType,
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
}





void cItem::Empty()
{
	m_ItemType = Item::Air;
	m_ItemCount = 0;
	m_ItemDamage = 0;
	m_Enchantments.Clear();
	m_CustomName = "";
	m_LoreTable.clear();
	m_RepairCost = 0;
	m_FireworkItem.EmptyData();
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
		case Item::Bow:                  return 384;
		case Item::ChainmailBoots:       return 196;
		case Item::ChainmailChestplate:  return 241;
		case Item::ChainmailHelmet:      return 166;
		case Item::ChainmailLeggings:    return 226;
		case Item::DiamondAxe:           return 1561;
		case Item::DiamondBoots:         return 430;
		case Item::DiamondChestplate:    return 529;
		case Item::DiamondHelmet:        return 364;
		case Item::DiamondHoe:           return 1561;
		case Item::DiamondLeggings:      return 496;
		case Item::DiamondPickaxe:       return 1561;
		case Item::DiamondShovel:        return 1561;
		case Item::DiamondSword:         return 1561;
		case Item::Elytra:               return 432;
		case Item::FlintAndSteel:        return 64;
		case Item::FishingRod:           return 65;
		case Item::GoldenAxe:            return 32;
		case Item::GoldenBoots:          return 92;
		case Item::GoldenChestplate:     return 113;
		case Item::GoldenHelmet:         return 78;
		case Item::GoldenHoe:            return 32;
		case Item::GoldenLeggings:       return 106;
		case Item::GoldenPickaxe:        return 32;
		case Item::GoldenShovel:         return 32;
		case Item::GoldenSword:          return 32;
		case Item::IronAxe:              return 250;
		case Item::IronBoots:            return 196;
		case Item::IronChestplate:       return 241;
		case Item::IronHelmet:           return 166;
		case Item::IronHoe:              return 250;
		case Item::IronLeggings:         return 226;
		case Item::IronPickaxe:          return 250;
		case Item::IronShovel:           return 250;
		case Item::IronSword:            return 250;
		case Item::LeatherBoots:         return 66;
		case Item::LeatherHelmet:        return 55;
		case Item::LeatherLeggings:      return 76;
		case Item::LeatherChestplate:    return 81;
		case Item::Shears:               return 250;
		case Item::StoneAxe:             return 131;
		case Item::StoneHoe:             return 131;
		case Item::StonePickaxe:         return 131;
		case Item::StoneShovel:          return 131;
		case Item::StoneSword:           return 131;
		case Item::WoodenAxe:            return 59;
		case Item::WoodenHoe:            return 59;
		case Item::WoodenPickaxe:        return 59;
		case Item::WoodenShovel:         return 59;
		case Item::WoodenSword:          return 59;
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
	return (m_ItemCount >= GetMaxStackSize());
}





char cItem::GetMaxStackSize(void) const
{
	return cItemHandler::For(m_ItemType).GetMaxStackSize();
}





const cItemHandler & cItem::GetHandler(void) const
{
	return cItemHandler::For(m_ItemType);
}





void cItem::GetJson(Json::Value & a_OutValue) const
{
	auto NumericItem = PaletteUpgrade::ToItem(m_ItemType);
	a_OutValue["ID"] = NumericItem.first;
	if (NumericItem.first > 0)
	{
		a_OutValue["Count"] = m_ItemCount;
		a_OutValue["Health"] = m_ItemDamage + NumericItem.second;
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

		if ((m_ItemType == Item::FireworkRocket) || (m_ItemType == Item::FireworkStar))
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
	m_ItemType = PaletteUpgrade::FromItem(static_cast<short>(a_Value.get("ID", -1).asInt()), static_cast<short>(a_Value.get("Health", -1).asInt()));
	if (m_ItemType != Item::Air)
	{
		m_ItemDamage = static_cast<short>(a_Value.get("Health", -1).asInt());
		m_ItemCount = static_cast<char>(a_Value.get("Count", -1).asInt());
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

		if ((m_ItemType == Item::FireworkRocket) || (m_ItemType == Item::FireworkStar))
		{
			m_FireworkItem.m_HasFlicker = a_Value.get("Flicker", false).asBool();
			m_FireworkItem.m_HasTrail = a_Value.get("Trail", false).asBool();
			m_FireworkItem.m_Type = static_cast<unsigned char>(a_Value.get("Type", 0).asInt());
			m_FireworkItem.m_FlightTimeInTicks = static_cast<short>(a_Value.get("FlightTimeInTicks", 0).asInt());
			m_FireworkItem.ColoursFromString(a_Value.get("Colours", "").asString(), m_FireworkItem);
			m_FireworkItem.FadeColoursFromString(a_Value.get("FadeColours", "").asString(), m_FireworkItem);
		}

		m_RepairCost = a_Value.get("RepairCost", 0).asInt();
	}
}





bool cItem::IsEnchantable(Item a_ItemType, bool a_FromBook)
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
		case Item::Book:
		case Item::Bow:
		case Item::FishingRod:
		{
			return true;
		}

		case Item::CarrotOnAStick:
		case Item::Shears:
		case Item::FlintAndSteel:
		{
			return a_FromBook;
		}
		default: return false;
	}
}





unsigned cItem::GetEnchantability()
{
	switch (m_ItemType)
	{
		case Item::WoodenSword:
		case Item::WoodenPickaxe:
		case Item::WoodenShovel:
		case Item::WoodenAxe:
		case Item::WoodenHoe: return 15;

		case Item::LeatherHelmet:
		case Item::LeatherChestplate:
		case Item::LeatherLeggings:
		case Item::LeatherBoots: return 15;

		case Item::StoneSword:
		case Item::StonePickaxe:
		case Item::StoneShovel:
		case Item::StoneAxe:
		case Item::StoneHoe: return 5;

		case Item::IronHelmet:
		case Item::IronChestplate:
		case Item::IronLeggings:
		case Item::IronBoots: return 9;

		case Item::IronSword:
		case Item::IronPickaxe:
		case Item::IronShovel:
		case Item::IronAxe:
		case Item::IronHoe: return 14;

		case Item::ChainmailHelmet:
		case Item::ChainmailChestplate:
		case Item::ChainmailLeggings:
		case Item::ChainmailBoots: return 12;

		case Item::DiamondHelmet:
		case Item::DiamondChestplate:
		case Item::DiamondLeggings:
		case Item::DiamondBoots: return 10;

		case Item::DiamondSword:
		case Item::DiamondPickaxe:
		case Item::DiamondShovel:
		case Item::DiamondAxe:
		case Item::DiamondHoe: return 10;

		case Item::GoldenHelmet:
		case Item::GoldenChestplate:
		case Item::GoldenLeggings:
		case Item::GoldenBoots: return 25;

		case Item::GoldenSword:
		case Item::GoldenPickaxe:
		case Item::GoldenShovel:
		case Item::GoldenAxe:
		case Item::GoldenHoe: return 22;

		case Item::FishingRod:
		case Item::Bow:
		case Item::Book: return 1;
		default: return 0;
	}
}





bool cItem::EnchantByXPLevels(unsigned a_NumXPLevels, MTRand & a_Random)
{
	if (!cItem::IsEnchantable(m_ItemType))
	{
		return false;
	}

	const auto Enchantability = GetEnchantability();
	if (Enchantability == 0)
	{
		return false;
	}

	const auto ModifiedEnchantmentLevel = a_NumXPLevels + a_Random.RandInt(Enchantability / 4) + a_Random.RandInt(Enchantability / 4) + 1;
	const auto RandomBonus = 1.0F + (a_Random.RandReal() + a_Random.RandReal() - 1.0F) * 0.15F;
	const auto FinalEnchantmentLevel = static_cast<unsigned>(ModifiedEnchantmentLevel * RandomBonus + 0.5F);

	cWeightedEnchantments Enchantments;
	cEnchantments::AddItemEnchantmentWeights(Enchantments, m_ItemType, FinalEnchantmentLevel);

	if (m_ItemType == Item::Book)
	{
		m_ItemType = Item::EnchantedBook;
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
	if (m_ItemType == Item::EnchantedBook)
	{
		// Enchanted books can take anything
		return true;
	}

	// The organization here is based on the summary at:
	// https://minecraft.wiki/w/Enchanting
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
	if (m_ItemType == Item::Shears)
	{
		return ShearEnchantments.count(a_EnchantmentID) > 0;
	}
	if (m_ItemType == Item::Bow)
	{
		return BowEnchantments.count(a_EnchantmentID) > 0;
	}
	if (m_ItemType == Item::FishingRod)
	{
		return FishingEnchantments.count(a_EnchantmentID) > 0;
	}
	if (ItemCategory::IsHoe(m_ItemType) || (m_ItemType == Item::FlintAndSteel) || (m_ItemType == Item::CarrotOnAStick) || (m_ItemType == Item::Shield))
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
	bool FromBook = (a_Other.m_ItemType == Item::EnchantedBook);

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





void cItems::Set(int a_Idx, Item a_Item, char a_ItemCount, short a_ItemDamage)
{
	if ((a_Idx < 0) || (a_Idx >= static_cast<int>(size())))
	{
		LOGWARNING("cItems: Attempt to set an item at an out-of-bounds index %d; there are currently %zu items. Not setting.", a_Idx, size());
		return;
	}
	at(static_cast<size_t>(a_Idx)) = cItem(a_Item, a_ItemCount, a_ItemDamage);
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
