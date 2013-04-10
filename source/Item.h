#pragma once

#include "Defines.h"
#include "BlockID.h"

namespace Json
{
	class Value;
}





// tolua_begin
class cItem
{
public:
	cItem(short a_ItemType = E_ITEM_EMPTY, char a_ItemCount = 0, short a_ItemDamage = 0)
		: m_ItemType  (a_ItemType)
		, m_ItemCount (a_ItemCount)
		, m_ItemDamage(a_ItemDamage)
	{
		if (!IsValidItem(m_ItemType))
		{
			m_ItemType = E_ITEM_EMPTY;
		}
	}
	
	void Empty()
	{
		m_ItemType = E_ITEM_EMPTY;
		m_ItemCount = 0;
		m_ItemDamage = 0;
	}
	
	void Clear(void)
	{
		m_ItemType = E_ITEM_EMPTY;
		m_ItemCount = 0;
		m_ItemDamage = 0;
	}
	
	bool IsEmpty(void) const
	{
		return (m_ItemType <= 0 || m_ItemCount <= 0);
	}
	
	bool IsEqual(const cItem & a_Item) const
	{
		return (IsSameType(a_Item) && (m_ItemDamage == a_Item.m_ItemDamage));
	}
	
	bool IsSameType(const cItem & a_Item) const
	{
		return (m_ItemType == a_Item.m_ItemType) || (IsEmpty() && a_Item.IsEmpty());
	}

	// TODO Sorry for writing the functions in the header. But somehow it doesn´t worked when I put them into the cpp File :s

	inline int GetMaxDuration(void) const
	{
		switch (m_ItemType)
		{
			case 256: return 251;
			case 257: return 251;
			case 258: return 251;
			case 259: return 65;	//Lighter / Flint and Steel
			case 267: return 251;
			case 268: return 60;
			case 269: return 60;
			case 270: return 60;
			case 271: return 60;
			case 272: return 132;
			case 273: return 132;
			case 274: return 132;
			case 275: return 132;
			case 276: return 1563;
			case 277: return 1563;
			case 278: return 1563;
			case 279: return 1563;
			case 283: return 32;
			case 284: return 32;
			case 285: return 32;
			case 286: return 32;
			case 290: return 60;
			case 291: return 132;
			case 292: return 251;
			case 293: return 1563;
			case 294: return 32;
			case 359: return 251;
			default: return 0;
		}
	}

	/// Damages a weapon / tool. Returns true when destroyed
	inline bool DamageItem()
	{
		if (HasDuration())
		{
			m_ItemDamage++;
			if (m_ItemDamage >= GetMaxDuration())
				return true;
		}
		return false;
	}

	inline bool HasDuration() { return GetMaxDuration() > 0; }
	
	/// Returns true if this itemstack can stack with the specified stack (types match, enchantments etc.) ItemCounts are ignored!
	bool IsStackableWith(const cItem & a_OtherStack);

	// tolua_end
	void GetJson( Json::Value & a_OutValue ) const;
	void FromJson( const Json::Value & a_Value );
	// tolua_begin
	
	static bool IsEnchantable(short a_ItemType);

	short m_ItemType;
	char  m_ItemCount;
	short m_ItemDamage;
};
// tolua_end




// This stupid construct is here only so that Lua can access cItems in an API

class cItems  // tolua_export
	: public std::vector<cItem>
{  // tolua_export
public:
	// tolua_begin
	cItem & Get   (int a_Idx) {return at(a_Idx); }
	void    Set   (int a_Idx, const cItem & a_Item) {at(a_Idx) = a_Item; }
	void    Add   (const cItem & a_Item) {push_back(a_Item); }
	void    Delete(int a_Idx) {erase(begin() + a_Idx); }
	void    Clear (void) {clear(); }
	int     Size  (void) {return size(); }

	void    Add   (ENUM_ITEM_ID a_ItemType, char a_ItemCount, short a_ItemDamage)
	{
		push_back(cItem(a_ItemType, a_ItemCount, a_ItemDamage));
	}
	
	void    Set   (int a_Idx, ENUM_ITEM_ID a_ItemType, char a_ItemCount, short a_ItemDamage)
	{
		at(a_Idx) = cItem(a_ItemType, a_ItemCount, a_ItemDamage);
	}
	// tolua_end
} ;  // tolua_export





/// Used to store loot probability tables
class cLootProbab
{
public:
	cItem m_Item;
	int   m_MinAmount;
	int   m_MaxAmount;
	int   m_Weight;
} ;




