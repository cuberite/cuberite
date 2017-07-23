
// ForgeMods.cpp

// Data structure listing the Forge mods name and versions

#include "Globals.h"
#include "ForgeMods.h"

cForgeMods::cForgeMods()
{
}





cForgeMods::cForgeMods(AStringMap a):m_Mods(a)
{
	for (auto const & item: a)
	{
		m_ModNames.push_back(item.first);
		m_ModVersions.push_back(item.second);
	}
}




const cForgeMods & cForgeMods::Unmodded(void)
{
	static cForgeMods unmodded;

	return unmodded;
}




size_t cForgeMods::GetNumMods(void) const
{
	return m_Mods.size();
}



bool cForgeMods::HasMod(AString & name) const
{
	return m_Mods.find(name) != m_Mods.end();
}




const AString & cForgeMods::GetModVersion(AString & name) const
{
	return m_Mods.find(name)->second;
}




const AString & cForgeMods::GetModNameAt(size_t i) const
{
	return m_ModNames[i];
}





const AString & cForgeMods::GetModVersionAt(size_t i) const
{
	return m_ModVersions[i];
}




void cForgeMods::Add(AString & a_Name, AString & a_Version)
{
	m_ModNames.push_back(a_Name);
	m_ModVersions.push_back(a_Version);
	m_Mods.insert(std::make_pair(a_Name, a_Version));
}





void cForgeMods::Remove(AString & a_Name)
{
	auto it = std::find(m_ModNames.begin(), m_ModNames.end(), a_Name);
	if (it != m_ModNames.end())
	{
		size_t index = std::distance(m_ModNames.begin(), it);

		m_ModNames.erase(m_ModNames.begin() + index);
		m_ModVersions.erase(m_ModVersions.begin() + index);
	}

	m_Mods.erase(a_Name);
}
