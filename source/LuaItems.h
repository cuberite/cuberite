
// LuaItems.h

// Interfaces to the cLuaItems class representing a wrapper class that allows Lua to access and manipulate cItems





#pragma once

#include "cItem.h"





// tolua_begin
class cLuaItems
{
public:
	// tolua_end
	// The constructor is not to be Lua-exported, Lua cannot make use of this object
	cLuaItems(cItems & a_Items) :
		m_Items(a_Items)
	{
	}
	
	// tolua_begin
	cItem & Get   (int a_Idx) {return m_Items[a_Idx]; }
	void    Set   (int a_Idx, const cItem & a_Item) {m_Items[a_Idx] = a_Item; }
	void    Add   (const cItem & a_Item) {m_Items.push_back(a_Item); }
	void    Delete(int a_Idx) {m_Items.erase(m_Items.begin() + a_Idx); }
	void    Clear (void) {m_Items.clear(); }
	int     Size  (void) {return m_Items.size(); }

	void    Add   (ENUM_ITEM_ID a_ItemType, char a_ItemCount, short a_ItemHealth)
	{
		m_Items.push_back(cItem(a_ItemType, a_ItemCount, a_ItemHealth));
	}
	
	void    Set   (int a_Idx, ENUM_ITEM_ID a_ItemType, char a_ItemCount, short a_ItemHealth)
	{
		m_Items[a_Idx] = cItem(a_ItemType, a_ItemCount, a_ItemHealth);
	}
	
protected:
	cItems & m_Items;
} ;
// tolua_end




