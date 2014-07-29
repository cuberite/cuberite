
// Item.h

// Declares the cItem class representing an item (in the inventory sense)





#pragma once

#include "Defines.h"
#include "Enchantments.h"
#include "WorldStorage/FireworksSerializer.h"





// fwd:
class cItemHandler;

namespace Json
{
	class Value;
}





// tolua_begin
class cItem
{
public:
	/** Creates an empty item */
	cItem(void) :
		m_ItemType(E_ITEM_EMPTY),
		m_ItemCount(0),
		m_ItemDamage(0),
		m_CustomName(""),
		m_Lore(""),
		m_RepairCost(0),
		m_FireworkItem()
	{
	}
	
	
	/** Creates an item of the specified type, by default 1 piece with no damage and no enchantments */
	cItem(
		short a_ItemType,
		char a_ItemCount = 1,
		short a_ItemDamage = 0,
		const AString & a_Enchantments = "",
		const AString & a_CustomName = "",
		const AString & a_Lore = ""
	) :
		m_ItemType    (a_ItemType),
		m_ItemCount   (a_ItemCount),
		m_ItemDamage  (a_ItemDamage),
		m_Enchantments(a_Enchantments),
		m_CustomName  (a_CustomName),
		m_Lore        (a_Lore),
		m_RepairCost  (0),
		m_FireworkItem()
	{
		if (!IsValidItem(m_ItemType))
		{
			if ((m_ItemType != E_BLOCK_AIR) && (m_ItemType != E_ITEM_EMPTY))
			{
				LOGWARNING("%s: creating an invalid item type (%d), resetting to empty.", __FUNCTION__, a_ItemType);
			}
			Empty();
		}
	}
	
	
	// The constructor is disabled in code, because the compiler generates it anyway,
	// but it needs to stay because ToLua needs to generate the binding for it
	#if 0
	
	/** Creates an exact copy of the item */
	cItem(const cItem & a_CopyFrom) :
		m_ItemType    (a_CopyFrom.m_ItemType),
		m_ItemCount   (a_CopyFrom.m_ItemCount),
		m_ItemDamage  (a_CopyFrom.m_ItemDamage),
		m_Enchantments(a_CopyFrom.m_Enchantments),
		m_CustomName  (a_CopyFrom.m_CustomName),
		m_Lore        (a_CopyFrom.m_Lore),
		m_RepairCost  (a_CopyFrom.m_RepairCost),
		m_FireworkItem(a_CopyFrom.m_FireworkItem)
	{
	}
	
	#endif
	
	
	void Empty(void)
	{
		m_ItemType = E_ITEM_EMPTY;
		m_ItemCount = 0;
		m_ItemDamage = 0;
		m_Enchantments.Clear();
		m_CustomName = "";
		m_Lore = "";
		m_RepairCost = 0;
		m_FireworkItem.EmptyData();
	}
	
	
	void Clear(void)
	{
		m_ItemType = E_ITEM_EMPTY;
		m_ItemCount = 0;
		m_ItemDamage = 0;
		m_RepairCost = 0;
	}
	
	
	bool IsEmpty(void) const
	{
		return ((m_ItemType <= 0) || (m_ItemCount <= 0));
	}
	
	/* Returns true if this itemstack can stack with the specified stack (types match, enchantments etc.)
	ItemCounts are ignored. */
	bool IsEqual(const cItem & a_Item) const
	{
		return (
			IsSameType(a_Item) &&
			(m_ItemDamage == a_Item.m_ItemDamage) &&
			(m_Enchantments == a_Item.m_Enchantments) &&
			(m_CustomName == a_Item.m_CustomName) &&
			(m_Lore == a_Item.m_Lore) &&
			m_FireworkItem.IsEqualTo(a_Item.m_FireworkItem)
		);
	}
	
	
	bool IsSameType(const cItem & a_Item) const
	{
		return (m_ItemType == a_Item.m_ItemType) || (IsEmpty() && a_Item.IsEmpty());
	}


	bool IsBothNameAndLoreEmpty(void) const
	{
		return (m_CustomName.empty() && m_Lore.empty());
	}


	bool IsCustomNameEmpty(void) const { return (m_CustomName.empty()); }
	bool IsLoreEmpty(void) const { return (m_Lore.empty()); }

	/** Returns a copy of this item with m_ItemCount set to 1. Useful to preserve enchantments etc. on stacked items */
	cItem CopyOne(void) const;
	
	/** Adds the specified count to this object and returns the reference to self (useful for chaining) */
	cItem & AddCount(char a_AmountToAdd);
	
	/** Returns the maximum damage value that this item can have; zero if damage is not applied */
	short GetMaxDamage(void) const;
	
	/** Damages a weapon / tool. Returns true when damage reaches max value and the item should be destroyed */
	bool DamageItem(short a_Amount = 1);

	inline bool IsDamageable(void) const { return (GetMaxDamage() > 0); }
	
	/** Returns true if the item is stacked up to its maximum stacking. */
	bool IsFullStack(void) const;
	
	/** Returns the maximum amount of stacked items of this type. */
	char GetMaxStackSize(void) const;

	// tolua_end
	
	/** Returns the cItemHandler responsible for this item type */
	cItemHandler * GetHandler(void) const;
	
	/** Saves the item data into JSON representation */
	void GetJson(Json::Value & a_OutValue) const;
	
	/** Loads the item data from JSON representation */
	void FromJson(const Json::Value & a_Value);
	
	/** Returns true if the specified item type is enchantable (as per 1.2.5 protocol requirements) */
	static bool IsEnchantable(short a_ItemType);  // tolua_export

	/** Returns the enchantability of the item. When the item hasn't a enchantability, it will returns 0 */
	int GetEnchantability();  // tolua_export

	/** Enchants the item using the specified number of XP levels.
	Returns true if item enchanted, false if not. */
	bool EnchantByXPLevels(int a_NumXPLevels);  // tolua_export

	// tolua_begin
	
	short          m_ItemType;
	char           m_ItemCount;
	short          m_ItemDamage;
	cEnchantments  m_Enchantments;
	AString        m_CustomName;
	AString        m_Lore;

	int            m_RepairCost;
	cFireworkItem  m_FireworkItem;
};
// tolua_end





/** This class bridges a vector of cItem for safe access via Lua. It checks boundaries for all accesses
Note that this class is zero-indexed!
*/
class cItems  // tolua_export
	: public std::vector<cItem>
{  // tolua_export
public:
	// tolua_begin
	
	/** Need a Lua-accessible constructor */
	cItems(void) {}
	
	cItem * Get   (int a_Idx);
	void    Set   (int a_Idx, const cItem & a_Item);
	void    Add   (const cItem & a_Item) {push_back(a_Item); }
	void    Delete(int a_Idx);
	void    Clear (void) {clear(); }
	size_t  Size  (void) const { return size(); }
	void    Set   (int a_Idx, short a_ItemType, char a_ItemCount, short a_ItemDamage);

	void    Add   (short a_ItemType, char a_ItemCount, short a_ItemDamage)
	{
		push_back(cItem(a_ItemType, a_ItemCount, a_ItemDamage));
	}
	
	// tolua_end
} ;  // tolua_export





/** Used to store loot probability tables */
class cLootProbab
{
public:
	cItem m_Item;
	int   m_MinAmount;
	int   m_MaxAmount;
	int   m_Weight;
} ;




