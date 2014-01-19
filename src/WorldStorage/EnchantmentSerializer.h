
#pragma once

#include "Enchantments.h"

class cEnchantmentSerializer
{

public:

	/// Writes the enchantments into the specified NBT writer; begins with the LIST tag of the specified name ("ench" or "StoredEnchantments")
	static void WriteToNBTCompound(cEnchantments const& a_Enchantments, cFastNBTWriter & a_Writer, const AString & a_ListTagName);
	
	/// Reads the enchantments from the specified NBT list tag (ench or StoredEnchantments)
	static void ParseFromNBT(cEnchantments& a_Enchantments, const cParsedNBT & a_NBT, int a_EnchListTagIdx);
	
};
