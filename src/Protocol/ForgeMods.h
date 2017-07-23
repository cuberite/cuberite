
// ForgeMods.h

// Data structure listing the Forge mods name and versions.

#pragma once

#include <stddef.h>

// tolua_begin
class cForgeMods
{
public:
	// TODO: can tolua++ bridge AStringMap (etc.) as a Lua table? or non-fixedsize arrays?

	/** Returns the number of Forge mods. */
	size_t GetNumMods(void) const;
	
	/** Returns true if the mod with the given name is installed. */
	bool HasMod(AString & name) const;
	
	/** Returns the version of the mod name given. */
	const AString & GetModVersion(AString & name) const;

	/** Returns the name of the Forge mod at the given index. */
	const AString & GetModNameAt(size_t i) const;

	/** Returns the version of the Forge mod at the given index. */
	const AString & GetModVersionAt(size_t i) const;
	// tolua_end
	
	cForgeMods();
	cForgeMods(AStringMap a);
	
	static const cForgeMods & Unmodded(void);
	
	void Add(AString & a_Name, AString & a_Version);
	
	void Remove(AString & a_Name);
	
	AStringMap GetMods() { return m_Mods; }
	
	bool HasMods() { return m_Mods.size() != 0; }
	
private:
	AStringMap m_Mods;
	AStringVector m_ModNames;
	AStringVector m_ModVersions;
} ; // tolua_export
