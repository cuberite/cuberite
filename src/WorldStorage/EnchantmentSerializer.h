
#pragma once

class cEnchantments;
class cFastNBTWriter;
class cParsedNBT;





namespace EnchantmentSerializer
{

	/** Writes the enchantments into the specified NBT writer; begins with the LIST tag of the specified name ("ench" or "StoredEnchantments") */
	void WriteToNBTCompound(const cEnchantments & a_Enchantments, cFastNBTWriter & a_Writer, const AString & a_ListTagName, bool stringmode);

	/** Reads the enchantments from the specified NBT list tag (ench or StoredEnchantments) */
	void ParseFromNBT(cEnchantments & a_Enchantments, const cParsedNBT & a_NBT, int a_EnchListTagIdx);

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
};




