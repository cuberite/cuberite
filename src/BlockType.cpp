// BlockType.cpp

// Implements the helper functions for converting Block Type string to int etc.


#include "Globals.h"
#include "IniFile.h"
#include "Item.h"
#include "Mobs/Monster.h"




class cBlockIDMap
{
	// Making the map case-insensitive:
	struct Comparator
	{
		bool operator ()(const AString & a_Item1, const AString & a_Item2) const
		{
			return (NoCaseCompare(a_Item1, a_Item2) > 0);
		}
	} ;

	using ItemMap = std::map<AString, std::pair<short, short>, Comparator>;

public:

	cBlockIDMap(void)
	{
		cIniFile Ini;
		if (!Ini.ReadFile("items.ini"))
		{
			return;
		}
		int KeyID = Ini.FindKey("Items");
		if (KeyID == cIniFile::noID)
		{
			return;
		}
		int NumValues = Ini.GetNumValues(KeyID);
		for (int i = 0; i < NumValues; i++)
		{
			AString Name = Ini.GetValueName(KeyID, i);
			if (Name.empty())
			{
				continue;
			}
			AString Value = Ini.GetValue(KeyID, i);
			AddToMap(Name, Value);
		}  // for i - Ini.Values[]
	}


	int Resolve(const AString & a_ItemName)
	{
		ItemMap::iterator itr = m_Map.find(a_ItemName);
		if (itr == m_Map.end())
		{
			return -1;
		}
		return itr->second.first;
	}


	AString Desolve(short a_ItemType, short a_ItemDamage)
	{
		// First try an exact match, both ItemType and ItemDamage ("birchplanks=5:2"):
		for (ItemMap::iterator itr = m_Map.begin(), end = m_Map.end(); itr != end; ++itr)
		{
			if ((itr->second.first == a_ItemType) && (itr->second.second == a_ItemDamage))
			{
				return itr->first;
			}
		}  // for itr - m_Map[]

		// There is no exact match, try matching ItemType only ("planks=5"):
		if (a_ItemDamage == 0)
		{
			for (ItemMap::iterator itr = m_Map.begin(), end = m_Map.end(); itr != end; ++itr)
			{
				if ((itr->second.first == a_ItemType) && (itr->second.second == -1))
				{
					return itr->first;
				}
			}  // for itr - m_Map[]
		}

		// No match at all, synthesize a string ("5:1"):
		AString res;
		if (a_ItemDamage == -1)
		{
			res = fmt::format(FMT_STRING("{}"), a_ItemType);
		}
		else
		{
			res = fmt::format(FMT_STRING("{}:{}"), a_ItemType, a_ItemDamage);
		}
		return res;
	}


protected:
	ItemMap m_Map;


	void AddToMap(const AString & a_Name, const AString & a_Value)
	{
		AStringVector Split = StringSplit(a_Value, ":");
		if (Split.size() == 1)
		{
			Split = StringSplit(a_Value, "^");
		}
		if (Split.empty())
		{
			return;
		}
		short ItemType;
		if (!StringToInteger(Split[0], ItemType))
		{
			ASSERT(!"Invalid item type");
		}
		short ItemDamage = -1;
		if (Split.size() > 1 && !StringToInteger(Split[1], ItemDamage))
		{
			ASSERT(!"Invalid item damage");
		}
		m_Map[a_Name] = std::make_pair(ItemType, ItemDamage);
	}
} ;




static cBlockIDMap & GetBlockIDMap()
{
	static cBlockIDMap IDMap;
	return IDMap;
}





/*
// Quick self-test:
class Tester
{
public:
	Tester(void)
	{
		cItem Item;
		gsBlockIDMap.ResolveItem("charcoal", Item);
		AString Charcoal = gsBlockIDMap.Desolve(Item.m_ItemType, Item.m_ItemDamage);
		ASSERT(Charcoal == "charcoal");
	}
} test;
//*/





int BlockStringToType(const AString & a_BlockTypeString)
{
	int res = atoi(a_BlockTypeString.c_str());
	if ((res != 0) || (a_BlockTypeString.compare("0") == 0))
	{
		// It was a valid number, return that
		return res;
	}

	return GetBlockIDMap().Resolve(TrimString(a_BlockTypeString));
}





bool StringToItem(const AString & a_ItemTypeString, cItem & a_Item)
{
	AString ItemName = TrimString(a_ItemTypeString);
	a_Item = cItem(NamespaceSerializer::ToItem(NamespaceSerializer::SplitNamespacedID(ItemName).second));
	return true;
}





AString ItemToString(const cItem & a_Item)
{
	return AString(NamespaceSerializer::From(a_Item.m_ItemType));
}





AString ItemTypeToString(short a_ItemType)
{
	return AString(NamespaceSerializer::From(PaletteUpgrade::FromItem(a_ItemType, 0)));
}





AString ItemToFullString(const cItem & a_Item)
{
	return fmt::format(FMT_STRING("{}:{} * {:d}"), ItemToString(a_Item), a_Item.m_ItemDamage, a_Item.m_ItemCount);
}





cItem GetIniItemSet(cIniFile & a_IniFile, const char * a_Section, const char * a_Key, const char * a_Default)
{
	AString ItemStr = a_IniFile.GetValueSet(a_Section, a_Key, a_Default);
	cItem res;
	if (!StringToItem(ItemStr, res))
	{
		res.Empty();
	}
	return res;
}






