// Enchantments.cpp

// Implements the cEnchantments class representing a storage for item enchantments and stored-enchantments

#include "Globals.h"
#include "Enchantments.h"
#include "WorldStorage/FastNBT.h"
#include "FastRandom.h"
#include "Noise/Noise.h"
#include "BlockType.h"





cEnchantments::cEnchantments(void)
{
	// Nothing needed yet, but the constructor needs to be declared and impemented in order to be usable
}





cEnchantments::cEnchantments(const AString & a_StringSpec)
{
	AddFromString(a_StringSpec);
}





void cEnchantments::Add(const cEnchantments & a_Other)
{
	for (cEnchantments::cMap::const_iterator itr = a_Other.m_Enchantments.begin(), end = a_Other.m_Enchantments.end(); itr != end; ++itr)
	{
		SetLevel(itr->first, itr->second);
	}  // for itr - a_Other.m_Enchantments[]
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
		int id = StringToEnchantmentID(Split[0]);
		if (id < 0)
		{
			LOG("%s: Failed to parse enchantment \"%s\", skipping.", __FUNCTION__, Split[0].c_str());
			continue;
		}
		unsigned int lvl;
		if (!StringToInteger(Split[1], lvl))
		{
			// Level failed to parse
			LOG("%s: Failed to parse enchantment level \"%s\", skipping.", __FUNCTION__, Split[1].c_str());
			continue;
		}
		SetLevel(id, lvl);
	}  // for itr - Decls[]
}





size_t cEnchantments::Count(void) const
{
	return m_Enchantments.size();
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





unsigned int cEnchantments::GetLevel(int a_EnchantmentID) const
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





void cEnchantments::SetLevel(int a_EnchantmentID, unsigned int a_Level)
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





unsigned int cEnchantments::GetLevelCap(int a_EnchantmentID)
{
	switch (a_EnchantmentID)
	{
		case enchProtection:           return 4;
		case enchFireProtection:       return 4;
		case enchFeatherFalling:       return 4;
		case enchBlastProtection:      return 4;
		case enchProjectileProtection: return 4;
		case enchRespiration:          return 3;
		case enchAquaAffinity:         return 1;
		case enchThorns:               return 3;
		case enchDepthStrider:         return 3;
		case enchSharpness:            return 5;
		case enchSmite:                return 5;
		case enchBaneOfArthropods:     return 5;
		case enchKnockback:            return 2;
		case enchFireAspect:           return 2;
		case enchLooting:              return 3;
		case enchEfficiency:           return 5;
		case enchSilkTouch:            return 1;
		case enchUnbreaking:           return 3;
		case enchFortune:              return 3;
		case enchPower:                return 5;
		case enchPunch:                return 2;
		case enchFlame:                return 1;
		case enchInfinity:             return 1;
		case enchLuckOfTheSea:         return 3;
		case enchLure:                 return 3;
	}
	LOGWARNING("Unknown enchantment ID %d", a_EnchantmentID);
	return 0;
}





int cEnchantments::GetXPCostMultiplier(int a_EnchantmentID, bool FromBook)
{
	if (FromBook)
	{
		switch (a_EnchantmentID)
		{
			case enchProtection:           return 1;
			case enchFireProtection:       return 1;
			case enchFeatherFalling:       return 1;
			case enchBlastProtection:      return 2;
			case enchProjectileProtection: return 1;
			case enchRespiration:          return 2;
			case enchAquaAffinity:         return 2;
			case enchThorns:               return 4;
			case enchDepthStrider:         return 2;
			case enchSharpness:            return 1;
			case enchSmite:                return 1;
			case enchBaneOfArthropods:     return 1;
			case enchKnockback:            return 1;
			case enchFireAspect:           return 2;
			case enchLooting:              return 2;
			case enchEfficiency:           return 1;
			case enchSilkTouch:            return 4;
			case enchUnbreaking:           return 1;
			case enchFortune:              return 1;
			case enchPower:                return 1;
			case enchPunch:                return 2;
			case enchFlame:                return 2;
			case enchInfinity:             return 4;
			case enchLuckOfTheSea:         return 2;
			case enchLure:                 return 2;
		}
	}
	else  // Without book
	{
		switch (a_EnchantmentID)
		{
			case enchProtection:           return 1;
			case enchFireProtection:       return 2;
			case enchFeatherFalling:       return 2;
			case enchBlastProtection:      return 4;
			case enchProjectileProtection: return 2;
			case enchRespiration:          return 4;
			case enchAquaAffinity:         return 4;
			case enchThorns:               return 8;
			case enchDepthStrider:         return 4;

			case enchSharpness:            return 1;
			case enchSmite:                return 2;
			case enchBaneOfArthropods:     return 2;
			case enchKnockback:            return 2;
			case enchFireAspect:           return 4;
			case enchLooting:              return 4;

			case enchEfficiency:           return 1;
			case enchSilkTouch:            return 8;
			case enchUnbreaking:           return 2;
			case enchFortune:              return 4;
			case enchPower:                return 1;
			case enchPunch:                return 4;
			case enchFlame:                return 4;
			case enchInfinity:             return 8;
			case enchLuckOfTheSea:         return 4;
			case enchLure:                 return 4;
		}
	}
	LOGWARNING("Unknown enchantment ID %d", a_EnchantmentID);
	return 0;
}





bool cEnchantments::CanAddEnchantment(int a_EnchantmentID) const
{
	if (GetLevel(a_EnchantmentID) > 0)
	{
		return true;
	}

	static const std::vector<std::set<int> > IncompatibleEnchantments =
	{
		// Armor
		{ enchProtection, enchFireProtection, enchBlastProtection, enchProjectileProtection },

		// Tool
		{ enchFortune, enchSilkTouch },

		// Sword
		{ enchSharpness, enchSmite, enchBaneOfArthropods },

		// Boots
		// {enchDepthStrider, enchFrostWalker},

		// Bow
		// {enchInfinity, enchMending}
	};

	for (const auto & excl: IncompatibleEnchantments)
	{
		if (excl.count(a_EnchantmentID) != 0)
		{
			// See if we also have any of the enchantments
			for (auto ench: excl)
			{
				if (GetLevel(ench) > 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}





int cEnchantments::StringToEnchantmentID(const AString & a_EnchantmentName)
{
	static const struct
	{
		int m_Value;
		const char * m_Name;
	} EnchantmentNames[] =
	{
		{ enchProtection,           "Protection" },
		{ enchFireProtection,       "FireProtection" },
		{ enchFeatherFalling,       "FeatherFalling" },
		{ enchBlastProtection,      "BlastProtection" },
		{ enchProjectileProtection, "ProjectileProtection" },
		{ enchRespiration,          "Respiration" },
		{ enchAquaAffinity,         "AquaAffinity" },
		{ enchThorns,               "Thorns" },
		{ enchDepthStrider,         "DepthStrider" },
		{ enchSharpness,            "Sharpness" },
		{ enchSmite,                "Smite" },
		{ enchBaneOfArthropods,     "BaneOfArthropods" },
		{ enchKnockback,            "Knockback" },
		{ enchFireAspect,           "FireAspect" },
		{ enchLooting,              "Looting" },
		{ enchEfficiency,           "Efficiency" },
		{ enchSilkTouch,            "SilkTouch" },
		{ enchUnbreaking,           "Unbreaking" },
		{ enchFortune,              "Fortune" },
		{ enchPower,                "Power" },
		{ enchPunch,                "Punch" },
		{ enchFlame,                "Flame" },
		{ enchInfinity,             "Infinity" },
		{ enchLuckOfTheSea,         "LuckOfTheSea" },
		{ enchLure,                 "Lure" },
	} ;

	// First try to parse as a number:
	int id = atoi(a_EnchantmentName.c_str());
	if ((id != 0) || (a_EnchantmentName == "0"))
	{
		return id;
	}

	// It wasn't a number, do a lookup:
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





void cEnchantments::AddItemEnchantmentWeights(cWeightedEnchantments & a_Enchantments, short a_ItemType, int a_EnchantmentLevel)
{
	if (ItemCategory::IsSword(a_ItemType))
	{
		// Sharpness
		if ((a_EnchantmentLevel >= 34) && (a_EnchantmentLevel <= 54))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchSharpness, 4);
		}
		else if ((a_EnchantmentLevel >= 23) && (a_EnchantmentLevel <= 43))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchSharpness, 3);
		}
		else if ((a_EnchantmentLevel >= 12) && (a_EnchantmentLevel <= 32))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchSharpness, 2);
		}
		else if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 21))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchSharpness, 1);
		}

		// Smite
		if ((a_EnchantmentLevel >= 29) && (a_EnchantmentLevel <= 49))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchSmite, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 41))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchSmite, 3);
		}
		else if ((a_EnchantmentLevel >= 13) && (a_EnchantmentLevel <= 33))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchSmite, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 25))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchSmite, 1);
		}

		// Bane of Arthropods
		if ((a_EnchantmentLevel >= 29) && (a_EnchantmentLevel <= 49))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchBaneOfArthropods, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 41))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchBaneOfArthropods, 3);
		}
		else if ((a_EnchantmentLevel >= 13) && (a_EnchantmentLevel <= 33))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchBaneOfArthropods, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 25))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchBaneOfArthropods, 1);
		}

		// Knockback
		if ((a_EnchantmentLevel >= 25) && (a_EnchantmentLevel <= 75))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchKnockback, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 55))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchKnockback, 1);
		}

		// Fire Aspect
		if ((a_EnchantmentLevel >= 30) && (a_EnchantmentLevel <= 80))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFireAspect, 2);
		}
		else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 60))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFireAspect, 1);
		}

		// Looting
		if ((a_EnchantmentLevel >= 33) && (a_EnchantmentLevel <= 83))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchLooting, 3);
		}
		else if ((a_EnchantmentLevel >= 24) && (a_EnchantmentLevel <= 74))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchLooting, 2);
		}
		else if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 65))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchLooting, 1);
		}
	}

	else if (ItemCategory::IsTool(a_ItemType))
	{
		// Efficiency
		if ((a_EnchantmentLevel >= 31) && (a_EnchantmentLevel <= 81))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchEfficiency, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 71))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchEfficiency, 3);
		}
		else if ((a_EnchantmentLevel >= 11) && (a_EnchantmentLevel <= 61))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchEfficiency, 2);
		}
		else if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 51))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchEfficiency, 1);
		}

		// Silk Touch
		if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 65))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchSilkTouch, 1);
		}

		// Fortune
		if ((a_EnchantmentLevel >= 33) && (a_EnchantmentLevel <= 83))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFortune, 3);
		}
		else if ((a_EnchantmentLevel >= 24) && (a_EnchantmentLevel <= 74))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFortune, 2);
		}
		else if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 65))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFortune, 1);
		}
	}

	else if (ItemCategory::IsArmor(a_ItemType))
	{
		// Protection
		if ((a_EnchantmentLevel >= 34) && (a_EnchantmentLevel <= 54))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchProtection, 4);
		}
		else if ((a_EnchantmentLevel >= 23) && (a_EnchantmentLevel <= 43))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchProtection, 3);
		}
		else if ((a_EnchantmentLevel >= 12) && (a_EnchantmentLevel <= 32))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchProtection, 2);
		}
		else if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 21))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchProtection, 1);
		}

		// Fire Protection
		if ((a_EnchantmentLevel >= 34) && (a_EnchantmentLevel <= 46))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFireProtection, 4);
		}
		else if ((a_EnchantmentLevel >= 26) && (a_EnchantmentLevel <= 38))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFireProtection, 3);
		}
		else if ((a_EnchantmentLevel >= 18) && (a_EnchantmentLevel <= 30))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFireProtection, 2);
		}
		else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 22))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFireProtection, 1);
		}

		// Blast Protection
		if ((a_EnchantmentLevel >= 29) && (a_EnchantmentLevel <= 41))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchBlastProtection, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 33))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchBlastProtection, 3);
		}
		else if ((a_EnchantmentLevel >= 13) && (a_EnchantmentLevel <= 25))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchBlastProtection, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 17))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchBlastProtection, 1);
		}

		// Projectile Protection
		if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 36))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchProjectileProtection, 4);
		}
		else if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 30))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchProjectileProtection, 3);
		}
		else if ((a_EnchantmentLevel >= 9) && (a_EnchantmentLevel <= 24))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchProjectileProtection, 2);
		}
		else if ((a_EnchantmentLevel >= 3) && (a_EnchantmentLevel <= 18))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchProjectileProtection, 1);
		}

		// Thorns
		if ((a_EnchantmentLevel >= 50) && (a_EnchantmentLevel <= 100))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchThorns, 3);
		}
		else if ((a_EnchantmentLevel >= 30) && (a_EnchantmentLevel <= 80))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchThorns, 2);
		}
		else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 60))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchThorns, 1);
		}


		if (ItemCategory::IsHelmet(a_ItemType))
		{
			// Respiration
			if ((a_EnchantmentLevel >= 30) && (a_EnchantmentLevel <= 60))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 2, enchRespiration, 3);
			}
			else if ((a_EnchantmentLevel >= 20) && (a_EnchantmentLevel <= 50))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 2, enchRespiration, 2);
			}
			else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 40))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 2, enchRespiration, 1);
			}

			// Aqua Affinity
			if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 41))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 2, enchAquaAffinity, 1);
			}
		}

		else if (ItemCategory::IsBoots(a_ItemType))
		{
			// Feather Fall
			if ((a_EnchantmentLevel >= 23) && (a_EnchantmentLevel <= 33))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 5, enchFeatherFalling, 4);
			}
			else if ((a_EnchantmentLevel >= 17) && (a_EnchantmentLevel <= 27))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 5, enchFeatherFalling, 3);
			}
			else if ((a_EnchantmentLevel >= 11) && (a_EnchantmentLevel <= 21))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 5, enchFeatherFalling, 2);
			}
			else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 15))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 5, enchFeatherFalling, 1);
			}

			// Depth Strider
			if ((a_EnchantmentLevel >= 30) && (a_EnchantmentLevel <= 45))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 2, enchDepthStrider, 3);
			}
			else if ((a_EnchantmentLevel >= 20) && (a_EnchantmentLevel <= 35))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 2, enchDepthStrider, 2);
			}
			else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 25))
			{
				AddEnchantmentWeightToVector(a_Enchantments, 2, enchDepthStrider, 1);
			}
		}
	}

	else if (a_ItemType == E_ITEM_BOW)
	{
		// Power
		if ((a_EnchantmentLevel >= 31) && (a_EnchantmentLevel <= 46))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchPower, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 36))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchPower, 3);
		}
		else if ((a_EnchantmentLevel >= 11) && (a_EnchantmentLevel <= 26))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchPower, 2);
		}
		else if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 16))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchPower, 1);
		}

		// Punch
		if ((a_EnchantmentLevel >= 32) && (a_EnchantmentLevel <= 57))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchPunch, 2);
		}
		else if ((a_EnchantmentLevel >= 12) && (a_EnchantmentLevel <= 37))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchPunch, 1);
		}

		// Flame and Infinity
		if ((a_EnchantmentLevel >= 20) && (a_EnchantmentLevel <= 50))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFlame, 1);
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchInfinity, 1);
		}
	}

	else if (a_ItemType == E_ITEM_FISHING_ROD)
	{
		// Luck of the Sea and Lure
		if ((a_EnchantmentLevel >= 33) && (a_EnchantmentLevel <= 83))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLuckOfTheSea, 3);
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLure, 3);
		}
		else if ((a_EnchantmentLevel >= 24) && (a_EnchantmentLevel <= 74))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLuckOfTheSea, 2);
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLure, 2);
		}
		else if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 65))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLuckOfTheSea, 1);
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLure, 1);
		}
	}

	else if (a_ItemType == E_ITEM_BOOK)
	{
		// All Enchantments

		// Sharpness
		if ((a_EnchantmentLevel >= 34) && (a_EnchantmentLevel <= 54))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchSharpness, 4);
		}
		else if ((a_EnchantmentLevel >= 23) && (a_EnchantmentLevel <= 43))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchSharpness, 3);
		}
		else if ((a_EnchantmentLevel >= 12) && (a_EnchantmentLevel <= 32))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchSharpness, 2);
		}
		else if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 21))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchSharpness, 1);
		}

		// Smite
		if ((a_EnchantmentLevel >= 29) && (a_EnchantmentLevel <= 49))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchSmite, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 41))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchSmite, 3);
		}
		else if ((a_EnchantmentLevel >= 13) && (a_EnchantmentLevel <= 33))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchSmite, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 25))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchSmite, 1);
		}

		// Bane of Arthropods
		if ((a_EnchantmentLevel >= 29) && (a_EnchantmentLevel <= 49))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchBaneOfArthropods, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 41))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchBaneOfArthropods, 3);
		}
		else if ((a_EnchantmentLevel >= 13) && (a_EnchantmentLevel <= 33))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchBaneOfArthropods, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 25))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchBaneOfArthropods, 1);
		}

		// Knockback
		if ((a_EnchantmentLevel >= 25) && (a_EnchantmentLevel <= 75))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchKnockback, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 55))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchKnockback, 1);
		}

		// Fire Aspect
		if ((a_EnchantmentLevel >= 30) && (a_EnchantmentLevel <= 80))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFireAspect, 2);
		}
		else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 60))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFireAspect, 1);
		}

		// Looting
		if ((a_EnchantmentLevel >= 33) && (a_EnchantmentLevel <= 83))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchLooting, 3);
		}
		else if ((a_EnchantmentLevel >= 24) && (a_EnchantmentLevel <= 74))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchLooting, 2);
		}
		else if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 65))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchLooting, 1);
		}

		// Efficiency
		if ((a_EnchantmentLevel >= 31) && (a_EnchantmentLevel <= 81))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchEfficiency, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 71))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchEfficiency, 3);
		}
		else if ((a_EnchantmentLevel >= 11) && (a_EnchantmentLevel <= 61))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchEfficiency, 2);
		}
		else if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 51))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchEfficiency, 1);
		}

		// Silk Touch
		if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 65))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchSilkTouch, 1);
		}

		// Fortune
		if ((a_EnchantmentLevel >= 33) && (a_EnchantmentLevel <= 83))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFortune, 3);
		}
		else if ((a_EnchantmentLevel >= 24) && (a_EnchantmentLevel <= 74))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFortune, 2);
		}
		else if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 65))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFortune, 1);
		}

		// Protection
		if ((a_EnchantmentLevel >= 34) && (a_EnchantmentLevel <= 54))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchProtection, 4);
		}
		else if ((a_EnchantmentLevel >= 23) && (a_EnchantmentLevel <= 43))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchProtection, 3);
		}
		else if ((a_EnchantmentLevel >= 12) && (a_EnchantmentLevel <= 32))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchProtection, 2);
		}
		else if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 21))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchProtection, 1);
		}

		// Fire Protection
		if ((a_EnchantmentLevel >= 34) && (a_EnchantmentLevel <= 46))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFireProtection, 4);
		}
		else if ((a_EnchantmentLevel >= 26) && (a_EnchantmentLevel <= 38))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFireProtection, 3);
		}
		else if ((a_EnchantmentLevel >= 18) && (a_EnchantmentLevel <= 30))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFireProtection, 2);
		}
		else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 22))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFireProtection, 1);
		}

		// Blast Protection
		if ((a_EnchantmentLevel >= 29) && (a_EnchantmentLevel <= 41))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchBlastProtection, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 33))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchBlastProtection, 3);
		}
		else if ((a_EnchantmentLevel >= 13) && (a_EnchantmentLevel <= 25))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchBlastProtection, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 17))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchBlastProtection, 1);
		}

		// Projectile Protection
		if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 36))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchProjectileProtection, 4);
		}
		else if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 30))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchProjectileProtection, 3);
		}
		else if ((a_EnchantmentLevel >= 9) && (a_EnchantmentLevel <= 24))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchProjectileProtection, 2);
		}
		else if ((a_EnchantmentLevel >= 3) && (a_EnchantmentLevel <= 18))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchProjectileProtection, 1);
		}

		// Thorns
		if ((a_EnchantmentLevel >= 50) && (a_EnchantmentLevel <= 100))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchThorns, 3);
		}
		else if ((a_EnchantmentLevel >= 30) && (a_EnchantmentLevel <= 80))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchThorns, 2);
		}
		else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 60))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchThorns, 1);
		}

		// Respiration
		if ((a_EnchantmentLevel >= 30) && (a_EnchantmentLevel <= 60))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchRespiration, 3);
		}
		else if ((a_EnchantmentLevel >= 20) && (a_EnchantmentLevel <= 50))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchRespiration, 2);
		}
		else if ((a_EnchantmentLevel >= 10) && (a_EnchantmentLevel <= 40))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchRespiration, 1);
		}

		// Aqua Affinity
		if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 41))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchAquaAffinity, 1);
		}

		// Feather Fall
		if ((a_EnchantmentLevel >= 23) && (a_EnchantmentLevel <= 33))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFeatherFalling, 4);
		}
		else if ((a_EnchantmentLevel >= 17) && (a_EnchantmentLevel <= 27))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFeatherFalling, 3);
		}
		else if ((a_EnchantmentLevel >= 11) && (a_EnchantmentLevel <= 21))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFeatherFalling, 2);
		}
		else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 15))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 5, enchFeatherFalling, 1);
		}

		// Power
		if ((a_EnchantmentLevel >= 31) && (a_EnchantmentLevel <= 46))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchPower, 4);
		}
		else if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 36))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchPower, 3);
		}
		else if ((a_EnchantmentLevel >= 11) && (a_EnchantmentLevel <= 26))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchPower, 2);
		}
		else if ((a_EnchantmentLevel >= 1) && (a_EnchantmentLevel <= 16))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 10, enchPower, 1);
		}

		// Punch
		if ((a_EnchantmentLevel >= 32) && (a_EnchantmentLevel <= 57))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchPunch, 2);
		}
		else if ((a_EnchantmentLevel >= 12) && (a_EnchantmentLevel <= 37))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchPunch, 1);
		}

		// Flame and Infinity
		if ((a_EnchantmentLevel >= 20) && (a_EnchantmentLevel <= 50))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 2, enchFlame, 1);
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchInfinity, 1);
		}

		// Luck of the Sea and Lure
		if ((a_EnchantmentLevel >= 33) && (a_EnchantmentLevel <= 83))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLuckOfTheSea, 3);
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLure, 3);
		}
		else if ((a_EnchantmentLevel >= 24) && (a_EnchantmentLevel <= 74))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLuckOfTheSea, 2);
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLure, 2);
		}
		else if ((a_EnchantmentLevel >= 15) && (a_EnchantmentLevel <= 65))
		{
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLuckOfTheSea, 1);
			AddEnchantmentWeightToVector(a_Enchantments, 1, enchLure, 1);
		}
	}

	// Unbreaking
	if ((a_EnchantmentLevel >= 21) && (a_EnchantmentLevel <= 71))
	{
		AddEnchantmentWeightToVector(a_Enchantments, 5, enchUnbreaking, 3);
	}
	else if ((a_EnchantmentLevel >= 13) && (a_EnchantmentLevel <= 63))
	{
		AddEnchantmentWeightToVector(a_Enchantments, 5, enchUnbreaking, 2);
	}
	else if ((a_EnchantmentLevel >= 5) && (a_EnchantmentLevel <= 55))
	{
		AddEnchantmentWeightToVector(a_Enchantments, 5, enchUnbreaking, 1);
	}
}





void cEnchantments::AddEnchantmentWeightToVector(cWeightedEnchantments & a_Enchantments, int a_Weight, int a_EnchantmentID, unsigned int a_EnchantmentLevel)
{
	cWeightedEnchantment weightedenchantment;
	weightedenchantment.m_Weight = a_Weight;
	cEnchantments enchantment;
	enchantment.SetLevel(a_EnchantmentID, a_EnchantmentLevel);
	weightedenchantment.m_Enchantments = enchantment;
	a_Enchantments.push_back(weightedenchantment);
}





void cEnchantments::RemoveEnchantmentWeightFromVector(cWeightedEnchantments & a_Enchantments, int a_EnchantmentID)
{
	for (cWeightedEnchantments::iterator it = a_Enchantments.begin(); it != a_Enchantments.end(); ++it)
	{
		if ((*it).m_Enchantments.GetLevel(a_EnchantmentID) > 0)
		{
			a_Enchantments.erase(it);
			break;
		}
	}
}





void cEnchantments::RemoveEnchantmentWeightFromVector(cWeightedEnchantments & a_Enchantments, const cEnchantments & a_Enchantment)
{
	for (cWeightedEnchantments::iterator it = a_Enchantments.begin(); it != a_Enchantments.end(); ++it)
	{
		if ((*it).m_Enchantments == a_Enchantment)
		{
			a_Enchantments.erase(it);
			break;
		}
	}
}





void cEnchantments::CheckEnchantmentConflictsFromVector(
	cWeightedEnchantments & a_Enchantments, const cEnchantments & a_FirstEnchantment
)
{
	if (a_FirstEnchantment.GetLevel(cEnchantments::enchProtection) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchFireProtection);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchBlastProtection);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchProjectileProtection);
	}
	else if (a_FirstEnchantment.GetLevel(cEnchantments::enchFireProtection) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchProtection);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchBlastProtection);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchProjectileProtection);
	}
	else if (a_FirstEnchantment.GetLevel(cEnchantments::enchBlastProtection) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchProtection);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchFireProtection);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchProjectileProtection);
	}
	else if (a_FirstEnchantment.GetLevel(cEnchantments::enchProjectileProtection) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchProtection);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchFireProtection);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchBlastProtection);
	}

	else if (a_FirstEnchantment.GetLevel(cEnchantments::enchSharpness) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchSmite);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchBaneOfArthropods);
	}
	else if (a_FirstEnchantment.GetLevel(cEnchantments::enchSmite) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchSharpness);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchBaneOfArthropods);
	}
	else if (a_FirstEnchantment.GetLevel(cEnchantments::enchBaneOfArthropods) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchSharpness);
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchSmite);
	}
	else if (a_FirstEnchantment.GetLevel(cEnchantments::enchSilkTouch) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchFortune);
	}
	else if (a_FirstEnchantment.GetLevel(cEnchantments::enchFortune) > 0)
	{
		RemoveEnchantmentWeightFromVector(a_Enchantments, cEnchantments::enchSilkTouch);
	}
}





cEnchantments cEnchantments::GetRandomEnchantmentFromVector(const cWeightedEnchantments & a_Enchantments, MTRand & a_Random)
{
	int AllWeights = 0;
	for (const auto & Enchantment: a_Enchantments)
	{
		AllWeights += Enchantment.m_Weight;
	}
	int RandomNumber = a_Random.RandInt(AllWeights - 1);
	for (const auto & Enchantment: a_Enchantments)
	{
		RandomNumber -= Enchantment.m_Weight;
		if (RandomNumber < 0)
		{
			return Enchantment.m_Enchantments;
		}
	}

	return cEnchantments();
}





cEnchantments cEnchantments::SelectEnchantmentFromVector(const cWeightedEnchantments & a_Enchantments, int a_Seed)
{
	// Sum up all the enchantments' weights:
	int AllWeights = 0;
	for (const auto & Enchantment : a_Enchantments)
	{
		AllWeights += Enchantment.m_Weight;
	}

	// If there's no weight for any of the enchantments, return an empty enchantment
	if (AllWeights <= 0)
	{
		return cEnchantments();
	}

	// Pick a random enchantment:
	cNoise Noise(a_Seed);
	int RandomNumber = Noise.IntNoise1DInt(AllWeights) / 7 % AllWeights;
	for (const auto & Enchantment : a_Enchantments)
	{
		RandomNumber -= Enchantment.m_Weight;
		if (RandomNumber <= 0)
		{
			return Enchantment.m_Enchantments;
		}
	}

	// No enchantment picked, return an empty one (we probably shouldn't ever get here):
	return cEnchantments();
}




