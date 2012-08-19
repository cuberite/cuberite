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
	cItem(short a_ItemType = E_ITEM_EMPTY, char a_ItemCount = 0, short a_ItemHealth = 0)
		: m_ItemType  (a_ItemType)
		, m_ItemCount (a_ItemCount)
		, m_ItemHealth(a_ItemHealth)
	{
		if (!IsValidItem( m_ItemID ) ) m_ItemID = E_ITEM_EMPTY;
	}
	
	void Empty()
	{
		m_ItemID = E_ITEM_EMPTY;
		m_ItemCount = 0;
		m_ItemHealth = 0;
	}
	
	void Clear(void)
	{
		m_ItemID = E_ITEM_EMPTY;
		m_ItemCount = 0;
		m_ItemHealth = 0;
	}
	
	bool IsEmpty(void) const
	{
		return (m_ItemID <= 0 || m_ItemCount <= 0);
	}
	
	// tolua_end
	OBSOLETE
	// tolua_begin
	bool Equals(const cItem & a_Item) const  // obsolete, use IsEqual() instead
	{
		return IsEqual(a_Item);
	}
	
	bool IsEqual(const cItem & a_Item) const
	{
		return (IsSameType(a_Item) && (m_ItemHealth == a_Item.m_ItemHealth));
	}
	
	bool IsSameType(const cItem & a_Item) const
	{
		return (m_ItemID == a_Item.m_ItemID) || (IsEmpty() && a_Item.IsEmpty());
	}

	// TODO Sorry for writing the functions in the header. But somehow it doesn´t worked when I put them into the cpp File :s

	inline int GetMaxDuration(void) const
	{
		switch(m_ItemID)
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

	// Damages a weapon / tool. Returns true when destroyed
	inline bool DamageItem()
	{
		if (HasDuration())
		{
			m_ItemHealth++;
			if(m_ItemHealth >= GetMaxDuration())
				return true;
		}
		return false;
	}

	inline bool HasDuration() { return GetMaxDuration() > 0; }

	// tolua_end
	void GetJson( Json::Value & a_OutValue ) const;
	void FromJson( const Json::Value & a_Value );
	// tolua_begin
	
	static bool IsEnchantable(short a_ItemType);

	// tolua_end
	union
	{
		// tolua_begin
		short m_ItemID;  // OBSOLETE, use m_ItemType instead
		short m_ItemType;
		// tolua_end
	} ;
	char         m_ItemCount;  // tolua_export
	union
	{
		// tolua_begin
		short      m_ItemHealth;  // OBSOLETE, use m_ItemDamage instead
		short      m_ItemDamage;
		// tolua_end
	} ;
	// tolua_begin
};
// tolua_end

typedef std::vector<cItem> cItems;




