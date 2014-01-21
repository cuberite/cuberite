// Enchantments.cpp

// Implements the cEnchantments class representing a storage for item enchantments and stored-enchantments

#include "Globals.h"
#include "Enchantments.h"
#include "WorldStorage/FastNBT.h"





cEnchantments::cEnchantments(void)
{
	// Nothing needed yet, but the constructor needs to be declared and impemented in order to be usable
}





cEnchantments::cEnchantments(const AString & a_StringSpec)
{
	AddFromString(a_StringSpec);
}





void cEnchantments::AddFromString(const AString & a_StringSpec)
{
	// Add enchantments in the stringspec; if a specified enchantment already exists, overwrites it
	
	// Split the StringSpec into separate declarations, each in the form "id=lvl":
	AStringVector Decls = StringSplit(a_StringSpec, ";");
	for (AStringVector::const_iterator itr = Decls.begin(), end = Decls.end(); itr != end; ++itr)
	{
		// Split each declaration into the id and lvl part:
		if (itr->empty())
		{
			// The decl is empty (may happen if there's an extra semicolon at the end), ignore silently
			continue;
		}
		AStringVector Split = StringSplitAndTrim(*itr, "=");
		if (Split.size() != 2)
		{
			// Malformed decl
			LOG("%s: Malformed enchantment decl: \"%s\", skipping.", __FUNCTION__, itr->c_str());
			continue;
		}
		int id = atoi(Split[0].c_str());
		if ((id == 0) && (Split[0] != "0"))
		{
			id = StringToEnchantmentID(Split[0]);
		}
		int lvl = atoi(Split[1].c_str());
		if (
			((id  <= 0) && (Split[0] != "0")) ||
			((lvl == 0) && (Split[1] != "0"))
		)
		{
			// Numbers failed to parse
			LOG("%s: Failed to parse enchantment declaration for numbers: \"%s\" and \"%s\", skipping.",
				__FUNCTION__, Split[0].c_str(), Split[1].c_str()
			);
			continue;
		}
		SetLevel(id, lvl);
	}  // for itr - Decls[]
}





AString cEnchantments::ToString(void) const
{
	// Serialize all the enchantments into a string
	AString res;
	for (cEnchantments::cMap::const_iterator itr = m_Enchantments.begin(), end = m_Enchantments.end(); itr != end; ++itr)
	{
		AppendPrintf(res, "%d=%d;", itr->first, itr->second);
	}  // for itr - m_Enchantments[]
	return res;
}





int cEnchantments::GetLevel(int a_EnchantmentID) const
{
	// Return the level for the specified enchantment; 0 if not stored
	cMap::const_iterator itr = m_Enchantments.find(a_EnchantmentID);
	if (itr != m_Enchantments.end())
	{
		return itr->second;
	}

	// Not stored, return zero
	return 0;
}





void cEnchantments::SetLevel(int a_EnchantmentID, int a_Level)
{
	// Sets the level for the specified enchantment, adding it if not stored before or removing it if level <= 0
	if (a_Level == 0)
	{
		// Delete enchantment, if present:
		cMap::iterator itr = m_Enchantments.find(a_EnchantmentID);
		if (itr != m_Enchantments.end())
		{
			m_Enchantments.erase(itr);
		}
	}
	else
	{
		// Add / overwrite enchantment
		m_Enchantments[a_EnchantmentID] = a_Level;
	}
}






void cEnchantments::Clear(void)
{
	m_Enchantments.clear();
}





bool cEnchantments::IsEmpty(void) const
{
	return m_Enchantments.empty();
}





int cEnchantments::StringToEnchantmentID(const AString & a_EnchantmentName)
{
	struct
	{
		int m_Value;
		const char * m_Name;
	} EnchantmentNames[] =
	{
		{ enchProtection,           "Protection"},
		{ enchFireProtection,       "FireProtection"},
		{ enchFeatherFalling,       "FeatherFalling"},
		{ enchBlastProtection,      "BlastProtection"},
		{ enchProjectileProtection, "ProjectileProtection"},
		{ enchRespiration,          "Respiration"},
		{ enchAquaAffinity,         "AquaAffinity"},
		{ enchThorns,               "Thorns"},
		{ enchSharpness,            "Sharpness"},
		{ enchSmite,                "Smite"},
		{ enchBaneOfArthropods,     "BaneOfArthropods"},
		{ enchKnockback,            "Knockback"},
		{ enchFireAspect,           "FireAspect"},
		{ enchLooting,              "Looting"},
		{ enchEfficiency,           "Efficiency"},
		{ enchSilkTouch,            "SilkTouch"},
		{ enchUnbreaking,           "Unbreaking"},
		{ enchFortune,              "Fortune"},
		{ enchPower,                "Power"},
		{ enchPunch,                "Punch"},
		{ enchFlame,                "Flame"},
		{ enchInfinity,             "Infinity"},
		{ enchLuckOfTheSea,         "LuckOfTheSea"},
		{ enchLure,                 "Lure"},
	} ;
	for (size_t i = 0; i < ARRAYCOUNT(EnchantmentNames); i++)
	{
		if (NoCaseCompare(EnchantmentNames[i].m_Name, a_EnchantmentName) == 0)
		{
			return EnchantmentNames[i].m_Value;
		}
	}  // for i - EnchantmentNames[]
	return -1;
}





bool cEnchantments::operator ==(const cEnchantments & a_Other) const
{
	return m_Enchantments == a_Other.m_Enchantments;
}





bool cEnchantments::operator !=(const cEnchantments & a_Other) const
{
	return m_Enchantments != a_Other.m_Enchantments;
}










