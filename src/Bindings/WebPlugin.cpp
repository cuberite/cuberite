
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WebPlugin.h"
#include "../WebAdmin.h"
#include "../Server.h"
#include "../Root.h"





cWebPlugin::cWebPlugin()
{
	cWebAdmin * WebAdmin = cRoot::Get()->GetWebAdmin();
	if (WebAdmin != NULL)
	{
		WebAdmin->AddPlugin(this);
	}
}





cWebPlugin::~cWebPlugin()
{
	cWebAdmin * WebAdmin = cRoot::Get()->GetWebAdmin();
	if (WebAdmin != NULL)
	{
		WebAdmin->RemovePlugin(this);
	}

	for (TabList::iterator itr = m_Tabs.begin(); itr != m_Tabs.end(); ++itr)
	{
		delete *itr;
	}
	m_Tabs.clear();
}





std::list<std::pair<AString, AString> > cWebPlugin::GetTabNames(void)
{
	std::list< std::pair< AString, AString > > NameList;
	for (TabList::iterator itr = GetTabs().begin(); itr != GetTabs().end(); ++itr)
	{
		std::pair< AString, AString > StringPair;
		StringPair.first = (*itr)->Title;
		StringPair.second = (*itr)->SafeTitle;
		NameList.push_back( StringPair);
	}
	return NameList;
}





std::pair< AString, AString > cWebPlugin::GetTabNameForRequest(const HTTPRequest * a_Request)
{
	std::pair< AString, AString > Names;
	AStringVector Split = StringSplit(a_Request->Path, "/");

	if (Split.size() > 1)
	{
		sWebPluginTab * Tab = NULL;
		if (Split.size() > 2)  // If we got the tab name, show that page
		{
			for (TabList::iterator itr = GetTabs().begin(); itr != GetTabs().end(); ++itr)
			{
				if ((*itr)->SafeTitle.compare(Split[2]) == 0)  // This is the one!
				{
					Tab = *itr;
					break;
				}
			}
		}
		else  // Otherwise show the first tab
		{
			if (GetTabs().size() > 0)
				Tab = *GetTabs().begin();
		}

		if (Tab != NULL)
		{
			Names.first = Tab->Title;
			Names.second = Tab->SafeTitle;
		}
	}

	return Names;
}




AString cWebPlugin::SafeString(const AString & a_String)
{
	AString RetVal;
	for (unsigned int i = 0; i < a_String.size(); ++i)
	{
		char c = a_String[i];
		if (c == ' ')
		{
			c = '_';
		}
		RetVal.push_back( c);
	}
	return RetVal;
}




