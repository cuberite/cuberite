#pragma once

#include "Defines.h"
#include "BlockID.h"

namespace Json
{
	class Value;
};

class cItem																								//tolua_export
{																										//tolua_export
public:
	cItem( ENUM_ITEM_ID a_ItemID = E_ITEM_EMPTY, char a_ItemCount = 0, short a_ItemHealth = 0 )			//tolua_export
		: m_ItemID		( a_ItemID )
		, m_ItemCount	( a_ItemCount )
		, m_ItemHealth	( a_ItemHealth )
	{																									//tolua_export
		if(!IsValidItem( m_ItemID ) ) m_ItemID = E_ITEM_EMPTY;
	}																									//tolua_export
	void Empty()																						//tolua_export
	{																									//tolua_export
		m_ItemID = E_ITEM_EMPTY;
		m_ItemCount = 0;
		m_ItemHealth = 0;
	}																									//tolua_export
	bool IsEmpty(void) const																	//tolua_export
	{																									//tolua_export
		return (m_ItemID <= 0 || m_ItemCount <= 0);
	}																									//tolua_export
	bool Equals( cItem & a_Item ) const														//tolua_export
	{																									//tolua_export
		return ( (m_ItemID == a_Item.m_ItemID) && (m_ItemHealth == a_Item.m_ItemHealth) );
	}																									//tolua_export

	//TODO Sorry for writing the functions in the header. But somehow it doesn´t worked when I put them into the cpp File :s

	inline int GetMaxDuration()
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

	//Damages a weapon. Returns true when destroyed
	inline bool DamageItem() {
		if(HasDuration())
		{
			m_ItemHealth++;

			if(m_ItemHealth >= GetMaxDuration())
				return true;

		}
		return false;
	}

	inline bool HasDuration() { return GetMaxDuration() > 0; }

	void GetJson( Json::Value & a_OutValue );															//tolua_export
	void FromJson( const Json::Value & a_Value );														//tolua_export
	
	static bool IsEnchantable(ENUM_ITEM_ID item);

	ENUM_ITEM_ID	m_ItemID;																			//tolua_export
	char			m_ItemCount;																		//tolua_export
	short			m_ItemHealth;																		//tolua_export

}; //tolua_export