
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
	bool HasMod(const AString & name) const;

	/** Returns the version of the mod name given. */
	const AString & GetModVersion(const AString & name) const;

	/** Returns the name of the Forge mod at the given index. */
	const AString & GetModNameAt(size_t i) const;

	// tolua_end

	cForgeMods();
	cForgeMods(AStringMap a);

	static const cForgeMods & Unmodded(void);

	void Add(const AString & a_Name, const AString & a_Version);

	void Remove(const AString & a_Name);

	AStringMap GetMods() { return m_Mods; }

	bool HasMods() { return m_Mods.size() != 0; }

private:
	AStringMap m_Mods;
	AStringVector m_ModNames;
} ;  // tolua_export
