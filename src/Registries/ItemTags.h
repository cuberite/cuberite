
// ItemTags.h

#pragma once

#include "../Item.h"
#include "variant"

/*
Item tags are loaded from the item.ini
You can export them from the minecraft sever with data generators

This list is up to date with 1.16.2 if there are new added please add below:
https://minecraft.gamepedia.com/Tag#Items
*/

using cItemTag = std::vector<std::variant<cItem, AString>>;

class cItemTags
{
public:
	cItemTags();

	/** Resolves the given string to a cItems */
	const cItems Resolve(const AString & a_Name) const;

protected:
	void AddToMap(const AString & a_Name, const AString & a_Value);

	std::unordered_map<AString, cItemTag> m_Map;
};

/*
static cItemTags & GetItemTags()
{
	static cItemTags ItemTags;
	return ItemTags;
}
*/  // TODO: uncomment when used - 12xx12
