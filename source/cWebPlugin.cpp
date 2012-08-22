
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWebPlugin.h"
#include "cWebAdmin.h"
#include "cServer.h"
#include "cRoot.h"





cWebPlugin::cWebPlugin()
{
	LOG("cWebPlugin::cWebPlugin()");
	cWebAdmin* WebAdmin = cRoot::Get()->GetWebAdmin();
	if( WebAdmin ) WebAdmin->AddPlugin( this );
}





cWebPlugin::~cWebPlugin()
{
	LOG("~cWebPlugin::cWebPlugin()");
	cWebAdmin* WebAdmin = cRoot::Get()->GetWebAdmin();
	if( WebAdmin ) WebAdmin->RemovePlugin( this );

	for( TabList::iterator itr = m_Tabs.begin(); itr != m_Tabs.end(); ++itr )
	{
		delete *itr;
	}
	m_Tabs.clear();
}





std::list< std::pair<std::string, std::string> > cWebPlugin::GetTabNames()
{
	std::list< std::pair< std::string, std::string > > NameList;
	for( TabList::iterator itr = GetTabs().begin(); itr != GetTabs().end(); ++itr )
	{
		std::pair< std::string, std::string > StringPair;
		StringPair.first = (*itr)->Title;
		StringPair.second = (*itr)->SafeTitle;
		NameList.push_back( StringPair );
	}
	return NameList;
}





std::pair< std::string, std::string > cWebPlugin::GetTabNameForRequest( HTTPRequest* a_Request )
{
	std::pair< std::string, std::string > Names;
	AStringVector Split = StringSplit(a_Request->Path, "/");

	if( Split.size() > 1 )
	{
		sWebPluginTab* Tab = 0;
		if( Split.size() > 2 )	// If we got the tab name, show that page
		{
			for( TabList::iterator itr = GetTabs().begin(); itr != GetTabs().end(); ++itr )
			{
				if( (*itr)->SafeTitle.compare( Split[2] ) == 0 ) // This is the one! Rawr
				{
					Tab = *itr;
					break;
				}
			}
		}
		else	// Otherwise show the first tab
		{
			if( GetTabs().size() > 0 )
				Tab = *GetTabs().begin();
		}

		if( Tab )
		{
			Names.first = Tab->Title;
			Names.second = Tab->SafeTitle;
		}
	}

	return Names;
}




AString cWebPlugin::SafeString( const AString & a_String )
{
	std::string RetVal;
	for( unsigned int i = 0; i < a_String.size(); ++i )
	{
		char c = a_String[i];
		if( c == ' ' ) 
		{
			c = '_';
		}
		RetVal.push_back( c );
	}
	return RetVal;
}