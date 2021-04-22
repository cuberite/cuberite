
#include "Globals.h"

#include "EnchantmentSerializer.h"
#include "FastNBT.h"
#include "../Enchantments.h"

void EnchantmentSerializer::WriteToNBTCompound(const cEnchantments & a_Enchantments, cFastNBTWriter & a_Writer, const AString & a_ListTagName)
{
	// Write the enchantments into the specified NBT writer
	// begin with the LIST tag of the specified name ("ench" or "StoredEnchantments")

	a_Writer.BeginList(a_ListTagName, TAG_Compound);
	for (cEnchantments::cMap::const_iterator itr = a_Enchantments.m_Enchantments.begin(), end = a_Enchantments.m_Enchantments.end(); itr != end; ++itr)
	{
		a_Writer.BeginCompound("");
			a_Writer.AddShort("id",  static_cast<Int16>(itr->first));
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
			if (a_NBT.GetType(ch) != TAG_Short)
			{
				continue;
			}
			if (a_NBT.GetName(ch) == "id")
			{
				id = a_NBT.GetShort(ch);
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

