
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WebPlugin.h"
#include "../WebAdmin.h"
#include "../Root.h"





cWebPlugin::cWebPlugin()
{
	auto & WebAdmin = cRoot::Get()->GetWebAdmin();
	WebAdmin.AddPlugin(this);
}





cWebPlugin::~cWebPlugin()
{
	ASSERT(m_Tabs.empty());  // Has ClearTabs() been called?

	// Remove from WebAdmin:
	auto & WebAdmin = cRoot::Get()->GetWebAdmin();
	WebAdmin.RemovePlugin(this);
}





cWebPlugin::cTabNames cWebPlugin::GetTabNames(void) const
{
	std::list< std::pair<AString, AString>> NameList;
	for (auto itr = m_Tabs.cbegin(), end = m_Tabs.cend(); itr != end; ++itr)
	{
		NameList.push_back(std::make_pair((*itr)->m_Title, (*itr)->m_SafeTitle));
	}
	return NameList;
}





cWebPlugin::cTabPtr cWebPlugin::GetTabBySafeTitle(const AString & a_SafeTitle) const
{
	cCSLock Lock(m_CSTabs);
	for (auto itr = m_Tabs.cbegin(), end = m_Tabs.cend(); itr != end; ++itr)
	{
		if ((*itr)->m_SafeTitle == a_SafeTitle)
		{
			return *itr;
		}
	}
	return nullptr;
}





std::pair<AString, AString> cWebPlugin::GetTabNameForRequest(const HTTPRequest & a_Request)
{
	AStringVector Split = StringSplit(a_Request.Path, "/");
	if (Split.empty())
	{
		return std::make_pair(AString(), AString());
	}

	cCSLock Lock(m_CSTabs);
	cTabPtr Tab;
	if (Split.size() > 2)  // If we got the tab name, show that page
	{
		for (auto itr = m_Tabs.cbegin(), end = m_Tabs.cend(); itr != end; ++itr)
		{
			if ((*itr)->m_SafeTitle.compare(Split[2]) == 0)  // This is the one!
			{
				return std::make_pair((*itr)->m_Title, (*itr)->m_SafeTitle);
			}
		}
		// Tab name not found, display an "empty" page:
		return std::make_pair(AString(), AString());
	}

	// Show the first tab:
	if (!m_Tabs.empty())
	{
		return std::make_pair(m_Tabs.front()->m_SafeTitle, m_Tabs.front()->m_SafeTitle);
	}

	// No tabs at all:
	return std::make_pair(AString(), AString());
}




AString cWebPlugin::SafeString(const AString & a_String)
{
	AString RetVal;
	auto len = a_String.size();
	RetVal.reserve(len);
	for (size_t i = 0; i < len; ++i)
	{
		char c = a_String[i];
		if (c == ' ')
		{
			c = '_';
		}
		RetVal.push_back(c);
	}
	return RetVal;
}





void cWebPlugin::AddNewWebTab(const AString & a_Title, int a_UserData)
{
	auto Tab = std::make_shared<cTab>(a_Title, a_UserData);
	cCSLock Lock(m_CSTabs);
	m_Tabs.push_back(Tab);
}





void cWebPlugin::ClearTabs(void)
{
	// Remove the webadmin tabs:
	cTabPtrs Tabs;
	{
		cCSLock Lock(m_CSTabs);
		std::swap(Tabs, m_Tabs);
	}
}




