
#pragma once

struct lua_State;
struct HTTPRequest;





// tolua_begin
class cWebPlugin
{
public:
	// tolua_end
	cWebPlugin();
	virtual ~cWebPlugin();

	// tolua_begin
	virtual const AString GetWebTitle(void) const = 0;
	
	virtual AString HandleWebRequest(const HTTPRequest * a_Request) = 0;

	static AString SafeString( const AString & a_String);
	// tolua_end

	struct sWebPluginTab
	{
		std::string Title;
		std::string SafeTitle;

		int UserData;
	};

	typedef std::list< sWebPluginTab* > TabList;
	TabList & GetTabs() { return m_Tabs; }

	typedef std::list< std::pair<AString, AString> > TabNameList;
	TabNameList GetTabNames();  // >> EXPORTED IN MANUALBINDINGS <<
	std::pair< AString, AString > GetTabNameForRequest(const HTTPRequest* a_Request);

private:
	TabList m_Tabs;
};  // tolua_export




