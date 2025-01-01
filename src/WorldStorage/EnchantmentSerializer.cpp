
#include "Globals.h"

#include "EnchantmentSerializer.h"
#include "FastNBT.h"
#include "../Enchantments.h"

void EnchantmentSerializer::WriteToNBTCompound(const cEnchantments & a_Enchantments, cFastNBTWriter & a_Writer, const AString & a_ListTagName, bool stringmode)
{
	// Write the enchantments into the specified NBT writer
	// begin with the LIST tag of the specified name ("ench" or "StoredEnchantments")

	// bool stringmode = true;  //  a_ListTagName != "ench" ? true : false;

	a_Writer.BeginList(a_ListTagName, TAG_Compound);
	for (cEnchantments::cMap::const_iterator itr = a_Enchantments.m_Enchantments.begin(), end = a_Enchantments.m_Enchantments.end(); itr != end; ++itr)
	{
		a_Writer.BeginCompound("");
			if (stringmode)
			{
				a_Writer.AddString("id", GetEnchantmentName(itr->first));
			}
			else
			{
				a_Writer.AddShort("id",  static_cast<Int16>(itr->first));
			}
			a_Writer.AddShort("lvl", static_cast<Int16>(itr->second));
		a_Writer.EndCompound();
	}  // for itr - m_Enchantments[]
	a_Writer.EndList();
}





void EnchantmentSerializer::ParseFromNBT(cEnchantments & a_Enchantments, const cParsedNBT & a_NBT, int a_EnchListTagIdx)
{
	// Read the enchantments from the specified NBT list tag (ench or StoredEnchantments)

	// Verify that the tag is a list:
	if (a_NBT.GetType(a_EnchListTagIdx) != TAG_List)
	{
		LOGWARNING("%s: Invalid EnchListTag type: exp %d, got %d. Enchantments not parsed",
			__FUNCTION__, TAG_List, a_NBT.GetType(a_EnchListTagIdx)
		);
		ASSERT(!"Bad EnchListTag type");
		return;
	}

	// Verify that the list is of Compounds:
	if (a_NBT.GetChildrenType(a_EnchListTagIdx) != TAG_Compound)
	{
		LOGWARNING("%s: Invalid NBT list children type: exp %d, got %d. Enchantments not parsed",
			__FUNCTION__, TAG_Compound, a_NBT.GetChildrenType(a_EnchListTagIdx)
		);
		ASSERT(!"Bad EnchListTag children type");
		return;
	}

	a_Enchantments.Clear();

	// Iterate over all the compound children, parse an enchantment from each:
	for (int tag = a_NBT.GetFirstChild(a_EnchListTagIdx); tag >= 0; tag = a_NBT.GetNextSibling(tag))
	{
		// tag is the compound inside the "ench" list tag
		ASSERT(a_NBT.GetType(tag) == TAG_Compound);

		// Search for the id and lvl tags' values:
		int id = -1, lvl = -1;
		for (int ch = a_NBT.GetFirstChild(tag); ch >= 0; ch = a_NBT.GetNextSibling(ch))
		{
			if ((a_NBT.GetType(ch) != TAG_Short) && (a_NBT.GetType(ch) != TAG_String))
			{
				continue;
			}
			if (a_NBT.GetName(ch) == "id")
			{
				if (a_NBT.GetType(ch) == TAG_Short)
				{
					id = a_NBT.GetShort(ch);
				}
				else
				{
					AString name = a_NBT.GetString(ch);
					name.erase(0, strlen("minecraft:"));
					id = GetEnchantmentID(name);
				}
			}
			else if (a_NBT.GetName(ch) == "lvl")
			{
				lvl = a_NBT.GetShort(ch);
			}
		}  // for ch - children of the compound tag

		if ((id == -1) || (lvl <= 0))
		{
			// Failed to parse either the id or the lvl, skip this compound
			continue;
		}

		// Store the enchantment:
		a_Enchantments.m_Enchantments[id] = static_cast<unsigned int>(lvl);
	}  // for tag - children of the ench list tag
}





int EnchantmentSerializer::GetEnchantmentID(const AString & a_Name)
{
	/** Used by NBT parser to convert string names into numeric ids */
	static const std::map<AString, int> enchantment_names
	{
		{"protection",               0},
		{"fire_protection",          1},
		{"feather_falling",          2},
		{"blast_protection",         3},
		{"projectile_protection",    4},
		{"respiration",              5},
		{"aqua_affinity",            6},
		{"thorns",                   7},
		{"depth_strider",            8},
		{"sharpness",               16},
		{"smite",                   17},
		{"bane_of_arthropods",      18},
		{"knockback",               19},
		{"fire_aspect",             20},
		{"looting",                 21},
		{"efficiency",              32},
		{"silk_touch",              33},
		{"unbreaking",              34},
		{"fortune",                 35},
		{"power",                   48},
		{"punch",                   49},
		{"flame",                   50},
		{"infinity",                51},
		{"luck_of_the_sea",         61},
		{"lure",                    62},
	};
	ASSERT(enchantment_names.count(a_Name) == 1);
	return enchantment_names.at(a_Name);
}





AString EnchantmentSerializer::GetEnchantmentName(int a_ID)
{
	/** Used by NBT parser to convert string names into numeric ids */
	static const std::map<AString, int> enchantment_names
	{
		{"protection",               0},
		{"fire_protection",          1},
		{"feather_falling",          2},
		{"blast_protection",         3},
		{"projectile_protection",    4},
		{"respiration",              5},
		{"aqua_affinity",            6},
		{"thorns",                   7},
		{"depth_strider",            8},
		{"sharpness",               16},
		{"smite",                   17},
		{"bane_of_arthropods",      18},
		{"knockback",               19},
		{"fire_aspect",             20},
		{"looting",                 21},
		{"efficiency",              32},
		{"silk_touch",              33},
		{"unbreaking",              34},
		{"fortune",                 35},
		{"power",                   48},
		{"punch",                   49},
		{"flame",                   50},
		{"infinity",                51},
		{"luck_of_the_sea",         61},
		{"lure",                    62},
	};
	for (const auto & var : enchantment_names)
	{
		if (var.second == a_ID)
		{
			return "minecraft:" + var.first;
		}
	}
	ASSERT(false);
	return "";
}

