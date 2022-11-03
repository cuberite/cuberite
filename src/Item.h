
// Item.h

// Declares the cItem class representing an item (in the inventory sense)





#pragma once

#include <optional>

#include "Defines.h"
#include "Enchantments.h"
#include "WorldStorage/FireworksSerializer.h"
#include "Color.h"





// fwd:
class cItemHandler;
class cItemGrid;
class cColor;

namespace Json
{
	class Value;
}





// tolua_begin
class cItem
{
public:
	/** Creates an empty item */
	cItem(void);

	/** Creates an item of the specified type, by default 1 piece with no damage and no enchantments */
	cItem(
		short a_ItemType,
		char a_ItemCount = 1,
		short a_ItemDamage = 0,
		const AString & a_Enchantments = "",
		const AString & a_CustomName = "",
		const AStringVector & a_LoreTable = {}
	);

	// The constructor is disabled in code, because the compiler generates it anyway,
	// but it needs to stay because ToLua needs to generate the binding for it
	#ifdef TOLUA_EXPOSITION

	/** Creates an exact copy of the item */
	cItem(const cItem & a_CopyFrom);

	#endif

	/** Empties the item and frees up any dynamic storage used by the internals. */
	void Clear(void);

	/** Returns true if the item represents an empty stack - either the type is invalid, or count is zero. */
	bool IsEmpty(void) const
	{
		return ((m_ItemType <= 0) || (m_ItemCount <= 0));
	}

	/* Returns true if this item stack can stack with the specified stack (types match, enchantments etc.)
	ItemCounts are ignored. */
	bool IsEqual(const cItem & a_Item) const
	{
		auto displayLhs = get<cDisplayProperties>();
		auto displayRhs = a_Item.get<cDisplayProperties>();

		return (
			IsSameType(a_Item) &&
			(m_ItemDamage == a_Item.m_ItemDamage) &&
			(get<cEnchantments>().value_or(cEnchantments()) == a_Item.get<cEnchantments>().value_or(cEnchantments())) &&
			(get<cFireworkItem>().value_or(cFireworkItem()).IsEqualTo(a_Item.get<cFireworkItem>().value_or(cFireworkItem()))) &&
			(
				(
					displayLhs.has_value() &&
					displayRhs.has_value() &&
					(displayLhs.value().m_CustomName == displayRhs.value().m_CustomName) &&
					(displayLhs.value().m_LoreTable == displayRhs.value().m_LoreTable)
				)
				||
				(
					!displayLhs.has_value() &&
					!displayRhs.has_value()
				)
			)
		);
	}


	bool IsSameType(const cItem & a_Item) const
	{
		return (m_ItemType == a_Item.m_ItemType) || (IsEmpty() && a_Item.IsEmpty());
	}


	bool IsBothNameAndLoreEmpty(void) const
	{
		auto DisplayProperties = get<cDisplayProperties>();
		if (!DisplayProperties.has_value())
		{
			return true;
		}
		return (DisplayProperties.value().m_CustomName.empty() && DisplayProperties.value().m_LoreTable.empty());
	}


	bool IsCustomNameEmpty(void) const
	{
		auto DisplayProperties = get<cDisplayProperties>();
		if (!DisplayProperties.has_value())
		{
			return false;
		}
		return DisplayProperties.value().m_CustomName.empty();
	}

	bool IsLoreEmpty(void) const
	{
		auto DisplayProperties = get<cDisplayProperties>();
		if (!DisplayProperties.has_value())
		{
			return false;
		}
		return (DisplayProperties.value().m_LoreTable.empty());
	}

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
	const cItemHandler & GetHandler(void) const;

	/** Saves the item data into JSON representation */
	void GetJson(Json::Value & a_OutValue) const;

	/** Loads the item data from JSON representation */
	void FromJson(const Json::Value & a_Value);

	/** Returns true if the specified item type is enchantable.
	If FromBook is true, the function is used in the anvil inventory with book enchantments.
	So it checks the "only book enchantments" too. Example: You can only enchant a hoe with a book. */
	static bool IsEnchantable(short a_ItemType, bool a_FromBook = false);  // tolua_export

	/** Returns the enchantability of the item. When the item hasn't a enchantability, it will returns 0 */
	unsigned GetEnchantability();  // tolua_export

	/** Randomly enchants the item using the specified number of XP levels.
	Returns true if the item was enchanted, false if not (not enchantable / too many enchantments already).
	Randomness is derived from the provided PRNG. */
	bool EnchantByXPLevels(unsigned a_NumXPLevels, MTRand & a_Random);  // Exported in ManualBindings.cpp

	/** Adds this specific enchantment to this item, returning the cost.
	FromBook specifies whether the enchantment should be treated as coming
	from a book. If true, then the cost returned uses the book values, if
	false it uses the normal item multipliers. */
	int AddEnchantment(int a_EnchantmentID, unsigned int a_Level, bool a_FromBook);  // tolua_export

	/** Adds the enchantments on a_Other to this item, returning the
	XP cost of the transfer. */
	int AddEnchantmentsFromItem(const cItem & a_Other);  // tolua_export

	/** Returns whether or not this item is allowed to have the given enchantment. Note: Does not check whether the enchantment is exclusive with the current enchantments on the item. */
	bool CanHaveEnchantment(int a_EnchantmentID);

	// tolua_begin
	short          m_ItemType;
	char           m_ItemCount;
	short          m_ItemDamage;
	int            m_RepairCost;
	// tolua_end


	// https://minecraft.fandom.com/wiki/Tutorials/Command_NBT_tags#Items
	struct cDisplayProperties
	{
		AString m_CustomName;
		AStringVector  m_LoreTable;  // Exported in ManualBindings.cpp
		cColor m_Color;
	};

	struct cAdditionalBlockProperties
	{
		std::vector<std::pair<ENUM_BLOCK_TYPE, unsigned char>> m_CanPlaceOn;

	};

	std::vector<std::variant<
		cEnchantments,
		// cEntityTag,
		cDisplayProperties,
		// cAttributes,
		// bool,  // unbreakable
		// AString,  // SkullOwner
		// cHideFlags,
		// std::unique_ptr<cBlockEntity>,
		cFireworkItem
	>> m_Properties;

	/** Returns the requested property, if there is none present, the optional will handle this */
	template <class type>
	std::optional<type> get() const
	{
		for (auto & property : m_Properties)
		{
			if (std::holds_alternative<type>(property))
			{
				return std::get<type>(property);
			}
		}
		return {};
	}

	/** replaces the property if present, if not saves it */
	template <class type>
	void set(const type & a_Value)
	{
		for (auto & property : m_Properties)
		{
			if (std::holds_alternative<type>(property))
			{
				property = a_Value;
				return;
			}
		}
		m_Properties.template emplace_back(a_Value);
	}

	/** Only Sets a value if there is none present */
	template <class type>
	void SafeSet(const type & a_Value)
	{
		for (auto & property : m_Properties)
	{
			if (std::holds_alternative<type>(property))
			{
				return;
			}
		}
		m_Properties.emplace_back(a_Value);
	}

	/**
	Compares two items for the same type or category. Type of item is defined
	via `m_ItemType` and `m_ItemDamage`. Some items (e.g. planks) have the same
	`m_ItemType` and the wood kind is defined via `m_ItemDamage`. `-1` is used
	as placeholder for all kinds (e.g. all kind of planks).

	Items are different when the `ItemType` is different or the `ItemDamage`
	is different and unequal -1.
	*/
	struct sItemCompare
	{
		bool operator() (const cItem & a_Lhs, const cItem & a_Rhs) const
		{
			if (a_Lhs.m_ItemType != a_Rhs.m_ItemType)
			{
				return (a_Lhs.m_ItemType < a_Rhs.m_ItemType);
			}
			if ((a_Lhs.m_ItemDamage == -1) || (a_Rhs.m_ItemDamage == -1))
			{
				return false;  // -1 is a wildcard, damage of -1 always compares equal
			}
			return (a_Lhs.m_ItemDamage < a_Rhs.m_ItemDamage);
		}
	};
};  // tolua_export





/** This class bridges a vector of cItem for safe access via Lua. It checks boundaries for all accesses
Note that this class is zero-indexed!
*/
class cItems  // tolua_export
	: public std::vector<cItem>
{  // tolua_export
public:

	cItems(const cItems &) = default;
	cItems(cItems &&) = default;
	cItems & operator = (const cItems &) = default;
	cItems & operator = (cItems &&) = default;

	/** Constructs a new instance containing the specified item. */
	cItems(cItem && a_InitialItem);

	// tolua_begin

	/** Need a Lua-accessible constructor */
	cItems(void) {}

	cItem * Get   (int a_Idx);
	void    Set   (int a_Idx, const cItem & a_Item);
	void    Add   (const cItem & a_Item) {push_back(a_Item); }
	void    Add   (short a_ItemType) { emplace_back(a_ItemType); }
	void    Add   (short a_ItemType, char a_ItemCount) { emplace_back(a_ItemType, a_ItemCount); }
	void    Delete(int a_Idx);
	void    Clear (void) {clear(); }
	size_t  Size  (void) const { return size(); }
	void    Set   (int a_Idx, short a_ItemType, char a_ItemCount, short a_ItemDamage);
	bool    Contains(const cItem & a_Item);
	bool    ContainsType(const cItem & a_Item);

	void    Add   (short a_ItemType, char a_ItemCount, short a_ItemDamage)
	{
		emplace_back(a_ItemType, a_ItemCount, a_ItemDamage);
	}

	/** Adds a copy of all items in a_ItemGrid. */
	void AddItemGrid(const cItemGrid & a_ItemGrid);

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
