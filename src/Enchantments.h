// Enchantments.h

// Declares the cEnchantments class representing a storage for item enchantments and stored-enchantments





#pragma once

#include "Defines.h"
#include "FastRandom.h"
#include "WorldStorage/EnchantmentSerializer.h"



// fwd: "WorldStorage/FastNBT.h"
class cFastNBTWriter;
class cParsedNBT;


// fwd:
struct cWeightedEnchantment;

typedef std::vector<cWeightedEnchantment> cWeightedEnchantments;





/** Class that stores item enchantments or stored-enchantments
The enchantments may be serialized to a stringspec and read back from such stringspec.
The format for the stringspec is "id=lvl;id=lvl;id=lvl...", with an optional semicolon at the end,
mapping each enchantment's id onto its level. ID may be either a number or the enchantment name.
Level value of 0 means no such enchantment, and it will not be stored in the m_Enchantments.
Serialization will never put zero-level enchantments into the stringspec and will always use numeric IDs.
*/


// tolua_begin
class cEnchantments
{
public:
	/** Individual enchantment IDs, corresponding to their NBT IDs: https://minecraft.gamepedia.com/Data_values#Enchantment_IDs
	*/

	enum eEnchantment
	{
		// Currently missing: Frost walker, curse of binding, sweeping edge, mending, and curse of vanishing.
		enchProtection           = 0,
		enchFireProtection       = 1,
		enchFeatherFalling       = 2,
		enchBlastProtection      = 3,
		enchProjectileProtection = 4,
		enchRespiration          = 5,
		enchAquaAffinity         = 6,
		enchThorns               = 7,
		enchDepthStrider         = 8,
		enchSharpness            = 16,
		enchSmite                = 17,
		enchBaneOfArthropods     = 18,
		enchKnockback            = 19,
		enchFireAspect           = 20,
		enchLooting              = 21,
		enchEfficiency           = 32,
		enchSilkTouch            = 33,
		enchUnbreaking           = 34,
		enchFortune              = 35,
		enchPower                = 48,
		enchPunch                = 49,
		enchFlame                = 50,
		enchInfinity             = 51,
		enchLuckOfTheSea         = 61,
		enchLure                 = 62,
	} ;

	/** Creates an empty enchantments container */
	cEnchantments(void);

	/** Creates an enchantments container filled with enchantments parsed from stringspec */
	cEnchantments(const AString & a_StringSpec);

	/** Adds the enchantments contained in a_Other into this object.
	Existing enchantments are preserved, unless a_Other specifies a different level, in which case the level is changed to the a_Other's one. */
	void Add(const cEnchantments & a_Other);

	/** Adds enchantments in the stringspec; if a specified enchantment already exists, overwrites it */
	void AddFromString(const AString & a_StringSpec);

	/** Get the count of enchantments */
	size_t Count(void) const;

	/** Serializes all the enchantments into a string */
	AString ToString(void) const;

	/** Returns the level for the specified enchantment; 0 if not stored */
	unsigned int GetLevel(int a_EnchantmentID) const;

	/** Sets the level for the specified enchantment, adding it if not stored before or removing it if level <= 0 */
	void SetLevel(int a_EnchantmentID, unsigned int a_Level);

	/** Removes all enchantments */
	void Clear(void);

	/** Returns true if there are no enchantments */
	bool IsEmpty(void) const;

	/** Returns true if the given enchantment could be legally added to this object. Note that adding the enchantment may not actually increase the level. */
	bool CanAddEnchantment(int a_EnchantmentID) const;

	/** Converts enchantment name or ID (number in string) to the numeric representation; returns -1 if enchantment name not found; case insensitive */
	static int StringToEnchantmentID(const AString & a_EnchantmentName);

	/** Returns true if a_Other contains exactly the same enchantments and levels */
	bool operator ==(const cEnchantments & a_Other) const;

	// tolua_end

	/** Get the XP cost multiplier for the enchantment (for anvils).
	If FromBook is true, then this function returns the XP multiplier if
	the enchantment is coming from a book, otherwise it returns the normal
	item multiplier. */
	static int GetXPCostMultiplier(int a_EnchantmentID, bool FromBook);

	/** Get the maximum level the enchantment can have */
	static unsigned int GetLevelCap(int a_EnchantmentID);

	/** Add enchantment weights from item to the vector */
	static void AddItemEnchantmentWeights(cWeightedEnchantments & a_Enchantments, short a_ItemType, int a_EnchantmentLevel);

	/** Add a enchantment with weight to the vector */
	static void AddEnchantmentWeightToVector(cWeightedEnchantments & a_Enchantments, int a_Weight, int a_EnchantmentID, unsigned int a_EnchantmentLevel);

	/** Remove the entire enchantment (with weight) from the vector */
	static void RemoveEnchantmentWeightFromVector(cWeightedEnchantments & a_Enchantments, int a_EnchantmentID);

	/** Remove the entire enchantment (with weight) from the vector */
	static void RemoveEnchantmentWeightFromVector(cWeightedEnchantments & a_Enchantments, const cEnchantments & a_Enchantment);

	/** Check enchantment conflicts from enchantments from the vector */
	static void CheckEnchantmentConflictsFromVector(cWeightedEnchantments & a_Enchantments, const cEnchantments & a_FirstEnchantment);

	/** Gets random enchantment from Vector and returns it, with randomness derived from the provided PRNG. */
	static cEnchantments GetRandomEnchantmentFromVector(const cWeightedEnchantments & a_Enchantments, MTRand & a_Random);

	/** Selects one enchantment from a Vector using cNoise. Mostly used for generators.
	Uses the enchantments' weights for the random distribution.
	If a_Enchantments is empty, returns an empty enchantment. */
	static cEnchantments SelectEnchantmentFromVector(const cWeightedEnchantments & a_Enchantments, int a_Seed);

	/** Returns true if a_Other doesn't contain exactly the same enchantments and levels */
	bool operator !=(const cEnchantments & a_Other) const;

	/** Writes the enchantments into the specified NBT writer; begins with the LIST tag of the specified name ("ench" or "StoredEnchantments") */
	friend void EnchantmentSerializer::WriteToNBTCompound(const cEnchantments & a_Enchantments, cFastNBTWriter & a_Writer, const AString & a_ListTagName);

	/** Reads the enchantments from the specified NBT list tag (ench or StoredEnchantments) */
	friend void EnchantmentSerializer::ParseFromNBT(cEnchantments & a_Enchantments, const cParsedNBT & a_NBT, int a_EnchListTagIdx);

protected:
	/** Maps enchantment ID -> enchantment level */
	typedef std::map<int, unsigned int> cMap;

	/** Currently stored enchantments */
	cMap m_Enchantments;

public:
	/** Make this class iterable */
	cMap::const_iterator begin() const { return m_Enchantments.begin(); }
	cMap::const_iterator end()   const { return m_Enchantments.end(); }
};  // tolua_export




// Define the cWeightedEnchantment struct for the Enchanting System to store the EnchantmentWeights:
struct cWeightedEnchantment
{
	int m_Weight;
	cEnchantments m_Enchantments;
};



