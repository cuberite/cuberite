
// Enchantments.h

// Declares the cEnchantments class representing a storage for item enchantments and stored-enchantments





#pragma once





// fwd: WorldStorage/FastNBT.h
class cFastNBTWriter;
class cParsedNBT;





// tolua_begin

/** Class that stores item enchantments or stored-enchantments
The enchantments may be serialized to a stringspec and read back from such stringspec.
The format for the stringspec is "id=lvl;id=lvl;id=lvl...", with an optional semicolon at the end,
mapping each enchantment's id onto its level. ID may be either a number or the enchantment name.
Level value of 0 means no such enchantment, and it will not be stored in the m_Enchantments.
Serialization will never put zero-level enchantments into the stringspec and will always use numeric IDs.
*/
class cEnchantments
{
public:
	/// Individual enchantment IDs, corresponding to their NBT IDs ( http://www.minecraftwiki.net/wiki/Data_Values#Enchantment_IDs )
	enum
	{
		enchProtection           = 0,
		enchFireProtection       = 1,
		enchFeatherFalling       = 2,
		enchBlastProtection      = 3,
		enchProjectileProtection = 4,
		enchRespiration          = 5,
		enchAquaAffinity         = 6,
		enchThorns               = 7,
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
	} ;
	
	/// Creates an empty enchantments container
	cEnchantments(void);
	
	/// Creates an enchantments container filled with enchantments parsed from stringspec
	cEnchantments(const AString & a_StringSpec);
	
	/// Adds enchantments in the stringspec; if a specified enchantment already exists, overwrites it
	void AddFromString(const AString & a_StringSpec);
	
	/// Serializes all the enchantments into a string
	AString ToString(void) const;
	
	/// Returns the level for the specified enchantment; 0 if not stored
	int GetLevel(int a_EnchantmentID) const;
	
	/// Sets the level for the specified enchantment, adding it if not stored before or removing it if level <= 0
	void SetLevel(int a_EnchantmentID, int a_Level);
	
	/// Removes all enchantments
	void Clear(void);
	
	/// Returns true if there are no enchantments
	bool IsEmpty(void) const;
	
	/// Converts enchantment name to the numeric representation; returns -1 if enchantment name not found; case insensitive
	static int StringToEnchantmentID(const AString & a_EnchantmentName);
	
	/// Returns true if a_Other contains exactly the same enchantments and levels
	bool operator ==(const cEnchantments & a_Other) const;
	
	// tolua_end
	
	/// Returns true if a_Other doesn't contain exactly the same enchantments and levels
	bool operator !=(const cEnchantments & a_Other) const;
	
	/// Writes the enchantments into the specified NBT writer; begins with the LIST tag of the specified name ("ench" or "StoredEnchantments")
	void WriteToNBTCompound(cFastNBTWriter & a_Writer, const AString & a_ListTagName) const;
	
	/// Reads the enchantments from the specified NBT list tag (ench or StoredEnchantments)
	void ParseFromNBT(const cParsedNBT & a_NBT, int a_EnchListTagIdx);
	
protected:
	/// Maps enchantment ID -> enchantment level
	typedef std::map<int, int> cMap;
	
	/// Currently stored enchantments
	cMap m_Enchantments;
} ;  // tolua_export




