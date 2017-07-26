
// ForgeMods.cpp

// Data structure listing the Forge mods name and versions

#include "Globals.h"
#include "ForgeMods.h"

cForgeMods::cForgeMods()
{
}





cForgeMods::cForgeMods(AStringMap a):m_Mods(a)
{
	for (const auto & item: a)
	{
		m_ModNames.push_back(item.first);
	}
}




const cForgeMods & cForgeMods::Unmodded(void)
{
	static cForgeMods Unmodded;

	return Unmodded;
}




size_t cForgeMods::GetNumMods(void) const
{
	return m_Mods.size();
}



bool cForgeMods::HasMod(const AString & name) const
{
	return m_Mods.find(name) != m_Mods.end();
}




const AString & cForgeMods::GetModVersion(const AString & name) const
{
	return m_Mods.find(name)->second;
}




const AString & cForgeMods::GetModNameAt(size_t i) const
{
	return m_ModNames[i];
}





void cForgeMods::Add(const AString & a_Name, const AString & a_Version)
{
	m_ModNames.push_back(a_Name);
	m_Mods.insert({a_Name, a_Version});
}





void cForgeMods::Remove(const AString & a_Name)
{
	auto it = std::find(m_ModNames.begin(), m_ModNames.end(), a_Name);
	if (it != m_ModNames.end())
	{
		m_ModNames.erase(it);
	}

	m_Mods.erase(a_Name);
}
