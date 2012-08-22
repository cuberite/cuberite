
#pragma once

struct lua_State;
struct HTTPRequest;
//tolua_begin
class cWebPlugin
{
public:
	cWebPlugin();
	virtual ~cWebPlugin();
	
	void SetName( std::string a_Name ) { m_Name = a_Name; }
	std::string GetName() { return m_Name; }

	virtual std::string HandleWebRequest( HTTPRequest* a_Request ) = 0;

	static AString SafeString( const AString & a_String );
	//tolua_end

	struct sWebPluginTab
	{
		std::string Title;
		std::string SafeTitle;

		int UserData;
	};

	typedef std::list< sWebPluginTab* > TabList;
	TabList & GetTabs() { return m_Tabs; }

	std::list< std::pair<std::string, std::string> > GetTabNames();
	std::pair< std::string, std::string > GetTabNameForRequest( HTTPRequest* a_Request );

private:
	TabList m_Tabs;
	std::string m_Name;
}; //tolua_export